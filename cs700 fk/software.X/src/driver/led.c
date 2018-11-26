#include "../soc/soc.h"
#include "led.h"
#include "../depend/depend.h"
void led_port_init(void)
{
	led1_port_init();
	led2_port_init();
	led3_port_init();
	led4_port_init();
	led5_port_init();
	lcd_blck_port_init();		
}

void led_all_test(void)
{
	led_all_on();
	lcd_blck_on();
	delay_100ms();
	led_all_off();
	lcd_blck_off();	
}