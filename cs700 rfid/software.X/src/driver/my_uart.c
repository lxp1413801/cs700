#include "../soc/soc.h"
#include "my_uart.h"
uint8_t uart1Buffer[UART1_BUFFER_SIZE];
uint16_t uart1ReceivedCount=0x00;
uint16_t uart1ReceivedTimeout=0; 
/*
    // interrupt handler
    if(INTCONbits.TMR0IE == 1 && INTCONbits.TMR0IF == 1)
    {
        TMR0_ISR();
    }
    else if(INTCONbits.PEIE == 1 && PIE1bits.TX1IE == 1 && PIR1bits.TX1IF == 1)
    {
        EUSART1_Transmit_ISR();
    }
    else if(INTCONbits.PEIE == 1 && PIE1bits.RC1IE == 1 && PIR1bits.RC1IF == 1)
    {
        EUSART1_Receive_ISR();
    }
    else if(INTCONbits.PEIE == 1 && PIE1bits.TMR1IE == 1 && PIR1bits.TMR1IF == 1)
    {
        TMR1_ISR();
    }
    else
    {
        //Unhandled Interrupt
    }
 
 
 */
void my_uart1_init(void)
{
    // disable interrupts before changing states
    PIE1bits.RC1IE = 0;
    PIE1bits.TX1IE = 0;

    // Set the EUSART1 module to the options selected in the user interface.

    // ABDOVF no_overflow; SCKP Non-Inverted; BRG16 16bit_generator; WUE disabled; ABDEN disabled; 
    BAUD1CON = 0x08;

    // SPEN enabled; RX9 9-bit; CREN enabled; ADDEN disabled; SREN enabled; 
    RC1STA = 0xF0;

    // TX9 9-bit; TX9D 0; SENDB sync_break_complete; TXEN enabled; SYNC asynchronous; BRGH hi_speed; CSRC slave; 
    TX1STA = 0x64;

    // Baud Rate = 9600; SPBRGL 31; 
    SP1BRGL = 0x1F;
    // Baud Rate = 9600; SPBRGH 1; 
    SP1BRGH = 0x01;
    // initializing the driver state
    // enable receive interrupt
    PIE1bits.RC1IE = 1;
}
void my_uart1_send_poll(uint8_t* buf,uint16_t len)
{
	uint16_t i;
	if(PIR1bits.TXIF)PIR1bits.TXIF=0;
	for(i=0;i<len;i++)
	{
		//while(TXIF);
		TX1REG=*buf;
		buf++;
		while(!(PIR1bits.TXIF));
		PIR1bits.TXIF=0;
	}
}


void my_uart1_received_ready(void)
{
    PIE1bits.RC1IE = 0;
    uart1ReceivedCount=0;
    PIE1bits.RC1IE = 1;
}
/*
void EUSART1_Receive_ISR(void)
{
	uint8_t t8;
    if(1 == RC1STAbits.OERR)
    {
        // EUSART1 error - restart

        RC1STAbits.CREN = 0;
        RC1STAbits.CREN = 1;
    }
	t8=RC1REG;
	if(uart1ReceivedCount<UART1_BUFFER_SIZE)
	{
		uart1Buffer[uart1ReceivedCount]=t8;
		uart1ReceivedCount++;
	}
	uart1ReceivedTimeout=0;
}
 */