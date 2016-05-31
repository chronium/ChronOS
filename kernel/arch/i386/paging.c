#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <arch/i386/paging.h>
#include <kernel/heap.h>

#define IDENTITY_MAP_END 0x4000000

bool paging_enabled;

struct page_directory *current_directory;
struct page_directory *kernel_directory;

void page_alloc (struct page_directory *dir, uint32_t vaddr, uint32_t paddr, bool write, bool user) {
  vaddr /= 0x1000;
  uint32_t i = vaddr / 1024;

  if (!dir->tables[i]) {
    dir->tables[i] = (struct page_table *) malloc_pa (sizeof (struct page_table));
    for (int x = 0; x < 0x1000; x++)
      ((char *) dir->tables[i])[x] = 0;
    dir->tables_physical[i] = (uint32_t) dir->tables[i] | 0x7;
  }

  struct page *p = &dir->tables[i]->pages[vaddr % 1024];
  if (p->present)
    return;
  p->frame = paddr >> 12;
  p->present = 1;
  p->rw = write;
  p->user = user;
}

void page_remap (struct page_directory *dir, uint32_t vaddr, uint32_t paddr) {
  vaddr /= 0x1000;
  uint32_t i = vaddr / 1024;
  void *old_t = dir->tables[i];
  dir->tables[i] = (struct page_table *) malloc_pa (sizeof (struct page_table));
  memcpy (dir->tables[i], old_t, sizeof (struct page_table));
  dir->tables_physical[i] = (uint32_t) dir->tables[i] | 0x7;
  struct page *p = &dir->tables[i]->pages[vaddr % 1024];
  p->frame = paddr >> 12;
}

void page_free (struct page_directory *dir, uint32_t vaddr) {
  vaddr /= 0x1000;
  uint32_t i = vaddr / 1024;
  memset (&dir->tables[i]->pages[vaddr % 1024], 0, sizeof (struct page));
}

void *get_phyiscal_addr (struct page_directory *pdir, void *vaddr) {
  uint32_t i = (uint32_t) vaddr / 0x1000 / 1024;

  struct page_table *tbl = pdir->tables[i];

  if (tbl) {
    struct page *p = &pdir->tables[i]->pages[((uint32_t) vaddr / 0x1000) % 1024];
    return (void *) ((p->frame << 12) | ((uint32_t) vaddr & 0xFFF));
  }

  return NULL;
}

void init_paging () {
  kernel_directory = (struct page_directory *) malloc_pa (sizeof (struct page_directory));
  memset (kernel_directory, 0, sizeof (struct page_directory));

  switch_page_directory (kernel_directory);
  for (uint32_t ptr = 0; ptr < IDENTITY_MAP_END; ptr += 0x1000)
    page_alloc (kernel_directory, ptr, ptr, 0, 0);

  enable_paging ();
}
