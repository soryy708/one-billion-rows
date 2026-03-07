#ifndef GC_H_INCLUDED
#define GC_H_INCLUDED

#include <stddef.h>

void *gc_malloc(size_t size);
void gc_free(void *p);
void *gc_realloc(void *p, size_t size);
void gc_sweep(void);

#endif
