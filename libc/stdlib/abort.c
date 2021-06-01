#include <stdio.h>
#include <stdlib.h>

__attribute__((__noreturn__))
void abort(void)
{
    #ifdef __is_libk
        // TODO: Add proper kernel panic
        terminal_writestring("Kernel Panic: abort() called\n");
    #else
        // TODO: Properly implement abort()
        terminal_writestring("abort()\n");
    #endif
        while(1) {}
        __builtin_unreachable();
}
