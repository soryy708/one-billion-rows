#include <stdlib.h>
#include "vector.h"
#include "../c-polyfill.h"

struct Vector
{
    size_t allocatedLength;
    size_t length;
    void **elements;
    struct VectorOptions options;
};

struct Vector *vectorConstructor(struct VectorOptions options)
{
    struct Vector *v = malloc(sizeof(struct Vector));
    if (v == nullptr)
        return nullptr; // Panic
    v->allocatedLength = 0;
    v->length = 0;
    v->elements = nullptr;
    v->options = options;
    return v;
}

void vectorDeconstructor(struct Vector *v)
{
    if (v == nullptr)
        return;
    if (v->elements != nullptr)
        free(v->elements);
    free(v);
}

unsigned int vectorLength(struct Vector *v)
{
    if (v == nullptr)
        return 0;
    return v->length;
}

void growVector(struct Vector *v)
{
    if (v == nullptr)
        return;

    if (v->elements == nullptr)
    {
        v->elements = malloc(v->options.elementSize);
        if (v->elements == nullptr)
            return; // Panic
        v->allocatedLength = 1;
        return;
    }

    switch (v->options.growthType)
    {
    case QUADRATIC:
    {
        void **realloced = realloc(v->elements, v->options.elementSize * 2 * v->allocatedLength);
        if (realloced == nullptr)
            return; // Panic
        v->elements = realloced;
        v->allocatedLength *= 2;
        break;
    }
    default:
        return; // Panic
    }
}

void vectorPush(struct Vector *v, void *element)
{
    if (v == nullptr)
        return;

    if (v->length >= v->allocatedLength)
        growVector(v);
    if (v->length >= v->allocatedLength)
        return; // Panic, growVector failed

    v->elements[(v->options.elementSize / sizeof(void *)) * v->length] = element;
    ++v->length;
}

void *vectorGet(struct Vector *v, unsigned int index)
{
    if (v == nullptr || index >= v->length)
        return nullptr;
    return v->elements[(v->options.elementSize / sizeof(void *)) * index];
}

void vectorForEach(struct Vector *v, void (*callback)(void *))
{
    if (v == nullptr)
        return;
    for (unsigned int i = 0; i < vectorLength(v); ++i)
        callback(vectorGet(v, i));
}

void vectorEmpty(struct Vector *vector)
{
    if (vector == nullptr)
        return;
    if (vector->elements != nullptr)
    {
        free(vector->elements);
        vector->elements = nullptr;
    }
    vector->length = 0;
    vector->allocatedLength = 0;
}
