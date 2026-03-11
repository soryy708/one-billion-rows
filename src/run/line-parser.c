#include <stdlib.h>
#include <string.h>
#include "line-parser.h"
#include "../c-polyfill.h"
#include "../panic.h"

const struct ParsedEntry parseLine(char *line, struct Arena *arena)
{
    const char *delimiter = strchr(line, ';');
    const size_t stationLength = delimiter - line;
    if (stationLength <= 0)
    {
        panic("onStreamedLine called with corrupted line");
        return (struct ParsedEntry){nullptr, 0.0f};
    }

    char *station = arenaPush(arena, sizeof(char) * (stationLength + 1));
    if (station == nullptr)
    {
        panic("OOM");
        return (struct ParsedEntry){nullptr, 0.0f};
    }
    memcpy(station, line, stationLength);
    station[stationLength] = '\0';

    float measurement = 0.0f;
    if (delimiter[1] == '-')
    {
        if (delimiter[3] == '.')
        {
            // -x.y
            measurement = -1.0f * ((delimiter[2] - '0') + (delimiter[4] - '0'));
        }
        else
        {
            // -xy.z
            measurement = -1.0f * (10.0f * (delimiter[2] - '0') + (delimiter[3] - '0') + (delimiter[4] - '0'));
        }
    }
    else
    {
        if (delimiter[2] == '.')
        {
            // x.y
            measurement = ((delimiter[2] - '0') + (delimiter[4] - '0'));
        }
        else
        {
            // xy.z
            measurement = (10.0f * (delimiter[2] - '0') + (delimiter[3] - '0') + (delimiter[4] - '0'));
        }
    }

    return (struct ParsedEntry){station, measurement};
}
