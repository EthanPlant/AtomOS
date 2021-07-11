#ifndef _VMM_H
#define _VMM_H

#include <stdbool.h>
#include <stdint.h>

#define PAGE_SIZE 0x1000

// Represents a page in memory
typedef struct page
{
    uint32_t present : 1; // Is page present in memory?
    uint32_t rw : 1; // 0 if read-only, 1 if read-write
    uint32_t user : 1; // 0 if kernel-mode, 1 if user-mode
    uint32_t pwt : 1; // Page level write-through
    uint32_t pcd : 1; // Page level cache disable
    uint32_t accessed : 1; // Had the page been accessed since last refresh?
    uint32_t dirty : 1; // Had the page been written through since last refresh?
    uint32_t pat : 1; // Not used: related to caching on newer processors
    uint32_t global : 1; // Determines whether translation is global
    uint32_t guard : 1; // One of three "avail" bits, is this page a guard page?
    uint32_t avail : 2; // Bits available for use by the OS
    uint32_t frame : 20; // Upper 20 bits of the frame address
} __attribute__((packed)) page_t;

// Page Table Entry bits, i.e. ones used to control a single page
#define PTE_PRESENT (1 << 0)
#define PTE_RW (1 << 1)
#define PTE_USER (1 << 2)
#define PTE_PWT (1 << 3)
#define PTE_PCD (1 << 4)
#define PTE_ACCESSED (1 << 5)
#define PTE_DIRTY (1 << 6)
#define PTE_PAT (1 << 7)
#define PTE_GLOBAL (1 << 8)
#define PTE_GUARD (1 << 9)

// Represents a page table in memory
typedef struct page_table
{
    page_t pages[1024];
} page_table_t;

// Represents a page directory in memory
typedef struct page_directory
{
    // Array of pointers to page tables
    page_table_t *tables[1024];

    // Array of pointers to page tables physical addresses
    uint32_t tables_physical[1024];

    // Physical address of tables_physical
    uint32_t physical_addr;
} page_directory_t;

// Defines that can be used as parameters to the below functions
#define PAGE_RO 0
#define PAGE_RW 1
#define PAGE_ANY_PHYS 0
#define PAGE_CONTINUOUS_PHYS 1

#define IS_USER_SPACE(addr) ( (((uint32_t)addr) >= 0x10000000 && ((uint32_t)addr) < 0xc0000000) )
#define IS_KERNEL_SPACE(addr) ( !IS_USER_SPACE(addr) )

void vmm_map_kernel(uint32_t, uint32_t, bool);

void vmm_unmap(uint32_t, page_directory_t*);
void vmm_free(uint32_t, page_directory_t*);

uint32_t vmm_get_phys(uint32_t, page_directory_t*);

void vmm_set_guard(uint32_t, page_directory_t*);
void vmm_clear_guard(uint32_t, page_directory_t*);

void init_mm(uint32_t, uint32_t, uint32_t);

void switch_page_directory(page_directory_t*);

#endif