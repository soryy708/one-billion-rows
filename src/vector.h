#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

struct Vector;

enum VectorOptionsGrowthType
{
    QUADRATIC
};

struct VectorOptions
{
    enum VectorOptionsGrowthType growthType;
    size_t elementSize;
};

struct Vector *vectorConstructor(struct VectorOptions options);
void vectorDeconstructor(struct Vector *v);
unsigned int vectorLength(struct Vector *v);
void vectorPush(struct Vector *v, void *element);
void *vectorGet(struct Vector *v, unsigned int index);
void vectorForEach(struct Vector *v, void (*callback)(void *));
void vectorEmpty(struct Vector *v);

#endif
