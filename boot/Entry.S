#include "ARMv7AR.h"
#include "MemoryMap.h"

.text
    .code 32

    .global vector_start
    .global vector_end

    vector_start:
        LDR PC, reset_handler_addr
        LDR PC, undef_handler_addr
        LDR PC, svc_handler_addr
        LDR PC, pftch_abt_handler_addr
        LDR PC, data_abt_handler_addr
        b   .
        LDR PC, IRQ_handler_addr
        LDR PC, FIQ_handler_addr

        reset_handler_addr:     .word reset_handler
        undef_handler_addr:     .word dummy_handler
        svc_handler_addr:       .word dummy_handler
        pftch_abt_handler_addr: .word dummy_handler
        data_abt_handler_addr:  .word dummy_handler
        IRQ_handler_addr:       .word IRQ_handler
        FIQ_handler_addr:       .word FIQ_handler

    vector_end:

    reset_handler:
        // 현재 코어 ID 확인
        MRC p15, 0, r0, c0, c0, 5  // Read MPIDR into r0
        AND r0, r0, #0x3           // Mask Core ID (MPIDR[1:0])

        // 코어 0이 아니면 WFE 상태로 진입
        CMP r0, #0                 // Check if Core ID == 0
        BNE other_core_halt        // If not, branch to halt routine

        MRS r0, cpsr
        BIC r1, r0, #0x1F
        ORR r1, r1, #ARM_MODE_BIT_SVC
        MSR cpsr, r1
        LDR sp, =SVC_STACK_TOP

        MRS r0, cpsr
        BIC r1, r0, #0x1F
        ORR r1, r1, #ARM_MODE_BIT_IRQ
        MSR cpsr, r1
        LDR sp, =IRQ_STACK_TOP

        MRS r0, cpsr
        BIC r1, r0, #0x1F
        ORR r1, r1, #ARM_MODE_BIT_FIQ
        MSR cpsr, r1
        LDR sp, =FIQ_STACK_TOP

        MRS r0, cpsr
        BIC r1, r0, #0x1F
        ORR r1, r1, #ARM_MODE_BIT_ABT
        MSR cpsr, r1
        LDR sp, =ABT_STACK_TOP

        MRS r0, cpsr
        BIC r1, r0, #0x1F
        ORR r1, r1, #ARM_MODE_BIT_UND
        MSR cpsr, r1
        LDR sp, =UND_STACK_TOP

        MRS r0, cpsr
        BIC r1, r0, #0x1F
        ORR r1, r1, #ARM_MODE_BIT_SYS
        MSR cpsr, r1
        LDR sp, =USRSYS_STACK_TOP

        BL  main
        B .

    dummy_handler:
        b   .
    
other_core_halt:

halt_loop:
    WFE                         // Wait for Event (Low-power state)
    B halt_loop                 // 무한 대기

.end
