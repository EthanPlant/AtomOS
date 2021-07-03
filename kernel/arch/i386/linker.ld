ENTRY (_start)

SECTIONS
{
    . = 0x00100000;
	.text ALIGN (4K) : AT (ADDR (.text))
	{
		*(.multiboot*)
		*(.text*)
	}
	.rodata ALIGN (4K) : AT (ADDR (.rodata))
	{
		*(.rodata*)
	}
	.data ALIGN (4K) : AT (ADDR (.data))
	{
		*(.data)
	}
	.bss ALIGN (4K) : AT (ADDR (.bss))
	{
		*(COMMON)
		*(.bss)
	}
}