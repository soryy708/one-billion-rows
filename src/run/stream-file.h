#ifndef STREAM_FILE_H_INCLUDED
#define STREAM_FILE_H_INCLUDED

#include <stdio.h>

typedef void (*FileStreamChunkObserver)(char *chunk);

void streamFile(FILE *file, FileStreamChunkObserver observer);
const unsigned int getStreamFileChunkSize(void);

#endif
