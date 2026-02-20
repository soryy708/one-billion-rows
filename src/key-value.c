#include <stdlib.h>
#include <string.h>
#include "key-value.h"
#include "c-polyfill.h"
#include "panic.h"

typedef unsigned short numeric_hash;

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
    numeric_hash hash;
    size_t positionInBucket;
};

struct KeyValue
{
    struct KeyValueBucket *buckets;
    size_t maxBuckets;
    struct KeyValueCache cache;
};

struct KeyValue *keyValueConstructor(struct KeyValueOptions options)
{
    struct KeyValue *kv = malloc(sizeof(struct KeyValue));
    if (kv == nullptr)
    {
        panic("OOM");
        return nullptr;
    }
    kv->maxBuckets = options.maxBuckets;
    kv->buckets = malloc(sizeof(struct KeyValueBucket) * kv->maxBuckets);
    if (kv->buckets == nullptr)
    {
        free(kv);
        panic("OOM");
        return nullptr;
    }
    for (size_t i = 0; i < kv->maxBuckets; ++i)
    {
        kv->buckets[i].entries = nullptr;
        kv->buckets[i].length = 0;
    }
    kv->cache.key = nullptr;
    kv->cache.hash = 0;
    kv->cache.positionInBucket = -1;
    return kv;
}

void keyValueDeconstructor(struct KeyValue *kv)
{
    if (kv == nullptr)
        return;
    for (size_t i = 0; i < kv->maxBuckets; ++i)
        free(kv->buckets[i].entries);
    free(kv->buckets);
}

numeric_hash hashKey(struct KeyValue *kv, char *key)
{
    if (kv->cache.key != nullptr && kv->cache.key == key)
        return kv->cache.hash;
    numeric_hash hash = 0;
    for (size_t i = 0; key[i] != '\0'; ++i)
        hash += key[i];
    return hash % kv->maxBuckets;
}

size_t findKeyInBucket(struct KeyValue *kv, char *key, size_t bucket)
{
    for (size_t j = 0; j < kv->buckets[bucket].length; ++j)
        if (strcmp(key, kv->buckets[bucket].entries[j].key) == 0)
        {
            return j;
        }
    return -1;
}

bool keyValueHas(struct KeyValue *kv, char *key)
{
    if (kv == nullptr)
        return false;
    if (key == kv->cache.key)
        return kv->cache.positionInBucket != -1;
    numeric_hash hash = hashKey(kv, key);

    size_t j = findKeyInBucket(kv, key, hash);
    kv->cache.key = key;
    kv->cache.hash = hash;
    kv->cache.positionInBucket = j;
    return (j != -1);
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
    numeric_hash hash = hashKey(kv, key);

    size_t j = findKeyInBucket(kv, key, hash);
    kv->cache.key = key;
    kv->cache.hash = hash;
    kv->cache.positionInBucket = j;
    return j != -1 ? kv->buckets[hash].entries[j].value : nullptr;
}

void keyValueSet(struct KeyValue *kv, char *key, void *value)
{
    if (kv == nullptr)
        return;
    numeric_hash hash = hashKey(kv, key);

    size_t j = findKeyInBucket(kv, key, hash);
    if (j != -1)
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
        kv->buckets[hash].entries[0].key = key;
        kv->buckets[hash].entries[0].value = value;
        kv->cache.key = key;
        kv->cache.hash = hash;
        kv->cache.positionInBucket = 0;
        kv->buckets[hash].length = 1;
    }
    else
    {
        // Binary search
        int low = 0;
        int high = kv->buckets[hash].length - 1;
        while (low <= high)
        {
            int mid = low + (high - low) / 2;
            int stringOrder = strcmp(key, kv->buckets[hash].entries[mid].key);
            if (stringOrder < 0)
                low = mid + 1;
            else
                high = mid - 1;
        }
        int splicePosition = low;

        struct InternalKeyValueEntry *temp = realloc(
            kv->buckets[hash].entries,
            (kv->buckets[hash].length + 1) * sizeof(struct InternalKeyValueEntry));
        if (temp == nullptr)
            return panic("OOM");
        kv->buckets[hash].entries = temp;

        if (splicePosition < kv->buckets[hash].length)
        {
            memmove(
                &kv->buckets[hash].entries[splicePosition + 1],
                &kv->buckets[hash].entries[splicePosition],
                (kv->buckets[hash].length - splicePosition) * sizeof(struct InternalKeyValueEntry));
        }

        kv->buckets[hash].entries[splicePosition].key = key;
        kv->buckets[hash].entries[splicePosition].value = value;
        kv->cache.key = key;
        kv->cache.hash = hash;
        kv->cache.positionInBucket = splicePosition;
        ++kv->buckets[hash].length;
    }
}

struct KeyValueEntry *keyValueEntries(struct KeyValue *kv)
{
    if (kv == nullptr)
        return nullptr;

    size_t length = keyValueLength(kv);

    struct KeyValueEntry *entries = malloc(sizeof(struct KeyValueEntry) * length);
    size_t c = 0;
    for (size_t i = 0; i < kv->maxBuckets; ++i)
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
    for (size_t i = 0; i < kv->maxBuckets; ++i)
        length += kv->buckets[i].length;
    return length;
}
