#include "stdint.h"
#include "Uart.h"
#include "Interrupt.h"

extern volatile PL011_t* Uart;

static void interrupt_handler(void);
extern volatile Interrupt_t* Intr;

void Uart_init()
{
    // Enable UART
    Uart->uartcr.bits.UARTEN = 0;
    Uart->uartcr.bits.TXE = 1;
    Uart->uartcr.bits.RXE = 1;
    Uart->uartcr.bits.UARTEN = 1;

    // Enable input interupt
    Uart->uartimsc.bits.RXIM = 1;

    Interrupt_enable(UART_INTERRUPT);
    Interrupt_register_handler(interrupt_handler, UART_INTERRUPT);

}

void Uart_put_char(uint8_t ch)
{
    while(Uart->uartfr.bits.TXFF);
    Uart->uartdr.all = (ch & 0xFF);
}

uint8_t Uart_get_char(void)
{
    uint32_t data;
    uint32_t i=0;
    while(Uart->uartfr.bits.RXFE)
    {
        i++;
       debug_printf("rxfe: %d %d\n", i, Uart->uartfr.bits.RXFE);
    }  
    debug_printf("1234 rxfe: %d\n", Uart->uartfr.bits.RXFE);
    data = Uart->uartdr.all;

    // Check for an error flag
    if(data & 0xFFFFFF00)
    {
        // Clear the error
        Uart->uartrsr.all = 0xFF;
        return 0;
    }
    return (uint8_t)(data & 0xFF);
}

static void interrupt_handler(void)
{
    uint8_t ch = Uart_get_char();
    Uart_put_char(ch);
}