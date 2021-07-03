#ifndef _PMM_H
#define _PMM_H

#include <stdint.h>

void pmm_init(uint32_t mbd_mmap_addr, uint32_t mbd_mmap_length, uint32_t upper_mem);
uint32_t pmm_alloc(void);
uint32_t pmm_alloc_continuous(uint32_t num_frames);
void pmm_free(uint32_t phys_addr);
uint32_t pmm_bytes_free(void);

#endif