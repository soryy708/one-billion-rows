#include <stdlib.h>
#include <string.h>
#include "arena.h"
#include "c-polyfill.h"

struct Metadata
{
    size_t size;
};

struct ArenaBlock
{
    void *chunk; // Array of Metadata followed by user's memory
    void *chunkEnd;
    struct ArenaBlock *next; // Linked list
};

struct Arena
{
    struct ArenaOptions options;
    struct ArenaBlock *firstBlock;
    struct ArenaBlock *lastBlock;
};

struct Arena *arenaConstructor(struct ArenaOptions options)
{
    struct Arena *a = malloc(sizeof(struct Arena));
    if (a == nullptr)
        return nullptr;
    a->options = options;
    a->firstBlock = nullptr;
    a->lastBlock = nullptr;
    return a;
}

void deallocateBlocks(struct ArenaBlock *block)
{
    if (block == nullptr)
        return;
    if (block->next != nullptr)
        deallocateBlocks(block->next);
    if (block->chunk != nullptr)
        free(block->chunk);
    free(block);
}

void arenaDeconstructor(struct Arena *a)
{
    if (a == nullptr)
        return;
    deallocateBlocks(a->firstBlock);
    free(a);
}

bool blockHasRoom(struct ArenaBlock *block, size_t chunkSize, size_t requestedSize)
{
    if (block == nullptr || block->chunk == nullptr)
        return false;
    size_t usedChunkSize = block->chunkEnd - block->chunk;
    size_t neededSpace = usedChunkSize + sizeof(struct Metadata) + requestedSize;
    return neededSpace <= chunkSize;
}

struct ArenaBlock *appendNewBlock(struct Arena *a, struct ArenaBlock *block)
{
    if (a == nullptr || block == nullptr)
        return nullptr;
    block->next = malloc(sizeof(struct ArenaBlock));
    if (block->next == nullptr)
        return nullptr;
    struct ArenaBlock *created = block->next;
    created->chunk = malloc(a->options.chunkSize);
    if (created->chunk == nullptr)
    {
        free(block->next);
        block->next = nullptr;
        return nullptr;
    }
    created->chunkEnd = created->chunk;
    created->next = nullptr;
    a->lastBlock = created;
    return created;
}

void *arenaPush(struct Arena *a, size_t size)
{
    if (a == nullptr)
        return nullptr;
    if (size + sizeof(struct Metadata) > a->options.chunkSize)
        return nullptr;
    if (a->firstBlock == nullptr)
    {
        a->firstBlock = malloc(sizeof(struct ArenaBlock));
        if (a->firstBlock == nullptr)
            return nullptr;
        a->firstBlock->chunk = malloc(a->options.chunkSize);
        if (a->firstBlock->chunk == nullptr)
        {
            free(a->firstBlock);
            a->firstBlock = nullptr;
            return nullptr;
        }
        a->firstBlock->chunkEnd = a->firstBlock->chunk;
        a->firstBlock->next = nullptr;
        a->lastBlock = a->firstBlock;
    }
    struct ArenaBlock *block;
    if (blockHasRoom(a->lastBlock, a->options.chunkSize, size))
    {
        block = a->lastBlock;
    }
    else
    {
        block = appendNewBlock(a, a->lastBlock);
        if (block == nullptr)
            return nullptr;
    }
    void *allocated = block->chunkEnd + sizeof(struct Metadata);
    memcpy(a->lastBlock->chunkEnd, &(struct Metadata){size}, sizeof(struct Metadata));
    a->lastBlock->chunkEnd += sizeof(struct Metadata) + size;
    return allocated;
}
