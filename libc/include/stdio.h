#ifndef _STDIO_H
#define _STDIO_H 1

#include <stdarg.h>

#include <sys/cdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

int printf (const char *__restrict, ...);
int putchar (int);
int puts (const char *);
int sprintf (char *, const char *__restrict, ...);
int vsprintf (char *, const char *__restrict, va_list);

#ifdef __cplusplus
}
#endif

#endif
