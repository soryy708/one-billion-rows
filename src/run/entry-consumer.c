#include <stdlib.h>
#include "entry-consumer.h"
#include "../key-value.h"
#include "../c-polyfill.h"

struct Station
{
    float minimum;
    float maximum;
    float *measurements;
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
        // TODO: measurements for mean
    }
    else
    {
        struct Station *value = malloc(sizeof(struct Station));
        value->minimum = measurement;
        value->maximum = measurement;
        value->measurements = nullptr; // TODO
        keyValueSet(stations, station, value);
    }
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
        result.entries[i].mean = 0; // TODO
    }
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
