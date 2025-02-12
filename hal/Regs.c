#include "stdint.h"
#include "Uart.h"
#include "Interrupt.h"

volatile PL011_t* Uart = (PL011_t*)UART_BASE_ADDRESS;
volatile Interrupt_t* Intr = (Interrupt_t*)INTERRUPT_BASE_ADDRESS;