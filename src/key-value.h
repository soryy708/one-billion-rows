#ifndef KEY_VALUE_H_INCLUDED
#define KEY_VALUE_H_INCLUDED

#include "c-polyfill.h"

struct KeyValue;

struct KeyValueEntry
{
    char *key;
    void *value;
};

struct KeyValueOptions
{
    size_t maxBuckets;
};

struct KeyValue *keyValueConstructor(struct KeyValueOptions options);
void keyValueDeconstructor(struct KeyValue *kv);
bool keyValueHas(struct KeyValue *kv, char *key);
void *keyValueGet(struct KeyValue *kv, char *key);
void keyValueSet(struct KeyValue *kv, char *key, void *value);
struct KeyValueEntry *keyValueEntries(struct KeyValue *kv);
unsigned int keyValueLength(struct KeyValue *kv);

#endif
