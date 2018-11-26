#include "../soc/soc.h"
#include "other-gpio.h"
 em_ValveStatus  valveStatus=VALVE_STOPED;
 uint8_t valveTimer=0x00;
void other_gpio_port_init(void)
{
	other_gpio_init();
}
void valve_on(void)
{
    if(valveStatus== VALVE_CLOSEING || valveStatus==VALVE_OPENING)return;
	kz_ddf_power_low();
    kz_ddf_close_low();
    kz_ddf_power_hight();
    valveStatus=VALVE_OPENING;
    valveTimer=0;
}
void valve_off(void)
{
    if(valveStatus== VALVE_CLOSEING || valveStatus==VALVE_OPENING)return;
   
	kz_ddf_power_low();
    kz_ddf_close_hight();
    kz_ddf_power_hight();
    valveStatus=VALVE_CLOSEING;
    valveTimer=0;
}
void valve_stop(void)
{
	kz_ddf_power_low();
    kz_ddf_close_low();
    //kz_ddf_power(1);
}
/*
void valve_process_call_back_in_timer(void)
{
    if(valveStatus== VALVE_CLOSEING || valveStatus==VALVE_OPENING){
        valveTimer++;
        if(valveStatus== VALVE_CLOSEING && valveTimer>=5){
            valveStatus=VALVE_CLOSED;
            valve_stop();
        }
        if(valveStatus== VALVE_OPENING && valveTimer>=10){
            valveStatus=VALVE_OPENED;
            valve_stop();            
        }
    }
    else{
        valve_stop(); 
    }
}*/