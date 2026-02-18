#ifndef CHUNK_TO_LINE_H_INCLUDED
#define CHUNK_TO_LINE_H_INCLUDED

typedef void (*FileStreamLineObserver)(char *line);

void fileStreamChunkToLine(char *chunk, FileStreamLineObserver observer);
void initFileStreamChunkToLine(void);
void cleanupFileStreamChunkToLine(void);

#endif
