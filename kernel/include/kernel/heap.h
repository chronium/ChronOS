#ifndef _KERNEL_HEAP_H
#define _KERNEL_HEAP_H

#include <stdint.h>
#include <stddef.h>

struct mblock {
  void  *memory;
  size_t size;
  void  *next_block;
};

void init_kheap (uint32_t);
void *kalloc (size_t);
void *malloc_pa (size_t);
void *realloc (void *, size_t);
void kfree (void *);
size_t ksize (void *);

#endif
