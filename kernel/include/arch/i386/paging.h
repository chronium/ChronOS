#ifndef _ARCH_I386_PAGING_H
#define _ARCH_I386_PAGING_H

#include <stdint.h>
#include <stdbool.h>

struct page {
  uint32_t present  : 1;
  uint32_t rw       : 1;
  uint32_t user     : 1;
  uint32_t accessed : 1;
  uint32_t dirty    : 1;
  uint32_t unused   : 7;
  uint32_t frame    : 20;
} __attribute__ ((packed));

struct page_table {
  struct page pages[1024];
};

struct page_directory {
  struct page_table  *tables[1024];
  uint32_t            tables_physical[1024];
  uint32_t            physical_addr;
};

extern struct page_directory *kernel_directory;

void init_paging ();

void page_alloc (struct page_directory *, uint32_t, uint32_t, bool, bool);
void page_remap (struct page_directory *, uint32_t, uint32_t);
void page_free  (struct page_directory *, uint32_t);

void *get_physical_addr (struct page_directory *, void *);

struct page_directory *create_new_page ();

void copy_page_physical (uint32_t, uint32_t);

static inline void switch_page_directory (struct page_directory *dir) {
  extern struct page_directory *current_directory;
  current_directory = dir;

  asm volatile ("mov %0, %%cr3" : : "r" (&dir->tables_physical));
  asm volatile ("mov %cr3, %eax; mov %eax, %cr3;");
}

static inline void disable_paging () {
  uint32_t cr0;
  asm volatile ("mov %%cr0, %0" : "=r" (cr0));
  cr0 &= 0x7FFFFFFF;
  asm volatile ("mov %0, %%cr0" : : "r" (cr0));
}

static inline void enable_paging () {
  uint32_t cr0;
  asm volatile ("mov %%cr0, %0" : "=r" (cr0));
  cr0 |= 0x80000000;
  asm volatile ("mov %0, %%cr0" : : "r" (cr0));
}

static inline void invlpg (void *m) {
  asm volatile ("invlpg (%0)" : : "b" (m) : "memory");
}

#endif
