#include <stdlib.h>
#include <string.h>
#include "entry-consumer.h"
#include "../datastructures/key-value.h"
#include "../c-polyfill.h"
#include "../datastructures/gc.h"
#include "../datastructures/arena.h"
#include "../panic.h";

struct Station
{
    float minimum;
    float maximum;
    float mean;
    unsigned int count;
};

struct KeyValue *stations = nullptr;
struct Arena *arena = nullptr;

void consumeEntry(char *station, float measurement)
{
    if (stations == nullptr)
    {
        stations = keyValueConstructor((struct KeyValueOptions){256});
        if (stations == nullptr)
            return panic("OOM");
    }
    if (arena == nullptr)
    {
        arena = arenaConstructor((struct ArenaOptions){1024 * 640});
        if (arena == nullptr)
            return panic("OOM");
    }
    if (keyValueHas(stations, station))
    {
        struct Station *value = keyValueGet(stations, station);
        if (measurement < value->minimum)
            value->minimum = measurement;
        if (measurement > value->maximum)
            value->maximum = measurement;
        value->mean = (value->count * value->mean + measurement) / (value->count + 1);
        ++value->count;
    }
    else
    {
        struct Station *value = arenaPush(arena, sizeof(struct Station));
        if (value == nullptr)
            return panic("OOM");
        value->minimum = measurement;
        value->maximum = measurement;
        value->mean = measurement;
        value->count = 1;
        keyValueSet(stations, station, value);
    }
}

int compareRunResultEntries(const void *a, const void *b)
{
    return strcmp(
        ((const struct RunResultEntry *)a)->station,
        ((const struct RunResultEntry *)b)->station);
}

struct RunResult collectConsumedEntries()
{
    struct RunResult result = (struct RunResult){nullptr, 0};
    unsigned int kvLength = keyValueLength(stations);
    if (kvLength > 0)
    {
        result.entries = gc_malloc(sizeof(struct RunResultEntry) * kvLength);
        if (result.entries == nullptr)
        {
            panic("OOM");
            return (struct RunResult){nullptr, 0};
        }
    }
    result.length = kvLength;
    struct KeyValueEntry *entries = keyValueEntries(stations);
    for (unsigned int i = 0; i < kvLength; ++i)
    {
        struct KeyValueEntry entry = entries[i];
        struct Station *value = entry.value;
        result.entries[i].station = entry.key;
        result.entries[i].minimum = value->minimum;
        result.entries[i].maximum = value->maximum;
        result.entries[i].mean = value->mean;
    }
    qsort(result.entries, kvLength, sizeof(struct RunResultEntry), compareRunResultEntries);
    return result;
}

void cleanUpConsumedEntities()
{
    if (stations != nullptr)
    {
        keyValueDeconstructor(stations);
        stations = nullptr;
    }
    if (arena != nullptr)
    {
        arenaDeconstructor(arena);
        arena = nullptr;
    }
}
