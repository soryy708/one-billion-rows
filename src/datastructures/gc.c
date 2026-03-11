#include <stdlib.h>
#include "gc.h"
#include "../c-polyfill.h"
#include "vector.h"

struct Vector *freeableMemory = nullptr;
struct Vector *assertFreeableMemoryVector(void)
{
    if (freeableMemory == nullptr)
        freeableMemory = vectorConstructor((struct VectorOptions){QUADRATIC, sizeof(void *)});
    return freeableMemory;
}

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
    struct Vector *freeableMemory = assertFreeableMemoryVector();
    if (freeableMemory == nullptr)
    {
        gc_sweep();
        freeableMemory = assertFreeableMemoryVector();
        if (freeableMemory == nullptr)
        {
            free(p);
            return;
        }
    }
    vectorPush(freeableMemory, p);
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

void gc_sweep_item(void *p)
{
    free(p);
}

void gc_sweep(void)
{
    struct Vector *v = assertFreeableMemoryVector();
    vectorForEach(v, gc_sweep_item);
    vectorEmpty(v);
}
