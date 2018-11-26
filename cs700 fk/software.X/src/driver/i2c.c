#include "../soc/soc.h"
#include "i2c.h"
#include "../depend/depend.h"
#define __nop_delay() do{ \
	__nop();__nop();__nop();__nop();__nop();__nop(); \
}while(0);
/*
void iic_device_enable(void)
{
	//port_init();
	#if(cfg_iic_PWR_VIA_GPIO==1)
	iic_pwr_on();
	delay_5ms();
	#endif	
	//iic_wp_disable();	
    iic_wp_low();
}
void iic_device_disable(void)
{
	//iic_wp_enable();
    iic_wp_hight();
    iic_port_disable();
	#if(cfg_iic_PWR_VIA_GPIO==1)
	iic_pwr_off();
	#endif		
}
*/

void iic_start(void)
{
	iic_sda_mode_out();
	iic_sda_hight();
	iic_scl_hight();
	delay_10us();
	iic_sda_low();
	delay_10us();
	iic_scl_low();
	//delay_5ms();
}

void iic_stop(void)
{
	iic_sda_mode_out();
	iic_sda_low();
	delay_10us();
	iic_scl_hight();
	delay_10us();
	iic_sda_hight();
	//delay_5ms();
}

void iic_send_ack(int8_t ack)
{
	iic_sda_mode_out();
	iic_scl_low();
	if(ack)
	{
		iic_sda_hight();
	}
	else
	{
		iic_sda_low();
	}
	__nop_delay();
	iic_scl_hight();
	__nop_delay();
	iic_scl_low();
}

uint8_t iic_waite_ack(void)
{
	uint8_t timeout=200;
	iic_sda_mode_in();
    iic_scl_low();
	__nop_delay();
	iic_scl_hight();
    __nop_delay();
	while(timeout--)
	{
		if(!iic_sda_get())break;
	}
	if(!timeout)
	{
		iic_stop();
		return 0;
	}
	iic_scl_low();
	return 1;	
}

uint8_t iic_send_byte(uint8_t x)
{
	uint8_t i;
    
	iic_sda_mode_out();
	iic_scl_low();
	for(i=0;i<8;i++)
	{       
		if(x&0x80){
			iic_sda_hight();
        }
		else{
			iic_sda_low();
        }
		iic_scl_hight();
		__nop_delay();
		iic_scl_low();
        __nop_delay();
		x<<=1;
	}
	return iic_waite_ack();
}

uint8_t iic_received_byte(void)
{
	uint8_t i;
	uint8_t ret=0;
	iic_sda_mode_in();//iic_sda_mode_in();
	for(i=0;i<8;i++)
	{
        ret<<=1;
		iic_scl_low();
		__nop_delay();
		iic_scl_hight();
        __nop_delay();
		if(iic_sda_get()){
			ret|=1;
		}
	}
	iic_scl_low();
	return ret;
}
uint8_t iic_received_byte_if_ack(uint8_t ack)
{
	uint8_t ret=0;
	ret=iic_received_byte();
	iic_send_ack(ack);
	return ret;
}

uint8_t iic_read_byte(uint8_t addr)
{
	uint8_t ret;
	//device must enable 
	iic_start();
	iic_send_byte(SLV_ADDR);
	iic_send_byte(addr);
	//
	iic_start();
	iic_send_byte(SLV_ADDR+1);
	ret=iic_received_byte_if_ack(IIC_NACK);
	iic_stop();
    return ret;
}
/*
void iic_read_n_byte(uint8_t addr,uint8_t* buf,uint16_t len)
{
	//device must enable 
	uint16_t i;
	iic_start();
	iic_send_byte(SLV_ADDR);
	iic_send_byte(addr);
	//
	iic_start();
	iic_send_byte(SLV_ADDR+1);
	for(i=0;i<len-1;i++)
	{
		*buf++=iic_received_byte_if_ack(IIC_ACK);
	}
	*buf=iic_received_byte_if_ack(IIC_NACK);
	iic_stop();	
}
 * */
void iic_write_byte(uint8_t addr,uint8_t x)
{
	iic_start();
	iic_send_byte(SLV_ADDR);
	iic_send_byte(addr);
	
	iic_send_byte(x);
	iic_stop();	
}
void iic_write_n_byte(uint8_t addr,uint8_t* buf,uint16_t len)
{
	uint16_t i;
	for(i=0;i<len;i++)
	{
		iic_start();
		iic_send_byte(SLV_ADDR);
		iic_send_byte(addr++);
		
		iic_send_byte(*buf++);
		iic_stop();			
	}
}