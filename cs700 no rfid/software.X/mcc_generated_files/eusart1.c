
#include "eusart1.h"
#include "../src/driver/my_uart.h"
#include "pin_manager.h"
#include "../src/driver/other-gpio.h"
#include "../src/configs/configs.h"


uint8_t uart1Buffer[UART1_BUFFER_SIZE] @ 0x4a0;
uint16_t uart1ReceivedCount=0;
uint16_t uart1ReceivedTimeout=0;
bool uart1ReceivedTimer=false;

void EUSART1_Initialize(void)
{    
    // disable interrupts before changing states
    PIE1bits.RC1IE = 0;
    PIE1bits.TX1IE = 0;
    // Set the EUSART1 module to the options selected in the user interface.
    // ABDOVF no_overflow; SCKP Non-Inverted; BRG16 16bit_generator; WUE disabled; ABDEN disabled; 
    BAUD1CON = 0x08;
    // SPEN enabled; RX9 9-bit; CREN enabled; ADDEN disabled; SREN disabled; 
    RC1STA = 0x90;
    // TX9 9-bit; TX9D 0; SENDB sync_break_complete; TXEN enabled; SYNC asynchronous; BRGH hi_speed; CSRC slave; 
    TX1STA = 0x24;
    // Baud Rate = 9600; SPBRGL 31; 
    SP1BRGL = 0x1F;
    // Baud Rate = 9600; SPBRGH 1; 
    SP1BRGH = 0x01;
    /*
    // disable interrupts before changing states
    PIE1bits.RC1IE = 0;
    PIE1bits.TX1IE = 0;
    // Set the EUSART1 module to the options selected in the user interface.
    // ABDOVF no_overflow; SCKP Non-Inverted; BRG16 16bit_generator; WUE disabled; ABDEN disabled; 
    BAUD1CON = 0x08;
    // SPEN enabled; RX9 9-bit; CREN enabled; ADDEN disabled; SREN disabled; 
    RC1STA = 0x90;
    // TX9 9-bit; TX9D 0; SENDB sync_break_complete; TXEN enabled; SYNC asynchronous; BRGH hi_speed; CSRC slave; 
    TX1STA = 0x24;
    // Baud Rate = 9600; SPBRGL 31; 
    SP1BRGL = 0x1F;
    // Baud Rate = 9600; SPBRGH 1; 
    SP1BRGH = 0x01;     
     
     */
}

void EUSART1_Transmit_ISR(void)
{
    /*
    // add your EUSART1 interrupt custom code
    if(sizeof(eusart1TxBuffer) > eusart1TxBufferRemaining)
    {
        TX1REG = eusart1TxBuffer[eusart1TxTail++];
        if(sizeof(eusart1TxBuffer) <= eusart1TxTail)
        {
            eusart1TxTail = 0;
        }
        eusart1TxBufferRemaining++;
    }
    else
    {
        PIE1bits.TX1IE = 0;
    }
    */
   // PIE1bits.TX1IE = 0;
}



uint8_t sendCount=0;
void my_uart1_send_poll(uint8_t* buf,uint8_t len)
{
#if cfg_RS485_USED_RS3485==1
    rs485_set_rx();
#else
    rs485_set_tx();
#endif
	for(sendCount=0;sendCount<len;sendCount++){
		do{
			if(PIR1bits.TXIF)break;
		}while(1);
		__nop();
		__nop();
		__nop();
		//TX1REG=*buf;
		//buf++;
		TX1REG=(uint8_t)(buf[sendCount]);
		__nop();
		__nop();
		__nop();
	}
	do{
		if(PIR1bits.TXIF)break;
	}while(1);
    TX1REG=0x00;
    __nop();
    __nop();
    __nop();   
 	do{
		if(PIR1bits.TXIF)break;
	}while(1);   
    //sendCount=30;
    //while(sendCount--);
    //
#if cfg_RS485_USED_RS3485==1    
    rs485_set_tx();
#else
    rs485_set_rx();
#endif
}
void my_uart1_received_ready(void)
{
    
    PIE1bits.RC1IE = 0;
    //rs485_set_rx();
 #if cfg_RS485_USED_RS3485==1    
    rs485_set_tx();
#else
    rs485_set_rx();
#endif   
    uart1ReceivedCount=0;
    //PIR1bits.TXIF=0;
    PIE1bits.RC1IE = 1;
}

void EUSART1_Receive_ISR(void)
{
	uint8_t t8;
    if(1 == RC1STAbits.OERR)
    {
        // EUSART1 error - restart

        RC1STAbits.CREN = 0;
        RC1STAbits.CREN = 1;
        
    }
    else{
        t8=RC1REG;

        if(uart1ReceivedCount<UART1_BUFFER_SIZE)
        {
            uart1Buffer[uart1ReceivedCount]=t8;
            uart1ReceivedCount++;
        }
        uart1ReceivedTimer=true;
        uart1ReceivedTimeout=0;
    }
}

/**
  End of File
*/
