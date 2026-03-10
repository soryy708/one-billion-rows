#include <stdio.h>
#include "run.h"
#include "stream-file.h"
#include "chunk-to-line.h"
#include "line-parser.h"
#include "entry-producer.h"
#include "entry-consumer.h"
#include "../c-polyfill.h"
#include "../panic.h"

void onStreamedEntry(struct ParsedEntry entry)
{
    produceEntry(entry.station, entry.measurement);
}

void onStreamedLine(char *line)
{
    onStreamedEntry(parseLine(line));
}

void onStreamedChunk(char *chunk)
{
    fileStreamChunkToLine(chunk, onStreamedLine);
}

struct RunResult run(char *inputFilePath)
{
    initFileStreamChunkToLine();
    FILE *file = fopen(inputFilePath, "rb");
    if (file == nullptr)
    {
        panic("Failed to open file for reading");
        return (struct RunResult){
            nullptr,
            0};
    }
    streamFile(file, onStreamedChunk);
    fclose(file);
    cleanupFileStreamChunkToLine();

    struct RunResult result = collectConsumedEntries();
    cleanUpConsumedEntities();
    return result;
}
