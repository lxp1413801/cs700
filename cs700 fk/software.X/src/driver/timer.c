#include "../soc/soc.h"
#include "driver.h"
#include "../app/app.h"
#include "./other-gpio.h"
#include "../configs/configs.h"
#include"../api/ui_key.h"
//st_userTicker userTicker[1];
//st_userTicker userTicker;
uint16_t globlTickerms=0ul;
uint16_t findCardTimer=0x00;
uint16_t systemTikerms=0x00;
uint8_t systemTickerSec=0ul;

void set_globle_ticker(uint16_t tk)
{
	__irq_disable();
	globlTickerms=tk;
	__irq_enable();
}
uint16_t get_globle_ticker(void)
{
	uint16_t ret;
	__irq_disable();
	ret=globlTickerms;
	__irq_enable();
	return ret;
}
uint16_t tmTemp16;
void timer0_irq_callback(void){
    //uint16_t t16; 
    Event &= ~ flgEventMenuTimer0Irq;
    tmTemp16=systemTikerms;
    systemTikerms=0;
    
    //globlTickerms+=t16;
    findCardTimer+=tmTemp16;
    /*
    if(userTicker.userTickerStatus==TICK_STATUS_INC)
    {
        userTicker.Tick+=tmTemp16;
        if(userTicker.Tick>=userTicker.Target)
        {
            userTicker.userTickerStatus=TICK_STATUS_STOP;
        }
    }
     */
    if(findCardTimer>cfg_FIND_CARD_INTERVAL){
        Event |= flgEventReadCardTimerUp;
        findCardTimer=0;
    }
    if(uart1ReceivedTimer){
        uart1ReceivedTimeout+=tmTemp16;
        if(uart1ReceivedTimeout>=UART1_RECEIVED_TIME_OUT)
        {
            uart1ReceivedTimer=false;
            Event |= flgEventUart1Received;
        }
    }
	adcSamplingTimer+=tmTemp16;
	if(adcSamplingTimer>=ADC_TS){
		Event |= flgEvenAdcSamplling;
	}
} 
/*
 void timer0_irq_callback(void){
    uint16_t t16; 
    Event &= ~ flgEventMenuTimer0Irq;
    t16=systemTikerms;
    systemTikerms=0;
    
    findCardTimer+=t16;
    if(userTicker.userTickerStatus==TICK_STATUS_INC)
    {
        userTicker.Tick+=t16;
        if(userTicker.Tick>=userTicker.Target)
        {
            userTicker.userTickerStatus=TICK_STATUS_STOP;
        }
    }
    if(findCardTimer>300){
        Event |= flgEventReadCardTimerUp;
        findCardTimer=0;
    }
    if(uart1ReceivedTimer){
        uart1ReceivedTimeout+=t16;
        if(uart1ReceivedTimeout>=UART1_RECEIVED_TIME_OUT)
        {
            uart1ReceivedTimer=false;
            Event |= flgEventUart1Received;
        }
    }
	adcSamplingTimer+=t16;
	if(adcSamplingTimer>=ADC_TS){
		Event |= flgEvenAdcSamplling;
	}
} 
*/
void timer1_irq_callback(void){
    //seccount++;
    if(noKeyEventTimeOut)noKeyEventTimeOut--;
    Event &= ~ flgEventMenuTimer1Irq;
	Event |= flgEventMenuMainRefrash;
    //extern void valve_process_call_back_in_timer(void);
    //if((void*)(valve_process_call_back_in_timer)){
        valve_process_call_back_in_timer();
   // }
}
/*
void timer0_delay_ms(uint16_t ms)
{
    //what!!!!
    //user_ticker_init();
	userTicker.Target=ms;
    userTicker.Tick=0;    
    userTicker.userTickerStatus=TICK_STATUS_INC;
    //while(userTicker.userTickerStatus!=TICK_STATUS_STOP);
    while(1){
        if(userTicker.userTickerStatus ==TICK_STATUS_STOP )break;
    }
}
 */
/*
void timer1_delay_ms(uint16_t ms)
{
    //what!!!!
    //user_ticker_init();
	userTicker[1].Target=ms;
    userTicker[1].Tick=0;    
    userTicker[1].userTickerStatus=TICK_STATUS_INC;
    //while(userTicker[0].userTickerStatus!=TICK_STATUS_STOP);
    while(1){
        if(userTicker[1].userTickerStatus ==TICK_STATUS_STOP )break;
    }   
}
 */