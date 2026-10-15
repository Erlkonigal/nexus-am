#include "klib.h"

static unsigned long int next = 1;

int rand(void) {
  // RAND_MAX assumed to be 32767
  next = next * 1103515245 + 12345;
  return (unsigned int)(next/65536) % 32768;
}

void srand(unsigned int seed) {
  next = seed;
}

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)
typedef struct Block {
  size_t size;
  struct Block *next;
} Block;

static uintptr_t bump;
static Block *free_blocks;

void *malloc(size_t size) {
  if (!size) size = 1;
  if (size > SIZE_MAX - sizeof(Block) - 15) return NULL;
  size = (size + 15) & ~(size_t)15;
  Block **slot = &free_blocks;
  while (*slot) {
    if ((*slot)->size >= size) {
      Block *block = *slot;
      *slot = block->next;
      return block + 1;
    }
    slot = &(*slot)->next;
  }
  if (!bump) bump = ((uintptr_t)_heap.start + 15) & ~(uintptr_t)15;
  if (bump > (uintptr_t)_heap.end || sizeof(Block) + size > (uintptr_t)_heap.end - bump) return NULL;
  Block *block = (Block *)bump;
  block->size = size;
  bump += sizeof(Block) + size;
  return block + 1;
}

void free(void *ptr) {
  if (!ptr) return;
  Block *block = (Block *)ptr - 1;
  block->next = free_blocks;
  free_blocks = block;
}

void *calloc(size_t count, size_t size) {
  if (size && count > SIZE_MAX / size) return NULL;
  size_t bytes = count * size;
  void *ptr = malloc(bytes);
  if (ptr) memset(ptr, 0, bytes);
  return ptr;
}

void *realloc(void *ptr, size_t size) {
  if (!ptr) return malloc(size);
  if (!size) { free(ptr); return NULL; }
  Block *block = (Block *)ptr - 1;
  if (size <= block->size) return ptr;
  void *next = malloc(size);
  if (next) { memcpy(next, ptr, block->size); free(ptr); }
  return next;
}
#endif
