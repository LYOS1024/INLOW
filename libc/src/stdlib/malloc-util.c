#include <assert.h>
#include <stdint.h>
#include "malloc.h"

static Chunk emptyBigChunk[2] = {
	{MAGIC_BIG_CHUNK, sizeof(emptyBigChunk), NULL, NULL},
	{MAGIC_END_CHUNK, 0, NULL, NULL}
};
Chunk* __firstBigChunk = emptyBigChunk;

Chunk* __allocateBigChunk(Chunk* lastBigChunk, size_t size)
{
		assert(lastBigChunk->magic == MAGIC_BIG_CHUNK);
	size += 3 * sizeof(Chunk);
	size = alignUp(size, PAGESIZE);

	if (size < 4 * PAGESIZE)
			size = 4 * PAGESIZE;

	Chunk* bigChunk = mapMemory(size);

	if (!bigChunk)
			return NULL;

	Chunk* chunk = bigChunk + 1;
	Chunk* endChunk = (Chunk*) ((uintptr_t)bigChunk + size - sizeof(Chunk));

	bigChunk->magic = MAGIC_BIG_CHUNK;
	bigChunk->size = size;
	bigChunk->prev = lastBigChunk;
	bigChunk->next = NULL;

	lastBigChunk->next = bigChunk;

	chunk->magic = MAGIC_FREE_CHUNK;
	chunk->size = size - 3 * sizeof(Chunk);
	chunk->prev = NULL;
	chunk->next = endChunk;

	endChunk->magic = MAGIC_END_CHUNK;
	endChunk->size = 0;
	endChunk->prev = chunk;
	endChunk->next = NULL;

	return bigChunk;
}

void __splitChunk(Chunk* chunk, size_t size)
{
		assert(chunk->magic == MAGIC_FREE_CHUNK);
	Chunk* newChunk = (Chunk*) ((uintptr_t) chunk + sizeof(Chunk) + size);
	newChunk->magic = MAGIC_FREE_CHUNK;
	newChunk->size = chunk->size - sizeof(Chunk) - size;
	newChunk->prev = chunk;
	newChunk->next = chunk->next;

	chunk->size = size;
	chunk->next->prev = newChunk;
	chunk->next = newChunk;
}

Chunk* __unifyChunks(Chunk* first, Chunk* second)
{
		assert(first->magic == MAGIC_FREE_CHUNK);
		assert(second->magic == MAGIC_FREE_CHUNK);
	first->next = second->next;
	first->size += sizeof(Chunk) + second->size;
	second->next->prev = first;

	return first;
}

#ifdef __is_inlow_libc
void __lockHeap(void)
{
}

void __unlockHeap(void)
{
}
#endif
