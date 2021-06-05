#include <stdint.h>

#include <cpu/isr.h>
#include <drivers/ports.h>
#include <drivers/video/vga.h>
#include <kernel/timer.h>
#include <kernel/tty.h>
#include <kernel/utils.h>

#define PIT_CHANNEL_0 0x40
#define PIT_COMMAND 0x43


uint32_t tick = 0;

static void timer_callback(registers_t *regs)
{
    ++tick;
}

void init_timer(uint32_t freq)
{
    terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    terminal_writestring("Initializing timer... ");

    // Register the timer callback
    register_interrupt_handler(IRQ0, timer_callback);

    // Calcualate the divisor and send it to the PIT
    uint32_t div = 1193180 / freq;
    outb(PIT_COMMAND, 0x36);
    uint8_t l = (uint8_t)(div & 0xFF);
    uint8_t h = (uint8_t)((div >> 8) & 0xFF);
    outb(PIT_CHANNEL_0, l);
    outb(PIT_CHANNEL_0, h);

    terminal_setcolor(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
    terminal_writestring("done\n");
}

uint32_t get_ticks(void)
{
    return tick;
}