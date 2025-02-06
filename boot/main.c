#include "stdint.h"
#include "Uart.h"

static void HW_init(void);

void main()
{
    HW_init();

    int32_t i = 100;
    int32_t a, b, d;
    char c;
    while (i--)
    {
        debug_printf("%d %u ", -i, i);
        Uart_put_char('N');
    }
    Uart_put_char('\n');
    putstr("Hello world!\n");

    // i = 100;
    // while(i--)
    // {
    //     uint8_t ch = Uart_get_char();
    //     Uart_put_char(ch);
    // }
    debug_scanf("%d%d%d%c", &a, &b, &d, &c);
    debug_printf("a: %d b: %d d: %d c: %c c ascii: %d\n", a, b, d, c, c);
}

static void HW_init(void)
{
    Uart_init();
}