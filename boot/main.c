#include "stdint.h"
#include "Uart.h"

static void HW_init(void);

void main()
{
    HW_init();

    Uart_put_char('5');

    uint32_t i = 100;
    while(i--)
    {
        debug_printf("%u",i);
        Uart_put_char('N');
       
    }
    Uart_put_char('\n');
}

static void HW_init(void)
{
    Uart_init();
}