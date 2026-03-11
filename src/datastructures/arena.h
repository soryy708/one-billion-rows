#ifndef ARENA_H_INCLUDED
#define ARENA_H_INCLUDED

// Arena aka linear/bump/region allocator
// https://www.youtube.com/watch?v=UeJPyuVxL-o

#include <stddef.h>

struct Arena;

struct ArenaOptions
{
    size_t chunkSize;
};

struct Arena *arenaConstructor(struct ArenaOptions options);
void arenaDeconstructor(struct Arena *a);
void *arenaPush(struct Arena *a, size_t size);

#endif
