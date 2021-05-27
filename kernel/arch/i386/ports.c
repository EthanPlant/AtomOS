#include "ports.h"

// Port I/O support

// Read a byte from a specified port
uint8_t port_byte_in(uint16_t port) 
{
    uint8_t result;
    asm("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

// Write a byte to a port
void port_byte_out(uint16_t port, uint8_t data)
{
    asm volatile("out %%al, %%dx" : : "a" (data), "d" (port));
}

// Read a short from a port
uint16_t port_short_in(uint16_t port)
{
    uint16_t result;
    asm("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

// Write a short to a port
void port_short_out(uint16_t port, uint16_t data)
{
    asm volatile("out %%ax, %%dx" : : "a" (data), "d" (port));
}