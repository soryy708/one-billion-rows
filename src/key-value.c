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
    struct InternalKeyValueEntry *entries;
    size_t length;
};

struct KeyValueCache
{
    char *key;
    NUMERIC_HASH hash;
    size_t positionInBucket;
};

struct KeyValue
{
    struct KeyValueBucket *buckets;
    struct KeyValueCache cache;
};

struct KeyValue *keyValueConstructor()
{
    struct KeyValue *kv = malloc(sizeof(struct KeyValue));
    if (kv == nullptr)
    {
        panic("OOM");
        return nullptr;
    }
    kv->buckets = malloc(sizeof(struct KeyValueBucket) * MAX_BUCKETS);
    if (kv->buckets == nullptr)
    {
        free(kv);
        panic("OOM");
        return nullptr;
    }
    for (size_t i = 0; i < MAX_BUCKETS; ++i)
    {
        kv->buckets[i].entries = nullptr;
        kv->buckets[i].length = 0;
    }
    kv->cache.key = nullptr;
    kv->cache.hash = 0;
    kv->cache.positionInBucket = -1;
    return kv;
}

NUMERIC_HASH hashKey(struct KeyValue *kv, char *key)
{
    if (kv->cache.key != nullptr && kv->cache.key == key)
        return kv->cache.hash;
    NUMERIC_HASH hash = 0;
    for (size_t i = 0; key[i] != '\0'; ++i)
        hash += key[i];
    return hash % MAX_BUCKETS;
}

bool keyValueHas(struct KeyValue *kv, char *key)
{
    if (kv == nullptr)
        return false;
    if (key == kv->cache.key)
        return kv->cache.positionInBucket != -1;
    NUMERIC_HASH hash = hashKey(kv, key);
    for (size_t j = 0; j < kv->buckets[hash].length; ++j)
        if (strcmp(key, kv->buckets[hash].entries[j].key) == 0)
        {
            kv->cache.key = key;
            kv->cache.hash = hash;
            kv->cache.positionInBucket = j;
            return true;
        }
    kv->cache.key = key;
    kv->cache.hash = hash;
    kv->cache.positionInBucket = -1;
    return false;
}

void *keyValueGet(struct KeyValue *kv, char *key)
{
    if (kv == nullptr)
        return false;
    if (key == kv->cache.key)
    {
        if (kv->cache.positionInBucket != -1)
            return kv->buckets[kv->cache.hash].entries[kv->cache.positionInBucket].value;
        else
            return nullptr;
    }
    NUMERIC_HASH hash = hashKey(kv, key);
    for (size_t j = 0; j < kv->buckets[hash].length; ++j)
        if (strcmp(key, kv->buckets[hash].entries[j].key) == 0)
        {
            kv->cache.key = key;
            kv->cache.hash = hash;
            kv->cache.positionInBucket = j;
            return kv->buckets[hash].entries[j].value;
        }
    kv->cache.key = key;
    kv->cache.hash = hash;
    kv->cache.positionInBucket = -1;
    return nullptr;
}

void keyValueSet(struct KeyValue *kv, char *key, void *value)
{
    if (kv == nullptr)
        return;
    NUMERIC_HASH hash = hashKey(kv, key);

    for (size_t j = 0; j < kv->buckets[hash].length; ++j)
        if (strcmp(key, kv->buckets[hash].entries[j].key) == 0)
        {
            kv->buckets[hash].entries[j].value = value;
            kv->cache.key = key;
            kv->cache.hash = hash;
            kv->cache.positionInBucket = j;
            return;
        }

    // Allocate in existing bucket
    if (kv->buckets[hash].entries == nullptr)
    {
        kv->buckets[hash].entries = malloc(sizeof(struct InternalKeyValueEntry));
        if (kv->buckets[hash].entries == nullptr)
            return panic("OOM");
    }
    else
    {
        struct InternalKeyValueEntry *temp = realloc(kv->buckets[hash].entries, sizeof(struct InternalKeyValueEntry) * (kv->buckets[hash].length + 1));
        if (temp == nullptr)
            return panic("OOM");
        kv->buckets[hash].entries = temp;
    }
    kv->buckets[hash].entries[kv->buckets[hash].length].key = key;
    kv->buckets[hash].entries[kv->buckets[hash].length].value = value;
    kv->cache.key = key;
    kv->cache.hash = hash;
    kv->cache.positionInBucket = kv->buckets[hash].length;
    ++kv->buckets[hash].length;
    return;
}

struct KeyValueEntry *keyValueEntries(struct KeyValue *kv)
{
    if (kv == nullptr)
        return nullptr;

    size_t length = keyValueLength(kv);

    struct KeyValueEntry *entries = malloc(sizeof(struct KeyValueEntry) * length);
    size_t c = 0;
    for (size_t i = 0; i < MAX_BUCKETS; ++i)
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
    for (size_t i = 0; i < MAX_BUCKETS; ++i)
        length += kv->buckets[i].length;
    return length;
}
