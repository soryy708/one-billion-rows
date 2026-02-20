#include <stdlib.h>
#include <string.h>
#include "key-value.h"
#include "c-polyfill.h"
#include "panic.h"

#define MAX_BUCKETS 256

typedef unsigned short NUMERIC_HASH;

struct InternalKeyValueEntry
{
    char *key;
    void *value;
};

struct KeyValueBucket
{
    NUMERIC_HASH hash;
    struct InternalKeyValueEntry *entries;
    size_t length;
};

struct KeyValue
{
    struct KeyValueBucket *buckets;
    size_t length;
};

struct KeyValue *keyValueConstructor()
{
    struct KeyValue *kv = malloc(sizeof(struct KeyValue));
    if (kv == nullptr)
    {
        panic("OOM");
        return nullptr;
    }
    kv->buckets = nullptr;
    kv->length = 0;
    return kv;
}

NUMERIC_HASH hashKey(char *key)
{
    NUMERIC_HASH hash = 0;
    for (size_t i = 0; key[i] != '\0'; ++i)
        hash += key[i];
    return hash % MAX_BUCKETS;
}

bool keyValueHas(struct KeyValue *kv, char *key)
{
    if (kv == nullptr)
        return false;
    NUMERIC_HASH hash = hashKey(key);
    for (size_t i = 0; i < kv->length; ++i)
        if (hash == kv->buckets[i].hash)
        {
            for (size_t j = 0; j < kv->buckets[i].length; ++j)
                if (strcmp(key, kv->buckets[i].entries[j].key) == 0)
                    return true;
            return false;
        }
    return false;
}

void *keyValueGet(struct KeyValue *kv, char *key)
{
    if (kv == nullptr)
        return false;
    NUMERIC_HASH hash = hashKey(key);
    for (size_t i = 0; i < kv->length; ++i)
        if (hash == kv->buckets[i].hash)
        {
            for (size_t j = 0; j < kv->buckets[i].length; ++j)
                if (strcmp(key, kv->buckets[i].entries[j].key) == 0)
                    return kv->buckets[i].entries[j].value;
            return nullptr;
        }
    return nullptr;
}

void keyValueSet(struct KeyValue *kv, char *key, void *value)
{
    if (kv == nullptr)
        return;
    NUMERIC_HASH hash = hashKey(key);
    for (size_t i = 0; i < kv->length; ++i)
        if (hash == kv->buckets[i].hash)
        {
            for (size_t j = 0; j < kv->buckets[i].length; ++j)
                if (strcmp(key, kv->buckets[i].entries[j].key) == 0)
                {
                    kv->buckets[i].entries[j].value = value;
                    return;
                }
            // Allocate in existing bucket
            struct InternalKeyValueEntry *temp = realloc(kv->buckets[i].entries, sizeof(struct InternalKeyValueEntry) * (kv->buckets[i].length + 1));
            if (temp == nullptr)
                return panic("OOM");
            kv->buckets[i].entries = temp;
            kv->buckets[i].entries[kv->buckets[i].length].key = key;
            kv->buckets[i].entries[kv->buckets[i].length].value = value;
            ++kv->buckets[i].length;
            return;
        }
    // Allocate new bucket
    if (kv->buckets == nullptr)
    {
        kv->buckets = malloc(sizeof(struct KeyValueBucket));
        if (kv->buckets == nullptr)
            return panic("OOM");
    }
    else
    {
        struct KeyValueBucket *temp = realloc(kv->buckets, sizeof(struct KeyValueBucket) * (kv->length + 1));
        if (temp == nullptr)
            return panic("OOM");
        kv->buckets = temp;
    }
    kv->buckets[kv->length].entries = malloc(sizeof(struct InternalKeyValueEntry));
    if (kv->buckets[kv->length].entries == nullptr)
        return panic("OOM");
    kv->buckets[kv->length].entries[0].key = key;
    kv->buckets[kv->length].entries[0].value = value;
    kv->buckets[kv->length].hash = hash;
    kv->buckets[kv->length].length = 1;
    ++kv->length;
}

struct KeyValueEntry *keyValueEntries(struct KeyValue *kv)
{
    if (kv == nullptr || kv->length == 0)
        return nullptr;

    size_t length = keyValueLength(kv);

    struct KeyValueEntry *entries = malloc(sizeof(struct KeyValueEntry) * length);
    size_t c = 0;
    for (size_t i = 0; i < kv->length; ++i)
        for (size_t j = 0; j < kv->buckets[i].length; ++j)
            entries[c++] = (struct KeyValueEntry){
                kv->buckets[i].entries[j].key,
                kv->buckets[i].entries[j].value};
    return entries;
}

unsigned int keyValueLength(struct KeyValue *kv)
{
    if (kv == nullptr)
        return 0;
    size_t length = 0;
    for (size_t i = 0; i < kv->length; ++i)
        length += kv->buckets[i].length;
    return length;
}
