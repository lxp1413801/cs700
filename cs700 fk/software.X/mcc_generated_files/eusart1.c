/**
  EUSART1 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    eusart1.c

  @Summary
    This is the generated driver implementation file for the EUSART1 driver using MPLAB(c) Code Configurator

  @Description
    This header file provides implementations for driver APIs for EUSART1.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 4.15
        Device            :  PIC16F1947
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.40
*/

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/
#include "eusart1.h"
#include "../src/driver/my_uart.h"
#include "pin_manager.h"
#include "../src/driver/other-gpio.h"


uint8_t uart1Buffer[UART1_BUFFER_SIZE] @ 0x4a0;
uint16_t uart1ReceivedCount=0;
uint16_t uart1ReceivedTimeout=0;
bool uart1ReceivedTimer=false;
/**
  Section: Macro Declarations
*/
//#define EUSART1_TX_BUFFER_SIZE 8
//#define EUSART1_RX_BUFFER_SIZE 8

/**
  Section: Global Variables
*/
/*
static uint8_t eusart1TxHead = 0;
static uint8_t eusart1TxTail = 0;
static uint8_t eusart1TxBuffer[EUSART1_TX_BUFFER_SIZE];
volatile uint8_t eusart1TxBufferRemaining;

static uint8_t eusart1RxHead = 0;
static uint8_t eusart1RxTail = 0;
static uint8_t eusart1RxBuffer[EUSART1_RX_BUFFER_SIZE];
volatile uint8_t eusart1RxCount;
*/
/**
  Section: EUSART1 APIs
*/

void EUSART1_Initialize(void)
{
    // disable interrupts before changing states
    PIE1bits.RC1IE = 0;
    PIE1bits.TX1IE = 0;

    // Set the EUSART1 module to the options selected in the user interface.

    // ABDOVF no_overflow; SCKP Non-Inverted; BRG16 16bit_generator; WUE disabled; ABDEN disabled; 
    BAUD1CON = 0x08;

    // SPEN enabled; RX9 9-bit; CREN enabled; ADDEN disabled; SREN disabled; 
    RC1STA = 0xD0;

    // TX9 9-bit; TX9D 0; SENDB sync_break_complete; TXEN enabled; SYNC asynchronous; BRGH hi_speed; CSRC slave; 
    TX1STA = 0x64;

    // Baud Rate = 9600; SPBRGL 31; 
    SP1BRGL = 0x1F;

    // Baud Rate = 9600; SPBRGH 1; 
    SP1BRGH = 0x01;


    // initializing the driver state
    /*
    eusart1TxHead = 0;
    eusart1TxTail = 0;
    eusart1TxBufferRemaining = sizeof(eusart1TxBuffer);

    eusart1RxHead = 0;
    eusart1RxTail = 0;
    eusart1RxCount = 0;

    // enable receive interrupt
    PIE1bits.RC1IE = 1;
    */
}
/*
uint8_t EUSART1_Read(void)
{
    uint8_t readValue  = 0;
    
    while(0 == eusart1RxCount)
    {
    }

    readValue = eusart1RxBuffer[eusart1RxTail++];
    if(sizeof(eusart1RxBuffer) <= eusart1RxTail)
    {
        eusart1RxTail = 0;
    }
    PIE1bits.RC1IE = 0;
    eusart1RxCount--;
    PIE1bits.RC1IE = 1;

    return readValue;
}

void EUSART1_Write(uint8_t txData)
{
    while(0 == eusart1TxBufferRemaining)
    {
    }

    if(0 == PIE1bits.TX1IE)
    {
        TX1REG = txData;
    }
    else
    {
        PIE1bits.TX1IE = 0;
        eusart1TxBuffer[eusart1TxHead++] = txData;
        if(sizeof(eusart1TxBuffer) <= eusart1TxHead)
        {
            eusart1TxHead = 0;
        }
        eusart1TxBufferRemaining--;
    }
    PIE1bits.TX1IE = 1;
}
*/
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
    PIE1bits.TX1IE = 0;
}
/*
void EUSART1_Receive_ISR(void)
{

    if(1 == RC1STAbits.OERR)
    {
        // EUSART1 error - restart

        RC1STAbits.CREN = 0;
        RC1STAbits.CREN = 1;
    }
    
    // buffer overruns are ignored
    eusart1RxBuffer[eusart1RxHead++] = RC1REG;
    if(sizeof(eusart1RxBuffer) <= eusart1RxHead)
    {
        eusart1RxHead = 0;
    }
    eusart1RxCount++;
}
*/


void my_uart1_send_poll(uint8_t* buf,uint16_t len)
{
	uint16_t i;
    rs485_set_tx();
	if(PIR1bits.TXIF)PIR1bits.TXIF=0;
	for(i=0;i<len;i++)
	{
		//while(TXIF);
		TX1REG=*buf;
		buf++;
		while(!(PIR1bits.TXIF));
		PIR1bits.TXIF=0;
	}
    rs485_set_rx();
}
void my_uart1_received_ready(void)
{
    
    PIE1bits.RC1IE = 0;
    rs485_set_rx();
    uart1ReceivedCount=0;
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
