#include <stdlib.h>
#include "gc.h"

void *gc_malloc(size_t size)
{
    return malloc(size);
}

void gc_free(void *p)
{
    return free(p);
}

void *gc_realloc(void *p, size_t size)
{
    return realloc(p, size);
}