#include <string.h>
#include <stdint.h>
#include <stddef.h>

#include <kernel/panic.h>
#include <kernel/heap.h>

#define MAGIC   0x4E69636F
#define GUARD_1 0x42696961
#define GUARD_2 0x44656561

struct mblock *free_top;
struct mblock *used_top;

void *free_address;

static void *kheap_alloc (size_t);
static struct mblock *kheap_get_block (size_t);

void init_kheap (uint32_t addr) {
  free_address = (void *) (((uint32_t) addr % 8) + addr);
  free_top = NULL;
  used_top = NULL;
}

void *kalloc (size_t size) {
  struct mblock *new_mb = kheap_get_block (size);

  if (!new_mb) {
    new_mb = (struct mblock *) kheap_alloc (sizeof (struct mblock));
    new_mb->memory = kheap_alloc (size);
    new_mb->size = size;
  }

  new_mb->next_block = used_top;
  used_top = new_mb;

  memset (new_mb->memory, 0, size);

  return new_mb->memory;
}

static void *kheap_alloc (size_t size) {
  size_t sz_aligned = size + (size % 8) + 8;

  *((int *) free_address) = GUARD_1;
  *((int *) (free_address + size + 4)) = GUARD_2;

  free_address += sz_aligned;
  return 4 + free_address - sz_aligned;
}

static struct mblock *kheap_get_block (size_t size) {
  struct mblock *i = free_top;
  struct mblock *p = i;

  while (i) {
    if (i->size > size) {
      if (p == i)
        free_top = (struct mblock *) i->next_block;
      else
        p->next_block = i->next_block;
      return i;
    }
    p = i;
    i = (struct mblock *) i->next_block;
  }

  return NULL;
}

void *realloc (void *ptr, size_t size) {
  void *tmp = ptr;
  size_t sz = ksize (tmp);

  if (sz != 0) {
    kfree (tmp);
    void *rtn = kalloc (size);
    memcpy (rtn, tmp, sz);
    return rtn;
  }
  return NULL;
}

void *malloc_pa (size_t size) {
  free_address = (void *)(((uint32_t) free_address & 0xFFFFF000) + 0x1000);

  void *ret = free_address;
  free_address += size;
  return ret;
}

void kfree (void *ptr) {
  struct mblock *i = used_top;
  struct mblock *p = i;

  while (i) {
    if (i->memory == ptr) {
      if (*((int *) (i->memory - 4)) != GUARD_1 || *((int *) (i->memory + i->size)) != GUARD_2) {
        panic ();
      }
      if (p == i)
        used_top = (struct mblock *) i->next_block;
      else
        p->next_block = i->next_block;
      i->next_block = free_top;
      free_top = i;
    }
    i = i;
    i = (struct mblock *) i->next_block;
  }
}

size_t ksize (void *ptr) {
  struct mblock *i = used_top;

  while (i) {
      if (i->memory == ptr)
        return i->size;
      i = (struct mblock *) i->next_block;
  }

  return 0;
}
