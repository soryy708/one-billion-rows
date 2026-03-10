#ifndef LINE_PARSER_H_INCLUDED
#define LINE_PARSER_H_INCLUDED

#include "../arena.h"

struct ParsedEntry
{
    char *station;
    float measurement;
};

const struct ParsedEntry parseLine(char *line, struct Arena *arena);

#endif
