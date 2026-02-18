#ifndef RUN_H_INCLUDED
#define RUN_H_INCLUDED

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

struct RunResult run(void);

#endif
