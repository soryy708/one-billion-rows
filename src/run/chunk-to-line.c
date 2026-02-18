#include <stdlib.h>
#include <string.h>
#include "chunk-to-line.h"
#include "stream-file.h"
#include "../c-polyfill.h"
#include "../panic.h"

char *chunkOverflow = nullptr;
unsigned int chunkOverflowLength = 0;

void initFileStreamChunkToLine(void)
{
    chunkOverflow = malloc(sizeof(char) * getStreamFileChunkSize());
    if (chunkOverflow == nullptr)
        return panic("OOM");
    addPanicObserver(cleanupFileStreamChunkToLine);
}

void cleanupFileStreamChunkToLine(void)
{
    if (chunkOverflow != nullptr)
    {
        free(chunkOverflow);
        chunkOverflow = nullptr;
    }
}

void fileStreamChunkToLine(char *chunk, FileStreamLineObserver observer)
{
    unsigned int chunkLength = getStreamFileChunkSize();
    char line[chunkLength];

    unsigned int lineLength = 0;
    if (chunkOverflowLength > 0)
    {
        memcpy(line, chunkOverflow, chunkOverflowLength);
        lineLength = chunkOverflowLength;
        chunkOverflowLength = 0;
    }
    for (unsigned int i = lineLength; i < chunkLength; ++i)
        line[i] = '\0';

    for (unsigned int i = 0; i < chunkLength && chunk[i] != '\0'; ++i)
    {
        if (chunk[i] == '\r' || chunk[i] == '\n')
        {
            if (lineLength > 0)
            {
                line[lineLength] = '\0';
                observer(line);
                lineLength = 0;
            }
        }
        else
        {
            line[lineLength++] = chunk[i];
        }
    }
    if (lineLength > 0)
    {
        memcpy(chunkOverflow, line, lineLength);
        chunkOverflowLength = lineLength;
    }
}
