#include "stdint.h"
#include "memio.h"
#include "Interrupt.h"
#include "armcpu.h"

extern volatile Interrupt_t* Intr;
static InterHdlr_fptr sHandlers[INTERRUPT_HANDLER_NUM];

void Interrupt_init(void)
{
    // // 모든 인터럽트 비활성화
    // Intr->disableirq1 = 0xFFFFFFFF;
    // Intr->disableirq2 = 0xFFFFFFFF;
    // Intr->disablebasicirq.all = 0xFFFFFFFF;

    // 핸들러 테이블 초기화
    for(uint32_t i = 0; i < INTERRUPT_HANDLER_NUM; i++)
    {
        sHandlers[i] = NULL;
    } 

    // FIQ 비활성화
    Intr->fiqcontrol.all = 0;

    // // 모든 인터럽트 비활성화 취소
    // Intr->disableirq1 = 0;
    // Intr->disableirq2 = 0;
    // Intr->disablebasicirq.all = 0;

    // IRQ 인터럽트 활성화 (cpsie i 등 실행)
    enable_irq();
}

void Interrupt_enable(uint32_t interrupt_num)
{
    if (interrupt_num < IRQ_START || interrupt_num > IRQ_END)
    {
        return;
    }
    uint32_t bit_num = interrupt_num;

    if (interrupt_num < 32)
    {
        CLR_BIT(Intr->disableirq1, bit_num);
        SET_BIT(Intr->enableirq1, bit_num);
    }
    else if(interrupt_num < 64)
    {
        bit_num = interrupt_num - 32;
        CLR_BIT(Intr->disableirq2, bit_num);
        SET_BIT(Intr->enableirq2, bit_num);
    }
    else
    {
        bit_num = interrupt_num - 64;
        CLR_BIT(Intr->disablebasicirq.all, bit_num);
        SET_BIT(Intr->enablebasicirq.all, bit_num);
    }
}

void Interrupt_disable(uint32_t interrupt_num)
{
    {
        if (interrupt_num < IRQ_START || interrupt_num > IRQ_END)
        {
            return;
        }
    
        if (interrupt_num < 32)
        {
            CLR_BIT(Intr->enableirq1, interrupt_num);
            SET_BIT(Intr->disableirq1, interrupt_num);
        }
        else if (interrupt_num < 64)
        {
            CLR_BIT(Intr->enableirq2, interrupt_num - 32);
            SET_BIT(Intr->disableirq2, interrupt_num - 32);
        }
        else
        {
            CLR_BIT(Intr->enablebasicirq.all, interrupt_num - 64);
            SET_BIT(Intr->disablebasicirq.all, interrupt_num - 64);
        }
    }
}

void Interrupt_register_handler(InterHdlr_fptr handler, uint32_t interrupt_num)
{
    sHandlers[interrupt_num] = handler;
}


void Interrupt_run_handler(void)
{
    uint32_t interrupt_num;
    uint32_t pending_basic = Intr->irqbasicpending.all;
    uint32_t pending_1 = Intr->irqpending1;
    uint32_t pending_2 = Intr->irqpending2;

    debug_printf("run handler: %d %d %d\n",pending_basic, pending_1, pending_2);

    // 1. Pending된 인터럽트 확인 (Basic IRQ)
    if (Intr->irqbasicpending.all)
    {
        interrupt_num = __builtin_ctz(Intr->irqbasicpending.all) + 64;
    }
    // 2. Pending된 인터럽트 확인 (IRQ 32~63)
    else if (Intr->irqpending2)
    {
        interrupt_num = __builtin_ctz(Intr->irqpending2) + 32;
    }
    // 3. Pending된 인터럽트 확인 (IRQ 0~31)
    else if (Intr->irqpending1)
    {
        interrupt_num = __builtin_ctz(Intr->irqpending1);
    }
    else
    {
        return;  // 처리할 인터럽트가 없음
    }



    debug_printf("%d\n",interrupt_num);

    // 핸들러 실행
    if (sHandlers[interrupt_num] != NULL) {
        sHandlers[interrupt_num]();
    }

}
