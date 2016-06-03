#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <sys/cdefs.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

__attribute__ ((__noreturn__))
void abort (void);

__attribute__ ((__noreturn__))
void exit (int);

void *malloc (size_t);
void free (void *);
void *calloc (size_t, size_t);

#ifdef __cplusplus
}
#endif

#endif
