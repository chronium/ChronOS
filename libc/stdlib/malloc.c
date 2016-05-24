#include <stdlib.h>

#if defined (__is_chronos_kernel)
#include <kernel/heap.h>
#endif

void *malloc (size_t size) {
#if defined (__is_chronos_kernel)
  return kalloc (size);
#endif
  return NULL;
}
