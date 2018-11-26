#include "../soc/soc.h"
#include "key.h"
#include "../depend/depend.h"
extern uint8_t Event;
uint8_t KeyValue=0;
#define key_scan_delay_10ms() delay_10ms();
void key_scan_call(void)
{
    KeyValue=0;
    //uint8_t t1=0;
    //t1=(PORTB & 0x0e);
    if((PORTB & 0x0e) != 0x0e){
        //if((PORTA & 0x01<<5)==0)return;
        //if((PORTB & 0x01<<4)==0)return;
        key_scan_delay_10ms();
		if((PORTB & 0x0e)!=0x0e)
		{
			KeyValue=(PORTB & 0x0e);
			KeyValue>>=1;
			KeyValue = ~KeyValue;
			KeyValue &=0x07;
		}
		/*
        if(t1 == (PORTB & 0x0e)){
            t1>>=1;
            t1 =~ t1;
            KeyValue =t1 & 0x07;
        }
		*/
    }
    /*
    if((t1 & 0x07)!=0x07){
        key_scan_delay_10ms();
        
    }
	do{
		if(!key_get_up()){
			key_scan_delay_10ms();
			if(!key_get_up()){
				t8 &= ~upKEY_DOWN;
				break;
			}	
		}
		t8|=upKEY_DOWN;
	}while(0);
	do{
		if(!key_get_down()){
			key_scan_delay_10ms();
			if(!key_get_down()){
				t8 &= ~downKEY_DOWN;
				break;
			}	
		}
		t8|=downKEY_DOWN;
	}while(0);	
	do{
		if(!key_get_set()){
			key_scan_delay_10ms();
			if(!key_get_set()){
				t8 &= ~setKEY_DOWN;
				break;
			}	
		}
		t8|=setKEY_DOWN;
	}while(0);	
    t8=~t8;
    KeyValue=t8 & 0x07;
     */
}