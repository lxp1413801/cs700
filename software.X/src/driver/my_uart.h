#ifndef __my_UART_H__
#define __my_UART_H__

//#include "stdint.h"
#ifdef __cplusplus
extern "C" {
#endif
#define UART1_RECEIVED_TIME_OUT 200
#define UART1_BUFFER_SIZE 80
    extern uint8_t uart1Buffer[UART1_BUFFER_SIZE];
    extern uint16_t uart1ReceivedCount;
    extern uint16_t uart1ReceivedTimeout;
    extern bool uart1ReceivedTimer;
    extern void my_uart1_send_poll(uint8_t* buf, uint16_t len);
    //extern void my_uart1_received_disable(void);
    #define my_uart1_received_disable() do{PIE1bits.RC1IE = 0;}while(0);
    extern void my_uart1_received_ready(void);
#ifdef __cplusplus
}
#endif

#endif
