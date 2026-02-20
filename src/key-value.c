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

struct KeyValue
{
    struct KeyValueBucket *buckets;
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
    for (size_t j = 0; j < kv->buckets[hash].length; ++j)
        if (strcmp(key, kv->buckets[hash].entries[j].key) == 0)
            return true;
    return false;
}

void *keyValueGet(struct KeyValue *kv, char *key)
{
    if (kv == nullptr)
        return false;
    NUMERIC_HASH hash = hashKey(key);
    for (size_t j = 0; j < kv->buckets[hash].length; ++j)
        if (strcmp(key, kv->buckets[hash].entries[j].key) == 0)
            return kv->buckets[hash].entries[j].value;
    return nullptr;
}

void keyValueSet(struct KeyValue *kv, char *key, void *value)
{
    if (kv == nullptr)
        return;
    NUMERIC_HASH hash = hashKey(key);

    for (size_t j = 0; j < kv->buckets[hash].length; ++j)
        if (strcmp(key, kv->buckets[hash].entries[j].key) == 0)
        {
            kv->buckets[hash].entries[j].value = value;
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
