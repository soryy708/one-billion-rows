#include <stdlib.h>
#include <string.h>
#include "entry-consumer.h"
#include "../key-value.h"
#include "../c-polyfill.h"

struct Station
{
    float minimum;
    float maximum;
    float sum;
    unsigned int count;
};

struct KeyValue *stations = nullptr;

void consumeEntry(char *station, float measurement)
{
    if (stations == nullptr)
    {
        stations = keyValueConstructor((struct KeyValueOptions){256});
    }
    if (keyValueHas(stations, station))
    {
        struct Station *value = keyValueGet(stations, station);
        if (measurement < value->minimum)
            value->minimum = measurement;
        if (measurement > value->maximum)
            value->maximum = measurement;
        value->sum += measurement;
        ++value->count;
    }
    else
    {
        struct Station *value = malloc(sizeof(struct Station));
        value->minimum = measurement;
        value->maximum = measurement;
        value->sum = measurement;
        value->count = 1;
        keyValueSet(stations, station, value);
    }
}

int compareRunResultEntries(const struct RunResultEntry *a, const struct RunResultEntry *b)
{
    return strcmp(a->station, b->station);
}

struct RunResult collectConsumedEntries()
{
    struct RunResult result = (struct RunResult){nullptr, 0};
    unsigned int kvLength = keyValueLength(stations);
    if (kvLength > 0)
        result.entries = malloc(sizeof(struct RunResultEntry) * kvLength);
    result.length = kvLength;
    struct KeyValueEntry *entries = keyValueEntries(stations);
    for (unsigned int i = 0; i < kvLength; ++i)
    {
        struct KeyValueEntry entry = entries[i];
        struct Station *value = entry.value;
        result.entries[i].station = entry.key;
        result.entries[i].minimum = value->minimum;
        result.entries[i].maximum = value->maximum;
        result.entries[i].mean = value->sum / value->count;
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
}
