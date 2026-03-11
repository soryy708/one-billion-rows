#define _POSIX_C_SOURCE 200112L // for `fileno` in <stdio.h>, posix_fadvise in <fcntl.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <alloca.h>
#include "stream-file.h"
#include "../signal-handler.h"
#include "../debug-log.h"
#include "../panic.h"
// POSIX Specific:
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

void streamFile(FILE *file, FileStreamChunkObserver observer, ...)
{
    fseek(file, 0, SEEK_END);
    const long lastPosition = ftell(file);
    rewind(file);

    posix_fadvise(fileno(file), 0, lastPosition, POSIX_FADV_SEQUENTIAL);

    unsigned int bufferSize = getStreamFileChunkSize();

    char *str = alloca(sizeof(char) * (bufferSize + 1));
    size_t bytesRead;
    unsigned int i = 0;
    while (((bytesRead = fread(str, sizeof(char), bufferSize, file)) > 0) && !signalledToStop())
    {
        debugPrintf("Reading file %.1f%%\n", 100.0f * (i * bufferSize) / lastPosition);

        str[bufferSize] = '\0';

        va_list args;
        va_start(args, observer);
        observer(str, args);
        va_end(args);

        ++i;
    }
}

const unsigned int getStreamFileChunkSize()
{
    return sysconf(_SC_PAGE_SIZE);
}
