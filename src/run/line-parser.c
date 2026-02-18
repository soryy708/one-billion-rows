#include <stdlib.h>
#include <string.h>
#include "line-parser.h"
#include "../c-polyfill.h"
#include "../panic.h"

const struct ParsedEntry parseLine(char *line)
{
    const char *delimiter = strchr(line, ';');
    const size_t stationLength = delimiter - line;
    if (stationLength <= 0)
    {
        panic("onStreamedLine called with corrupted line");
        return (struct ParsedEntry){nullptr, 0.0f};
    }

    char *station = malloc(sizeof(char) * (stationLength + 1));
    if (station == nullptr)
    {
        panic("OOM");
        return (struct ParsedEntry){nullptr, 0.0f};
    }
    memcpy(station, line, stationLength);
    station[stationLength] = '\0';

    const float measurement = strtof(delimiter + 1, nullptr);

    return (struct ParsedEntry){station, measurement};
}
