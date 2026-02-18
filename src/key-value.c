#include <stdlib.h>
#include <string.h>
#include "key-value.h"
#include "c-polyfill.h"
#include "panic.h"

struct InternalKeyValueEntry
{
    char *key;
    void *value;
};

struct KeyValue
{
    struct InternalKeyValueEntry *entries;
    unsigned int length;
};

struct KeyValue *keyValueConstructor()
{
    struct KeyValue *kv = malloc(sizeof(struct KeyValue));
    if (kv == nullptr)
    {
        panic("OOM");
        return nullptr;
    }
    kv->entries = nullptr;
    kv->length = 0;
    return kv;
}

bool keyValueHas(struct KeyValue *kv, char *key)
{
    if (kv == nullptr)
        return false;
    for (unsigned int i = 0; i < kv->length; ++i)
        if (strcmp(key, kv->entries[i].key) == 0)
            return true;
    return false;
}

void *keyValueGet(struct KeyValue *kv, char *key)
{
    if (kv == nullptr)
        return false;
    for (unsigned int i = 0; i < kv->length; ++i)
        if (strcmp(key, kv->entries[i].key) == 0)
            return kv->entries[i].value;
    return nullptr;
}

void keyValueSet(struct KeyValue *kv, char *key, void *value)
{
    if (kv == nullptr)
        return;
    for (unsigned int i = 0; i < kv->length; ++i)
        if (strcmp(key, kv->entries[i].key) == 0)
        {
            kv->entries[i].value = value;
            return;
        }
    struct InternalKeyValueEntry *temp = realloc(kv->entries, sizeof(struct InternalKeyValueEntry) * (kv->length + 1));
    if (temp == nullptr)
        return panic("OOM");
    kv->entries = temp;
    kv->entries[kv->length].key = key;
    kv->entries[kv->length].value = value;
    ++kv->length;
}

struct KeyValueEntry *keyValueEntries(struct KeyValue *kv)
{
    if (kv == nullptr || kv->length == 0)
        return nullptr;
    struct KeyValueEntry *entries = malloc(sizeof(struct KeyValueEntry) * kv->length);
    for (unsigned int i = 0; i < kv->length; ++i)
        entries[i] = (struct KeyValueEntry){kv->entries[i].key, kv->entries[i].value};
    return entries;
}

unsigned int keyValueLength(struct KeyValue *kv)
{
    if (kv == nullptr)
        return 0;
    return kv->length;
}
