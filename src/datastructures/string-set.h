#ifndef STRING_SET_H_INCLUDED
#define STRING_SET_H_INCLUDED

// aka String Pool
// Re-use strings to avoid allocating duplicates

#include "arena.h"

struct StringSet;

struct StringSetOptions
{
    struct Arena *arena;
};

struct StringSet *stringSetConstructor(struct StringSetOptions);
void stringSetDeconstructor(struct StringSet *);
char *stringSetAdd(struct StringSet *, char *, size_t);

#endif
