#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/memory/kheap.h>
#include <kernel/memory/pmm.h>
#include <kernel/memory/vmm.h>
#include <kernel/utils.h>
#include <kernel/tty.h>

// The kernel's page directory
page_directory_t *kernel_dir = 0;
// Current page directory
page_directory_t *current_dir = 0;

// Internal functions
static void _vmm_invalidate(void*);
static void _vmm_map(uint32_t, uint32_t, page_directory_t*, bool, bool);
static page_t *_vmm_get_page(uint32_t, page_directory_t*);
static void _vmm_create_page_table(uint32_t, page_directory_t*);

// Defined in kheap.c
extern uint32_t placement_addr;

// pmm.c
extern uint32_t *used_frames;

// Map a virtual address to a physical one, in kernel space
void vmm_map_kernel(uint32_t virtual, uint32_t physical, bool write)
{
    _vmm_map(virtual, physical, kernel_dir, true, write);
}

// Unmap a virtual address, does NOT free the associated physical memory
void vmm_unmap(uint32_t virtual, page_directory_t *dir)
{
    page_t *page = _vmm_get_page(virtual, dir);
    *((uint32_t*)page) = 0;

    _vmm_invalidate((void*) virtual);
}

// Unmap a virtual address and free its associated physical memory
void vmm_free(uint32_t virtual, page_directory_t *dir)
{
    pmm_free(vmm_get_phys(virtual, dir));
    vmm_unmap(virtual, dir);
}

// Calculate the physical address for a given virtual one
uint32_t vmm_get_phys(uint32_t virtual, page_directory_t *dir)
{
    page_t *page = _vmm_get_page(virtual, dir);
    uint32_t phys = page->frame * PAGE_SIZE;
    phys += (virtual & 0xFFF);
}

// Set/clear a guard page
static void _vmm_set_guard(uint32_t virtual, page_directory_t *dir, bool guard)
{
    page_t *page = _vmm_get_page(virtual, dir);
    page->present = !guard;
    page->guard = guard;

    _vmm_invalidate((void*)virtual);
}

// Set a guard page
void vmm_set_guard(uint32_t virtual, page_directory_t *dir)
{
    _vmm_set_guard(virtual, dir, true);
}

// Clear a guard page
void vmm_clear_guard(uint32_t virtual, page_directory_t *dir)
{
    _vmm_set_guard(virtual, dir, false);
}

// Map a virtual address to a physical one
static void _vmm_map(uint32_t virtual, uint32_t physical, page_directory_t *dir, bool kernel, bool write)
{
    page_t *page = _vmm_get_page(virtual, dir);

    page->frame = (physical / PAGE_SIZE);
    page->user = !kernel;
    page->rw = write;
    page->present = 1;

    if (IS_KERNEL_SPACE(virtual))
    {
        page->global = 1;
    }

    _vmm_invalidate((void*) virtual);
}

// Get a pointer to a page entry
static page_t *_vmm_get_page(uint32_t virtual, page_directory_t *dir)
{
    uint32_t pt_index = virtual / 4096 / 1024;
    uint32_t pt_offset = (virtual / 4096) % 1024;

    page_table_t *table = dir->tables[pt_index];

    if (table == NULL) _vmm_create_page_table(pt_index, dir);

    return &dir->tables[pt_index]->pages[pt_offset];
}

// Create and set up a page table
static void _vmm_create_page_table(uint32_t pt_index, page_directory_t *dir)
{
    uint32_t phys;
    dir->tables[pt_index] = (page_table_t*)kmalloc_ap(sizeof(page_table_t), &phys);
    memset(dir->tables[pt_index], 0, sizeof(page_table_t));
    dir->tables_physical[pt_index] = phys | PTE_PRESENT | PTE_USER | PTE_RW;
}

static void _enable_paging(void)
{
    asm volatile("mov %%cr0, %%eax;"
        "or $0x80000000, %%eax;"
        "mov %%eax, %%cr0;"
        "mov %%cr4, %%eax;"
        "or $0x00000080, %%eax;"
        "mov %%eax, %%cr4"
        :
        :
        : "%eax", "cc");
}

// Enables the virtual memory manager
void init_mm(uint32_t mbd_mmap_addr, uint32_t mbd_mmap_len, uint32_t upper_mem)
{
    pmm_init(mbd_mmap_addr, mbd_mmap_len, upper_mem);

    // Create kernel page dir
    kernel_dir = (page_directory_t*)kmalloc_a(sizeof(page_directory_t));
    memset(kernel_dir, 0, sizeof(page_directory_t));
    kernel_dir->physical_addr = (uint32_t)kernel_dir->tables_physical;

    // Map the virtual addresses for the kernel
    
    uint32_t addr = 0x00100000;
    while (addr < placement_addr + PAGE_SIZE)
    {
        vmm_map_kernel(addr + 0xC0000000, addr, PAGE_RW); // Higher half stuff as well
        pmm_set_frame(addr);

        addr += PAGE_SIZE;
    }

    // Map a page for the VGA buffer
    // Only need to map 1 page, as the VGA buffer is just under 4 KiB large
    _vmm_map(0xB8000, 0xB8000, kernel_dir, true, PAGE_RW);
    
    // Set address 0 as a guard page to catch NULL pointer dereferences
    _vmm_map(0, 0, kernel_dir, true, PAGE_RO);
    vmm_set_guard(0, kernel_dir);

    used_frames = 0xC0200000; // Virtual address corresponding to our pmm bitmap, need to update this so we don't get page faults

    // Enable paging
    switch_page_directory(kernel_dir);
    _enable_paging();

    placement_addr = 0;
}

// Switch the current page directory
void switch_page_directory(page_directory_t *dir)
{
    current_dir = dir;
    uint32_t new_cr3_contents = (uint32_t) dir->physical_addr;

    asm volatile("mov %0, %%cr3;" : : "r"(new_cr3_contents) : "%eax", "cc");
}

/* Tells the CPU that the page at this (virtual) address has changed. */
static void _vmm_invalidate(void *addr) {
	asm volatile("invlpg (%%eax)" : : "a" (addr) );
}
