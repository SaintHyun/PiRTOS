#include "stdint.h"
#include "Uart.h"
#include "Interrupt.h"

static void HW_init(void);
extern volatile Interrupt_t* Intr;

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
    // c=Uart_get_char();
    // Uart_put_char(c);
    putstr("Hello world!\n");

    // i = 100;
    // while(i--)
    // {
    //     uint8_t ch = Uart_get_char();
    //     Uart_put_char(ch);
    // }
    // debug_scanf("%d%d%d%c", &a, &b, &d, &c);
    // debug_printf("a: %d b: %d d: %d c: %c c ascii: %d\n", a, b, d, c, c);

    uint32_t cpsr;
    __asm__ volatile("mrs %0, cpsr" : "=r"(cpsr));
    debug_printf("CPSR: %x\n", cpsr);


    while (1)
    {
    }
}

static void HW_init(void)
{
    Interrupt_init();
    Uart_init();
}