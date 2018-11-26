#ifndef __LED_H__
#define __LED_H__


#ifdef __cplusplus
extern "C"{
#endif
//led3 rf1
#define led3_port_init() do{ \
	IO_RF1_SetDigitalMode(); \
	IO_RF1_SetDigitalOutput(); \
	}while(0); 
#define led3_on() IO_RF1_SetHigh();
#define led3_off() IO_RF1_SetLow();
#define led3_change() IO_RF1_Toggle();

//led2 rf0
#define led2_port_init() do{ \
	IO_RF0_SetDigitalMode(); \
	IO_RF0_SetDigitalOutput(); \
	}while(0); 
#define led2_on() IO_RF0_SetHigh();
#define led2_off() IO_RF0_SetLow();
#define led2_change() IO_RF0_Toggle();	

//led1 ra3
#define led1_port_init() do{ \
	IO_RA3_SetDigitalMode(); \
	IO_RA3_SetDigitalOutput(); \
	}while(0); 
#define led1_on() IO_RA3_SetHigh();
#define led1_off() IO_RA3_SetLow();
#define led1_change() IO_RA3_Toggle();	
	
//led4 ra2
#define led5_port_init() do{ \
	IO_RA2_SetDigitalMode(); \
	IO_RA2_SetDigitalOutput(); \
	}while(0);
#define led5_on() IO_RA2_SetHigh();
#define led5_off() IO_RA2_SetLow();
#define led5_change() IO_RA2_Toggle();	

//lcd_blck ra1
#define lcd_blck_port_init() do{ \
	IO_RA1_SetDigitalMode(); \
	IO_RA1_SetDigitalOutput(); \
	}while(0);
#define lcd_blck_on() IO_RA1_SetHigh();
#define lcd_blck_off() IO_RA1_SetLow();
#define lcd_blck_change() IO_RA1_Toggle();	

//led5	ra0
#define led4_port_init() do{ \
	IO_RA0_SetDigitalMode(); \
	IO_RA0_SetDigitalOutput(); \
	}while(0);	
#define led4_on() IO_RA0_SetHigh();
#define led4_off() IO_RA0_SetLow();
#define led4_change() IO_RA0_Toggle();	



#define led_all_on() do{ \
	led1_on();led2_on();led3_on(); \
	led4_on();led5_on();}while(0);

#define led_all_off() do{ \
led1_off();led2_off();led3_off(); \
led4_off();led5_off();}while(0);

extern void led_port_init(void);
extern void led_all_test(void);


#ifdef __cplusplus
}
#endif

#endif