#ifndef _KERNEL_ASSERT_H
#define _KERNEL_ASSERT_H

#include <stdint.h>

void assert_failed (const char *file, uint32_t line, const char *desc);
#define assert(statement) ((statement) ? (void) 0 : assert_failed (__FILE__, __LINE__, #statement))

#endif
