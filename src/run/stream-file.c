#include <stdio.h>
#include <stddef.h>
#include "stream-file.h"
#include "../signal-handler.h"
#include "../debug-log.h"

#define BUFFER_SIZE (4 * 1024)

void streamFile(FILE *file, FileStreamChunkObserver observer)
{
    fseek(file, 0, SEEK_END);
    const long lastPosition = ftell(file);
    rewind(file);

    char buffer[BUFFER_SIZE];
    unsigned int bytesRead;
    while (((bytesRead = fread(buffer, sizeof(char), BUFFER_SIZE - 1, file)) > 0) && !signalledToStop())
    {
        debugPrintf("Reading file %.1f%%\n", (float)ftell(file) / lastPosition);
        buffer[bytesRead] = '\0';
        observer(buffer);
    }
}

const unsigned int getStreamFileChunkSize()
{
    return BUFFER_SIZE;
}
