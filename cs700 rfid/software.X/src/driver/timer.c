#include "../soc/soc.h"
#include "driver.h"
#include "../app/app.h"
#include "./other-gpio.h"
#include "../configs/configs.h"
#include"../api/ui_key.h"
//st_userTicker userTicker[1];
//st_userTicker userTicker;
uint16_t globlTickerms=0;
uint16_t findCardTimer=0x00;
uint16_t systemTikerms=0x00;
uint8_t systemTickerSec=0;
uint8_t sec=0;
uint8_t min=0;
uint8_t hour=0;
uint8_t calVolumeTimeSec=0;      

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

    Event &= ~ flgEventMenuTimer0Irq;
    tmTemp16=systemTikerms;
    systemTikerms=0;

    findCardTimer+=tmTemp16;

    if(findCardTimer>cfg_FIND_CARD_INTERVAL){
        Event |= flgEventReadCardTimerUp;
        findCardTimer=0;
    }

	adcSamplingTimerOneTime+=tmTemp16;
	if(adcSamplingTimerOneTime>=ADC_TS){
		Event |= flgEvenAdcSamplling;
	}
} 

void timer1_irq_callback(void){
    //seccount++;
    if(noKeyEventTimeOut)noKeyEventTimeOut--;
    Event &= ~ flgEventMenuTimer1Irq;
	Event |= flgEventMenuMainRefrash;

    
	do{
		if(valveStatus== VALVE_CLOSEING || valveStatus==VALVE_OPENING){ 
			valveTimer++; 
			if(valveStatus== VALVE_CLOSEING && valveTimer>=20){ 
				valveStatus=VALVE_CLOSED; 
				valve_stop();           
			} 
			if(valveStatus== VALVE_OPENING && valveTimer>=20){ 
				valveStatus=VALVE_OPENED; 
				valve_stop(); 
			} 
		} 
		else{ 
		
		} 
	}while(0);    
	sec+=systemTickerSec;
	systemTickerSec=0;
	if(sec>=60){
		sec-=60;
		min++;
	}
	if(min>=60){
		min-=60;
		hour++;
	}
	if(hour>=1){
		Event |=flgEventSaveRtflow;
		sec=0;
		min=0;
		hour=0;
	}
}

