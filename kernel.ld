ENTRY(_start)
SECTIONS
{
    . = 0x10000;
    start_addr = .; _start_addr = .; __start_addr = .;
    .text : { *(.text) }
    .data : { *(.data) }
    .bss : { *(.bss) }
    end_addr = .; _end_addr = .; __end_addr = .;
}