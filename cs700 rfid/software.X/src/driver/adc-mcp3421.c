#include "../soc/soc.h"
#include "../depend/depend.h"
#include "driver.h"
#include "../app/flow.h"

#define _ADC_18_BITS 0
#define _ADC_16_BITS 1
#define _ADC_14_BITS 2
#define _ADC_12_BITS 3
#define _ADC_RESULT_BITS _ADC_12_BITS
void adc_mcp3241_start_convert_16(uint8_t b,uint8_t g)
{
	uint8_t t8=b;
    iic_scl_mode_out();
    iic_start();
    //start adc
    iic_send_byte(MCP3421_SLAVE_ADDR);
	if(t8>=2)t8=2;
	t8<<=2;
	t8 |= (g & 0x03);
    iic_send_byte(0x80 | t8);
    iic_stop();	
    __irq_disable();
	//adcSamplingTimer=0;
    adcSamplingTimerOneTime=0;
    __irq_enable();
}
uint16_t adc_mcp3241_read_convert_16(uint8_t* sta)
{
	volatile uint16_t t16=0;
    iic_start();
    iic_send_byte(MCP3421_SLAVE_ADDR | 0x01);
    t16 |= iic_received_byte_if_ack(IIC_ACK);
    t16 <<=8;
    t16 |= iic_received_byte_if_ack(IIC_ACK);    
    *sta=iic_received_byte_if_ack(IIC_NACK);
    iic_stop();	
	//if()
	return t16;
}
