#include <stdlib.h>
#include "gc.h"

void *gc_malloc(size_t size)
{
    return malloc(size);
}

void gc_free(void *p)
{
    // TODO: Defer freeing of memory to gc_sweep
    return free(p);
}

void *gc_realloc(void *p, size_t size)
{
    return realloc(p, size);
}

void gc_sweep(void)
{
    // TODO: Actually free the freeable memory
}
