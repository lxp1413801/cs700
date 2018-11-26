#include "../soc/soc.h"
#include "other-gpio.h"
#include "../app/event.h"
#include "../driver/led.h"

 //em_ValveStatus  valveStatus=VALVE_STOPED;
 //uint8_t valveTimer=0x00;
void other_gpio_port_init(void)
{
	other_gpio_init();
}
/*
void valve_on(void)
{
    if(valveStatus== VALVE_CLOSEING || valveStatus==VALVE_OPENING)return;
	//kz_ddf_power_low();
    //kz_ddf_close_low();
    kz_ddf_power_hight();
    valveStatus=VALVE_OPENING;
    valveTimer=0;
}
void valve_off(void)
{
    //uint8_t t=3;
    if(valveStatus== VALVE_CLOSEING || valveStatus==VALVE_OPENING)return;
   
	//kz_ddf_power_low();
    while((Event & flgEventMenuTimer1Irq)==0);
    Event &= ~flgEventMenuTimer1Irq;      
    kz_ddf_close_hight();
   // while(t>0){
        while((Event & flgEventMenuTimer1Irq)==0);
        Event &= ~flgEventMenuTimer1Irq;  
       // t--;
   // }
    kz_ddf_power_hight();
    valveStatus=VALVE_CLOSEING;
    valveTimer=0;
}
void valve_stop(void)
{
    //uint8_t t=3;
    while((Event & flgEventMenuTimer1Irq)==0);
    Event &= ~flgEventMenuTimer1Irq;  
    
    kz_ddf_power_low();
   // while(t>0){
        while((Event & flgEventMenuTimer1Irq)==0);
        Event &= ~flgEventMenuTimer1Irq;  
       // t--;
   // }
    kz_ddf_close_low();

    //kz_ddf_power(1);
}
*/