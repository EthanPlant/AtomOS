#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <drivers/video/vga.h>

#include <kernel/tty.h>
#include <kernel/utils.h>
#include <kernel/memory/kheap.h>
#include <kernel/memory/pmm.h>
#include <kernel/memory/vmm.h>
#include <kernel/multiboot.h>

uint32_t *used_frames;
uint32_t nframes;

uint32_t last_allocated_frame;

uint32_t mem_end_page;
extern uint32_t placement_addr;

// Helper macros
#define ARRAY_INDEX(a) (a >> 5)
#define OFFSET_INTO_DWORD(a) (a & 31)

// Set a frame in the used frames bitmap
void pmm_set_frame(uint32_t phys_addr)
{
    // Get the address's index in the bitmap
    uint32_t frame_index = phys_addr / PAGE_SIZE;
    uint32_t index = ARRAY_INDEX(frame_index);
    assert(index <= nframes / 32); // Ensure we're in bounds
    // Set the frame
    uint32_t offset = OFFSET_INTO_DWORD(frame_index);
    assert((used_frames[index] & (1 << offset)) == 0); // Ensure this frame isn't already set
    used_frames[index] |= (1 << offset);
}

// Clear a frame in the used frames bitmap
static void pmm_clear_frame(uint32_t phys_addr)
{
    uint32_t frame_index = phys_addr / PAGE_SIZE;
    uint32_t index = ARRAY_INDEX(frame_index);
    assert(index <= nframes / 32);
    uint32_t offset = OFFSET_INTO_DWORD(frame_index);
    assert((used_frames[index] & (1 << offset)) != 0); // Ensure this frame isn't already set
    used_frames[index] &= ~(1 << offset);
}

// Test if a bit in the bitmap is set
static bool pmm_test_frame(uint32_t phys_addr)
{
    uint32_t frame_index = phys_addr / PAGE_SIZE;
    uint32_t index = ARRAY_INDEX(frame_index);
    assert(index <= nframes / 32);
    uint32_t offset = OFFSET_INTO_DWORD(frame_index);
    return used_frames[index] & (1 << offset);
}

// Return the first free frame at or before a given starting address
static uint32_t pmm_first_free_frame(uint32_t start_addr)
{
    uint32_t index = start_addr / PAGE_SIZE / 32;
    if (index != 0) index  -= 1;

    for (; index < nframes / 32; ++index)
    {
        if (used_frames[index] == 0xFFFFFFFF) continue; // No free frames, try the next index

        // If we're here, at least one bit among these 32 are 0, find the first
        for (uint32_t offset = 0; offset < 32; ++offset)
        {
            if ((used_frames[index] & (1 << offset)) == 0) return (index * 32 + offset) * PAGE_SIZE; // Found it, return the address of this frame
        }
    }

    return 0xFFFFFFFF; // If we make it here, there's no free frames available.    
}

