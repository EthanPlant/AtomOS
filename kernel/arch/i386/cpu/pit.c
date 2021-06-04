#include <stdint.h>

#include <kernel/timer.h>
#include <kernel/tty.h>
#include <kernel/utils.h>
#include <drivers/ports.h>
#include <isr.h>

uint32_t tick = 0;

static void timer_callback(registers_t *regs)
{
    ++tick;
    // Don't need to spam the console with timer info
    // char tick_ascii[256];
    // terminal_writestring("Tick: ");
    // itoa(tick, tick_ascii, 10);
    // terminal_writestring(tick_ascii);
    // terminal_writestring("\n");
}

void init_timer(uint32_t freq)
{
    // Register the timer callback
    register_interrupt_handler(IRQ0, timer_callback);

    // Calcualate the divisor and send it to the PIT
    uint32_t div = 1193180 / freq;
    outb(0x43, 0x36);
    uint8_t l = (uint8_t)(div & 0xFF);
    uint8_t h = (uint8_t)((div >> 8) & 0xFF);
    outb(0x40, l);
    outb(0x40, h);
}