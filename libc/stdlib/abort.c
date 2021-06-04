#include <stdio.h>
#include <stdlib.h>

#include <kernel/utils.h>

__attribute__((__noreturn__))
void abort(void)
{
    #ifdef __is_libk
        panic(__FILE__, "KERNEL ERROR: abort() called from kernel", __LINE__);
    #else
        // TODO: Properly implement abort()
        terminal_writestring("abort()\n");
    #endif
        while(1) {}
        __builtin_unreachable();
}