void pmm_init(uint32_t mbd_mmap_addr, uint32_t mbd_mmap_length, uint32_t upper_mem)
{
    terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    terminal_writestring("Initializing physical memory manager...");
    // upper_mem is provided by the bootloader, it's the number of kilobytes starting from 1 MiB
    mem_end_page = 0x100000 + (uint32_t)upper_mem * 1024;

    // Ignore the last few bytes to align if needed
    mem_end_page &= 0xFFFFF000;

    /*
	 * Check the GRUB memory map a first pass, to see if there are higher addresses
	 * than mem_end_page.
	 * This can happen in VMware Fusion, but not in QEMU. With 256 MB RAM, QEMU
	 * gives two regions: a small one at below 1 MB, and then 0x100000 - 0x0FEF0000.
	 * VMWare fusion with 256 MB gives an area below 1 MB, 0x100000 - 0x0FEF0000,
	 * but ALSO 0x0FF00000 to 0x10000000.
	 *
	 * Because mem_end_page = 0x0FEF0000 (as it's based on the **continous** size),
	 * nframes is allocated too small, and we get a buffer overflow (or, rather,
	 * the assertions that prevent that fail, and we get a kernel panic).
	 *
	 * To solve this, we first check how high the largest physical address is, and
	 * then allocate based on that. Finally, we make a second pass through the map
	 * to actually set things up. This pass is only to find out how many frames
	 * there will be.
	 */

    if (mbd_mmap_addr != 0 && mbd_mmap_length != 0)
    {
        for (multiboot_memory_map_t *memmap = (multiboot_memory_map_t*)mbd_mmap_addr; 
             (uint32_t)memmap < mbd_mmap_addr + mbd_mmap_length;
             ++memmap)
        {
            if (memmap->type != 1) continue;
            if (memmap->addr >> 32 != 0) continue;
            
            // Page align addresses
            uint32_t addr_lo = memmap->addr & 0xFFFFFFFF;
            if (addr_lo < PAGE_SIZE) addr_lo = PAGE_SIZE; // Ignore the first page
            if (addr_lo & 0xFFF)
            {
                addr_lo &= 0xFFFFF000;
                addr_lo += PAGE_SIZE;
            }

            uint32_t addr_hi = addr_lo + (memmap->len & 0xFFFFFFFF);
            if (addr_hi & 0xFFF) addr_hi &= 0xFFFFF000;
            if (addr_hi > mem_end_page) mem_end_page = addr_hi;
        }
    }

    // The size of the bitmap is one bit per page
    nframes = mem_end_page / PAGE_SIZE;

    // Allocate and initialize the bitmap
    used_frames = (uint32_t*)kmalloc((nframes / 32 + 1) * sizeof(uint32_t));

    // Set all frames to used, and clear the free areas below
    memset(used_frames, 0xFF, (nframes / 32 + 1) * sizeof(uint32_t));

    last_allocated_frame = 0xFFFFFFFF;

    /*
	 * Utilize the GRUB memory map, if we got one.
	 * All frames are set to used (above), so that reserved areas
	 * are never used.
	 * Then, we loop through the frames in the free areas, and
	 * set them to free, so that they can be allocated by pmm_alloc*.
	 * We only use full pages/frames, so if there's memory available
	 * from 0x500 to 0x9f400, we use the range [0x1000, 0x9f000) and
	 * ignore the rest.
	 */
    if (mbd_mmap_addr != 0 && mbd_mmap_length != 0)
    {
        // We have a memory map from GRUB

        for (multiboot_memory_map_t *memmap = (multiboot_memory_map_t*)mbd_mmap_addr; 
             (uint32_t)memmap < mbd_mmap_addr + mbd_mmap_length;
             ++memmap)
        {
            if (memmap->type != 1) continue; // Any type but 1 means space is reserved

            // Uncomment to print free memory map regions
            // char *str;
            // terminal_writestring("Entry ");
            // itoa(memmap, str, 10);
            // terminal_writestring(str);
            // terminal_writestring(": base ");
            // itoa(memmap->addr, str, 10);
            // terminal_writestring(str);
            // terminal_writestring(" length ");
            // itoa(memmap->len, str, 10);
            // terminal_writestring(str);
            // terminal_writestring(" (free)\n");

            if (memmap->addr >> 32 != 0)
            {
                terminal_setcolor(VGA_COLOR_YELLOW, VGA_COLOR_BLACK);
                terminal_writestring("\nWarning: Ignoring available RAM above 4 GiB\n");
                continue;
            }

            if (memmap->len >> 32 != 0)
            {
                terminal_setcolor(VGA_COLOR_YELLOW, VGA_COLOR_BLACK);
                terminal_writestring("\nWarning: Ignoring part of available RAM (length > 4 GiB)\n");
            }

            // Page align adresses
            uint32_t addr_lo = memmap->addr & 0xFFFFFFFF;
            if (addr_lo < PAGE_SIZE) addr_lo = PAGE_SIZE;
            if (addr_lo & 0xFFF) 
            {
                addr_lo &= 0xFFFFF000;
                addr_lo += PAGE_SIZE;
            }

            uint32_t addr_hi = addr_lo + (memmap->len & 0xFFFFFFFF);

            if (memmap->addr & 0xFFFFFFFF < PAGE_SIZE) addr_hi -= PAGE_SIZE;

            if (addr_hi & 0xFFF) addr_hi &= 0xFFFFF000;

            if (addr_lo >= addr_hi) continue;

            assert(IS_PAGE_ALIGNED(addr_lo));
			assert(IS_PAGE_ALIGNED(addr_hi));
			assert(addr_lo >= memmap->addr >> 32);


            // Clear the addresses in this area
            for (uint32_t addr = addr_lo; addr < addr_hi; addr += PAGE_SIZE)
            {
                assert(addr < (memmap->addr & 0xFFFFFFFF) + (memmap->len & 0xFFFFFFFF));
                pmm_clear_frame(addr);
            }            
        }
    }
    else
    {
        terminal_setcolor(VGA_COLOR_YELLOW, VGA_COLOR_BLACK);
        terminal_writestring("\nWarning: No memory map provided by GRUB, ignoring all RAM below 1 MiB\n");
        for (uint32_t addr = 0x100000; addr < mem_end_page; addr += 0x1000) pmm_clear_frame(addr);
    }

    terminal_setcolor(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
    terminal_writestring("done!\n");
}

// Allocate the next available frame
uint32_t pmm_alloc(void)
{
    // Get the first available frame after the last allocated frame
    if (last_allocated_frame = 0xFFFFFFFF) last_allocated_frame = 0;
    uint32_t phys_addr = pmm_first_free_frame(last_allocated_frame);
    if (phys_addr == 0xFFFFFFFF) panic(__FILE__, "ERROR: No free frames (out of memory)!", __LINE__);

    // Allocate the memory
    last_allocated_frame = phys_addr;
    pmm_set_frame(phys_addr); // This also checks that the frame is available

    // Return the physical memory address of the allocated memory
    return phys_addr;
}

// Allocates a continous block of memory
uint32_t pmm_alloc_continuous(uint32_t num_frames) 
{
    if (num_frames < 2) return pmm_alloc(); // Only need to allocate 1 frame

    bool success = false;
    uint32_t start = pmm_first_free_frame(0); // Find the first available frame in memory
    
    while (!success)
    {
        success = true; // If set when the for loop finishes, we're done
        if (start + (num_frames - 1) * PAGE_SIZE > mem_end_page) panic(__FILE__, "ERROR: No large enough memory region found!", __LINE__);

        // We know start is free, so start looking at start + 1
        for (uint32_t i = 1; i < num_frames; ++i)
        {
            if (pmm_test_frame(start + (i * PAGE_SIZE)) != 0)
            {
                // We found a non-free frame, move on to the next possible block
                start = start + ((i + 1) * PAGE_SIZE);
                success = false;
                break;
            }
        }
    }

    // If we're here, there's a block available starting at start
    for (uint32_t i = 0; i < num_frames; ++i)
        pmm_set_frame(start + i * PAGE_SIZE);

    return start; // Return the starting address of the block
}

// Clear the frame for a certain physical address
void pmm_free(uint32_t phys_addr)
{
    pmm_clear_frame(phys_addr);
    last_allocated_frame = 0xFFFFFFFF;
}

// Return the amount of physical RAM that is still unused
uint32_t pmm_bytes_free(void)
{
    uint32_t unused = 0;

    for (uint32_t i = 0; i < nframes / 32; ++i)
    {
        if (used_frames[i] == 0)
        {
            // All 32 frames in this bitmap chunk are free
            unused += PAGE_SIZE * 32;
            continue;
        }
        else if (used_frames[i] == 0xFFFFFFFF) continue; // No free frames

        // We're somewhere between all empty and all free, let's check each individual frame
        for (uint32_t offset = 0; offset < 32; ++offset) {
            if ((used_frames[i] & (1 << offset)) == 0) unused += 0x1000; // This is a free frame
        }
    }

    assert(unused % 4096 == 0); // Ensure we didn't have any errors
    return unused;
}

// Do the same with used RAM
uint32_t pmm_bytes_used(void)
{
    uint32_t used = 0;

    for (uint32_t i = 0; i < nframes / 32; ++i)
    {
        if (used_frames[i] == 0) continue;
        else if (used_frames[i] == 0xFFFFFFFF) 
        {
            used += 32 * 0x1000;
            continue;
        }

        // We're somewhere between all empty and all free, let's check each individual frame
        for (uint32_t offset = 0; offset < 32; ++offset) {
            if ((used_frames[i] & (1 << offset)) != 0) used += 0x1000; // This is a free frame
        }
    }

    assert(used % 4096 == 0); // Ensure we didn't have any errors
    return used;
}