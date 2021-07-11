#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <kernel/memory/kheap.h>

uint32_t placement_addr = 0x00200000; // Should be determined at link time, for now 2 MiB should be safe to use

// TODO proper kernel heap
void *kmalloc_int(uint32_t size, bool align, uint32_t *phys)
{
    if (align == 1 && (!IS_PAGE_ALIGNED(placement_addr))) // If the address is not already page-aligned
    {
        placement_addr &= 0xFFFFF000;
        placement_addr += 0x1000;
    }

    if (phys) *phys = placement_addr;

    uint32_t tmp = placement_addr;
    placement_addr += size;
    return (void*) tmp;
}

void *kmalloc(uint32_t size)
{
    return kmalloc_int(size, false, NULL);
}

void *kmalloc_a(uint32_t size)
{
    return kmalloc_int(size, true, NULL);
}

void *kmalloc_p(uint32_t size, uint32_t *phys)
{
    return kmalloc_int(size, false, phys);
}

void *kmalloc_ap(uint32_t size, uint32_t *phys)
{
    return kmalloc_int(size, true, phys);
}

