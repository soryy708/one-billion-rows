#include <stdlib.h>
#include "gc.h"
#include "c-polyfill.h"

void *gc_malloc(size_t size)
{
    void *malloced = malloc(size);
    if (malloced == nullptr)
    {
        gc_sweep();
        malloced = malloc(size);
    }
    return malloced;
}

void gc_free(void *p)
{
    // TODO: Defer freeing of memory to gc_sweep
    return free(p);
}

void *gc_realloc(void *p, size_t size)
{
    void *realloced = realloc(p, size);
    if (realloced == nullptr)
    {
        gc_sweep();
        realloced = realloc(p, size);
    }
    return realloced;
}

void gc_sweep(void)
{
    // TODO: Actually free the freeable memory
}
