#include <stdio.h>
#include <stdarg.h>
#include "debug-log.h"
#include "config.h"

void debugPrintf(char *format, ...)
{
    if (!ENABLE_DEBUG_LOGGING)
        return;
    va_list args;
    va_start(args, format);
    vfprintf(stdout, format, args);
    va_end(args);
}
