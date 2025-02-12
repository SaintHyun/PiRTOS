ENTRY(vector_start)
SECTIONS
{
    . = 0x80000;


    .text :
    {
        *(vector_start)
        *(.text .rodata)
    }
    .data :
    {
        *(.data)
    }
    .bss :
    {
        *(.bss)
    }
}