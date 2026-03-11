#ifndef LINE_PARSER_H_INCLUDED
#define LINE_PARSER_H_INCLUDED

#include "../datastructures/string-set.h"

struct ParsedEntry
{
    char *station;
    float measurement;
};

const struct ParsedEntry parseLine(char *line, struct StringSet *stringSet);

#endif
