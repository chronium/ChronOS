#ifndef _STRING_H
#define _STRING_H 1

#include <sys/cdefs.h>

#include <stddef.h>
#include <limits.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned long uintptr_t;

#define ALIGN (sizeof(size_t))
#define ONES ((size_t) -1 / UCHAR_MAX)
#define HIGHS (ONES * (UCHAR_MAX / 2 + 1))
#define HASZERO(X) (((X) - ONES) & ~(X) & HIGHS)

int memcmp (const void *, const void *, size_t);
void *memcpy (void *__restrict, const void *__restrict, size_t);
void *memmove (void *, const void *, size_t);
void *memset (void *, int, size_t);

char *strdup (const char *);
size_t strlen (const char *);
int strcmp (const char *, const char *);

#ifdef __cplusplus
}
#endif

#endif
