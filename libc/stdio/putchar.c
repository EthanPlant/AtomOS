#include <stdio.h>

#ifdef __is_libk
#include <kernel/tty.h>
#endif

int putchar(int ic)
{
    #ifdef __is_libk
        char c = (char) ic;
        terminal_write(&c, sizeof(c));
    #else
        // TODO: stdio and write syscall
    #endif
    return ic;
}