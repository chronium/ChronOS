#include <stddef.h>
#include <stdlib.h>

__attribute__ ((weak))
void *operator new (size_t size) {
  return malloc (size);
}

__attribute__ ((weak))
void *operator new[] (size_t size) {
  return malloc (size);
}

__attribute__ ((weak))
void operator delete (void* addr) {
  free (addr);
}

__attribute__ ((weak))
void operator delete[] (void* addr) {
  free (addr);
}
