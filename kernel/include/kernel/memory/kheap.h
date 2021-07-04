#ifndef _KHEAP_H
#define _KHEAP_H

#include <stdbool.h>
#include <stdint.h>

#define IS_PAGE_ALIGNED(x) ((((uint32_t)(x)) & 0x00000fff) == 0)

/* Allocate /size/ bytes of memory.
 * If align == true, the return value will be page aligned.
 * If phys isn't NULL, it will be set to the physical address of the allocated area.
 */
void *kmalloc_int(uint32_t size, bool align, uint32_t *phys);

// Plain kmalloc, not page aligned
void *kmalloc(uint32_t size);

// Page aligned kmalloc
void *kmalloc_a(uint32_t size);

// Returns the physical address in phys, but doesn't page align
void *kmalloc_p(uint32_t size, uint32_t *phys);

// Same as kmalloc_p but page aligned
void *kmalloc_ap(uint32_t size, uint32_t *phys);

#endif