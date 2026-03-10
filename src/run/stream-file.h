#ifndef STREAM_FILE_H_INCLUDED
#define STREAM_FILE_H_INCLUDED

#include <stdio.h>
#include <stdarg.h>

typedef void (*FileStreamChunkObserver)(char *chunk, va_list args);

void streamFile(FILE *file, FileStreamChunkObserver observer, ...);
const unsigned int getStreamFileChunkSize(void);

#endif
