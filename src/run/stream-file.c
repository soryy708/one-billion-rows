#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stream-file.h"
#include "../signal-handler.h"
#include "../debug-log.h"
#include "../panic.h"
// POSIX Specific:
#include <sys/mman.h>
#include <unistd.h>

void streamFile(FILE *file, FileStreamChunkObserver observer)
{
    fseek(file, 0, SEEK_END);
    const long lastPosition = ftell(file);
    rewind(file);

    unsigned int bufferSize = getStreamFileChunkSize();

    char *str = malloc(sizeof(char) * (bufferSize + 1));
    if (str == nullptr)
        return panic("OOM");
    for (unsigned int i = 0; i < lastPosition / bufferSize && !signalledToStop(); ++i)
    {
        debugPrintf("Reading file %.1f%%\n", (i * bufferSize) / lastPosition);

        char *address = mmap(nullptr, bufferSize, PROT_READ, MAP_PRIVATE, fileno(file), i * bufferSize);
        if (address == MAP_FAILED)
        {
            panic("mmap Failed");
            free(str);
            return;
        }

        memcpy(str, address, bufferSize);
        str[bufferSize] = '\0';
        observer(str);

        munmap(address, bufferSize);
    }
    if (!signalledToStop())
    {
        unsigned int lastChunkStart = (lastPosition / bufferSize) * bufferSize;
        if (lastChunkStart != lastPosition)
        {
            fseek(file, lastChunkStart, SEEK_SET);
            unsigned int bytesRead = fread(str, sizeof(char), bufferSize, file);
            if (bytesRead > 0)
            {
                str[bytesRead] = '\0';
                observer(str);
            }
        }
    }
    free(str);
}

const unsigned int getStreamFileChunkSize()
{
    return sysconf(_SC_PAGE_SIZE);
}
