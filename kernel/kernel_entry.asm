; Ensure we always call the kernel entrypoint
[bits 32]
[extern kernel_main]
call kernel_main
jmp $