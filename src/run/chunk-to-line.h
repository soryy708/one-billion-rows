#ifndef CHUNK_TO_LINE_H_INCLUDED
#define CHUNK_TO_LINE_H_INCLUDED

#include <stdarg.h>

typedef void (*FileStreamLineObserver)(char *line, va_list args);

void fileStreamChunkToLine(char *chunk, FileStreamLineObserver observer, va_list args);
void initFileStreamChunkToLine(void);
void cleanupFileStreamChunkToLine(void);

#endif
