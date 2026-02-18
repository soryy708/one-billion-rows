#ifndef KEY_VALUE_H_INCLUDED
#define KEY_VALUE_H_INCLUDED

#include "c-polyfill.h"

struct KeyValue;

struct KeyValueEntry
{
    char *key;
    void *value;
};

struct KeyValue *keyValueConstructor();
bool keyValueHas(struct KeyValue *kv, char *key);
void *keyValueGet(struct KeyValue *kv, char *key);
void keyValueSet(struct KeyValue *kv, char *key, void *value);
struct KeyValueEntry *keyValueEntries(struct KeyValue *kv);
unsigned int keyValueLength(struct KeyValue *kv);

#endif
