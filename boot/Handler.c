#include "stdbool.h"
#include "stdint.h"
#include "Interrupt.h"

__attribute__ ((interrupt ("IRQ"))) void IRQ_handler(void)
{
    Interrupt_run_handler();
}

__attribute__ ((interrupt ("FIQ"))) void FIQ_handler(void)
{
    while(true);
}