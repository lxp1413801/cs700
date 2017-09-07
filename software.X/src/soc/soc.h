#ifndef __SOC_H__
#define __SOC_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <xc.h>
#include "../../mcc_generated_files/mcc.h"
#ifndef NULL 
#define NULL ((void*)0)
#endif
#ifndef null
#define null NULL
#endif

#define __irq_enable() do{ \
    INTERRUPT_GlobalInterruptEnable(); \
    INTERRUPT_PeripheralInterruptEnable(); \
}while(0);

#define __irq_disable() do{ \
    INTERRUPT_PeripheralInterruptDisable(); \
    INTERRUPT_GlobalInterruptDisable(); \
}while(0);
    
#ifdef __cplusplus
    extern "C" {
#endif



#ifdef __cplusplus
    }
#endif
#endif
