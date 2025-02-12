#ifndef INTERRUPT_H_
#define INTERRUPT_H_

typedef union IRQBasicPending_t
{
    uint32_t all;
    struct {
        uint32_t timer:1;          // 0
        uint32_t mailbox:1;
        uint32_t doorbell0:1;
        uint32_t doorbell1:1;
        uint32_t GPUhalt0:1;
        uint32_t GPUhalt1:1;
        uint32_t illegal1:1;
        uint32_t illegal0:1;
        uint32_t oneormore1:1;
        uint32_t oneormore2:1;
        uint32_t GPU7:1;
        uint32_t GPU9:1;
        uint32_t GPU10:1;
        uint32_t GPU18:1;
        uint32_t GPU19:1;
        uint32_t GPU53:1;
        uint32_t GPU54:1;
        uint32_t GPU55:1;
        uint32_t GPU56:1;
        uint32_t GPU57:1;
        uint32_t GPU62:1;
        uint32_t reserved:11;
    } bits;
} IRQBasicPending_t;

typedef union FIQControl_t
{
    uint32_t all;
    struct {
        uint32_t source:7;    // 6:0
        uint32_t enable:1;     // 7
        uint32_t reserved:24;
    } bits;
} FIQControl_t;

typedef union EnableBasicIRQ_t
{
    uint32_t all;
    struct {
        uint32_t timer:1;          // 0
        uint32_t mailbox:1;
        uint32_t doorbell0:1;
        uint32_t doorbell1:1;
        uint32_t GPUhalt0:1;
        uint32_t GPUhalt1:1;
        uint32_t illegal1:1;
        uint32_t illegal0:1;
        uint32_t reserved:24;
    } bits;
} EnableBasicIRQ_t;

typedef union DisableBasicIRQ_t
{
    uint32_t all;
    struct {
        uint32_t timer:1;          // 0
        uint32_t mailbox:1;
        uint32_t doorbell0:1;
        uint32_t doorbell1:1;
        uint32_t GPUhalt0:1;
        uint32_t GPUhalt1:1;
        uint32_t illegal1:1;
        uint32_t illegal0:1;
        uint32_t reserved:24;
    } bits;
} DisableBasicIRQ_t;

typedef struct Interrupt_t
{
    IRQBasicPending_t   irqbasicpending;        //0x200
    uint32_t            irqpending1;            //0x204
    uint32_t            irqpending2;            //0x208
    FIQControl_t        fiqcontrol;             //0x20C
    uint32_t            enableirq1;             //0x210
    uint32_t            enableirq2;             //0x214
    EnableBasicIRQ_t    enablebasicirq;         //0x218
    uint32_t            disableirq1;            //0x21C
    uint32_t            disableirq2;            //0x220
    DisableBasicIRQ_t   disablebasicirq;        //0x224
} Interrupt_t;
 
#define INTERRUPT_BASE_ADDRESS          0x3F00B200   // 인터럽트 컨트롤러 베이스 주소

#define IRQ_START   0
#define IRQ_END     72
#define INTERRUPT_HANDLER_NUM   72

typedef void (*InterHdlr_fptr)(void);

void Interrupt_init(void);
void Interrupt_enable(uint32_t interrupt_num);
void Interrupt_disable(uint32_t interrupt_num);
void Interrupt_register_handler(InterHdlr_fptr hanlder, uint32_t interrupt_num);
void Interrupt_run_handler(void);

#endif /* INTERRUPT_H_ */