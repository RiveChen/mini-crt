#include <minicrt.h>

// AArch64 Linux brk syscall number = 214
// param: x0 = new brk address (if 0, return current brk address)
// return: x0 = new brk address (if failed, usually return current value or
// error code)
static void *sys_brk(void *addr) {
  register void *x0 __asm__("x0") = addr;
  register long x8 __asm__("x8") = 214;

  __asm__ volatile("svc #0" : "+r"(x0) : "r"(x8) : "memory");
  return x0;
}

/* heap structure:
 * +------+ -+
 * | type |  |
 * +------+  |
 * | size |  |
 * +------+  +- header
 * | next |  |
 * +------+  |
 * | prev |  |
 * +------+ -+
 * | data | <- ptr passed to user
 * +------+
 * | ...  |
 * +------+
 */

typedef struct _heap_header {
  enum {
    HEAP_BLOCK_FREE = 0xAE86AE86,
    HEAP_BLOCK_USED = 0xDEADBEEF,
  } type;

  unsigned size; // size of the block, including the header
  struct _heap_header *next;
  struct _heap_header *prev;
} heap_header;

#define ADDR_ARR(a, o) (((char *)a) + o)
#define HEADER_SIZE (sizeof(heap_header))

static heap_header *list_head = NULL;

void free(void *ptr) {
  // retrieve the header from the pointer
  heap_header *header = (heap_header *)ADDR_ARR(ptr, -HEADER_SIZE);
  if (header->type != HEAP_BLOCK_USED) {
    return;
  }

  // mark the block as free
  header->type = HEAP_BLOCK_FREE;

  // try to merge with previous block
  if (header->prev != NULL && header->prev->type == HEAP_BLOCK_FREE) {
    header->prev->next = header->next;
    if (header->next != NULL) {
      header->next->prev = header->prev;
    }
    header->prev->size += header->size;

    header = header->prev;
  }

  // try to merge with next block
  if (header->next != NULL && header->next->type == HEAP_BLOCK_FREE) {
    header->size += header->next->size;
    header->next = header->next->next;
  }
}

void *malloc(unsigned size) {

  if (size == 0) {
    return NULL;
  }

  heap_header *header = list_head;
  while (header != NULL) {
    // linear search for a free block
    if (header->type == HEAP_BLOCK_USED) {
      continue;
    }

    if (header->size >= size + HEADER_SIZE &&
        header->size <= size + HEADER_SIZE * 2) {
      header->type = HEAP_BLOCK_USED;
      return ADDR_ARR(header, HEADER_SIZE);
    }

    if (header->size > size + HEADER_SIZE * 2) {
      // split the block
      heap_header *new_header =
          (heap_header *)ADDR_ARR(header, size + HEADER_SIZE);
      new_header->type = HEAP_BLOCK_FREE;
      new_header->size = header->size - (size - HEADER_SIZE);
      new_header->next = header->next;
      new_header->prev = header;
      header->type = HEAP_BLOCK_USED;
      header->next = new_header;
      header->size = size + HEADER_SIZE;
      return ADDR_ARR(header, HEADER_SIZE);
    }
    header = header->next;
  }

  return NULL;
}

int crt_heap_init(void) {
  void *base = NULL;
  heap_header *header = NULL;
  unsigned heap_size = 1024 * 1024 * 32; // 32MB

  base = sys_brk(0);
  void *end = ADDR_ARR(base, heap_size);
  end = sys_brk(end);
  if (end == base) {
    return 0;
  }

  header = (heap_header *)base;
  header->type = HEAP_BLOCK_FREE;
  header->size = heap_size;
  header->next = NULL;
  header->prev = NULL;

  list_head = header;
  return 1;
}