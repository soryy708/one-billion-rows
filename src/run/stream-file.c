#include <stdio.h>
#include <stddef.h>
#include "stream-file.h"
#include "../signal-handler.h"

#define BUFFER_SIZE 1024

void streamFile(FILE *file, FileStreamChunkObserver observer)
{
    char buffer[BUFFER_SIZE];
    unsigned int bytesRead;
    while (((bytesRead = fread(buffer, sizeof(char), BUFFER_SIZE - 1, file)) > 0) && !signalledToStop())
    {
        buffer[bytesRead] = '\0';
        observer(buffer);
    }
}

const unsigned int getStreamFileChunkSize()
{
    return BUFFER_SIZE;
}
