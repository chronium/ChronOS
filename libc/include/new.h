#ifndef _NEW_H
#define _NEW_H 1

#include <stddef.h>
#include <stdlib.h>

inline void *operator new (size_t, void *p) { return p; };
inline void *operator new[] (size_t, void *p) { return p; };
inline void operator delete (void *, void *) { };
inline void operator delete[] (void *, void *) { };

#endif
