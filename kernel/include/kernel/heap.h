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
#ifdef __cplusplus
extern "C"
#endif
void *kalloc (size_t);
void *malloc_pa (size_t);
void *realloc (void *, size_t);
#ifdef __cplusplus
extern "C"
#endif
void kfree (void *);
size_t ksize (void *);

#endif
