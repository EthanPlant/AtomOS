#ifndef _PORTS_H
#define _PORTS_H

#include <stdint.h>

uint8_t inb(uint16_t port);
void out_b(uint16_t port, uint8_t data);

uint16_t inw(uint16_t port);
void outw(uint16_t port, uint16_t data);

#endif