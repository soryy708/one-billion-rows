#ifndef RUN_H_INCLUDED
#define RUN_H_INCLUDED

#include "../arena.h"

struct RunResultEntry
{
    char *station;
    float minimum;
    float maximum;
    float mean;
};

struct RunResult
{
    struct RunResultEntry *entries;
    unsigned int length;
};

struct RunResult run(char *inputFilePath, struct Arena *arena);

#endif
