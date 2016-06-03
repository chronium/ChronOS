#include <stdlib.h>

#if defined (__is_chronos_kernel)
#include <kernel/heap.h>
#endif

void free (void *addr) {
#if defined (__is_chronos_kernel)
  kfree (addr);
#endif
}
