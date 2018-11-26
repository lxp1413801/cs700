//file name:mfrc522_bal.c
//add other header file
//...
#include "../mfrc522_config.h"
#include "../mfrc522_includes.h"
#include "mfrc522_bal.h"
//declear some macro
#if (__MFRC522_INTERFACE_TYPE!=MFRC522_INTERFACE_TYPE_SPI)
	#error "In addition to SPI, do not support other interface methods!!!"
#endif

#ifndef USED_CUSTOM_INTERFACE_FUNCTION_EN
	#error "must define USED_CUSTOM_INTERFACE_FUNCTION_EN to 0 or 1!!!"	
#endif

#if ((__MFRC522_INTERFACE_TYPE==MFRC522_INTERFACE_TYPE_SPI) && !USED_CUSTOM_INTERFACE_FUNCTION_EN)
//spi transfer data
#ifndef mfrc522_bal_write_read_via_spi
uint8_t mfrc522_bal_write_read_via_spi(uint8_t x)
{
	uint8_t i;
	uint8_t rb=0;
	for(i=0;i<8;i++)
	{
		mfrc522_sck_set_low();

		if(x&0x80){
			mfrc522_mosi_set_hight();
        }
		else{
			mfrc522_mosi_set_low();	
        }
		mfrc522_sck_set_hight();
		x<<=1;

		rb<<=1;
		if(mfrc522_miso_get())
			rb|=1;	
	}
	mfrc522_sck_set_low();
	return rb;		
}
#endif

//spi write
#ifndef mfrc522_bal_write_reg
void mfrc522_bal_write_reg(uint8_t reg,uint8_t x)
{
	mfrc522_nss_enable();

	mfrc522_sck_set_low();
	mfrc522_bal_write_read_via_spi((reg<<1) & 0x7e);
	mfrc522_bal_write_read_via_spi(x);
	mfrc522_nss_disable();	
}
#endif

#ifndef mfrc522_bal_write_reg_len
void mfrc522_bal_write_reg_len(uint8_t reg,uint8_t* buf,uint8_t len)
{
	uint8_t i;
	mfrc522_nss_enable();
	mfrc522_sck_set_low();
	mfrc522_bal_write_read_via_spi((reg<<1) & 0x7e);
	for(i=0;i<len;i++)
	{
		mfrc522_bal_write_read_via_spi(*buf++);
	}
	mfrc522_nss_disable();	
}
#endif
	
//spi read
#ifndef mfrc522_bal_read_reg
uint8_t mfrc522_bal_read_reg(uint8_t reg)
{
	uint8_t x=0;
	mfrc522_nss_enable();
	mfrc522_sck_set_low();
	mfrc522_bal_write_read_via_spi(((reg<<1) & 0x7e) | 0x80);
	x=mfrc522_bal_write_read_via_spi(0x00);
	mfrc522_nss_disable();
    return x;	
}
#endif
/*
#ifndef mfrc522_bal_read_reg_len
void mfrc522_bal_read_reg_len(uint8_t reg,uint8_t* buf,uint8_t len)
{
	uint8_t i;
	if (len == 0)return;
	mfrc522_nss_enable();
	mfrc522_sck_set_low();
	for(i=0;i<len;i++)
	{
        *buf++=mfrc522_bal_read_reg(reg++);
	}
	mfrc522_nss_disable();
}
#endif	
*/
#ifndef mfrc522_bal_read_reg_len_ex
const uint8_t TablerxAlign[8]={0xff,0xfe,0xfc,0xf8,0xf0,0xe0,0xc0,0x80};
/*
void mfrc522_bal_read_reg_len_ex(uint8_t reg,uint8_t* buf,uint8_t len,uint8_t rx_align)
{
	uint8_t mask;
	if(len == 0)return;
	mfrc522_bal_read_reg_len(reg,buf,len);
	mask=TablerxAlign[rx_align];
	buf[0] &= mask;	
}
*/
#endif

#ifndef mfrc522_bal_read_fifo_len_ex
void mfrc522_bal_read_fifo_len_ex(uint8_t reg,uint8_t* buf,uint8_t len,uint8_t rx_align)
{
    uint8_t adr;
	uint8_t i;
	mfrc522_nss_enable();
	mfrc522_sck_set_low();
    adr=((reg<<1) & 0x7e) | 0x80;
	mfrc522_bal_write_read_via_spi(adr);
	for(i=0;i<len;i++)
	{
		*buf++=mfrc522_bal_write_read_via_spi(adr);
	}
	mfrc522_nss_disable();
	i=TablerxAlign[rx_align];
	buf[0] &= i;	
}
#endif	

#ifndef mfrc522_bal_set_bit_reg
void mfrc522_bal_set_bit_reg(uint8_t reg,uint8_t bits)
{
	uint8_t tmp = 0x0;
	tmp = mfrc522_bal_read_reg(reg);
	tmp|=bits;
	mfrc522_bal_write_reg(reg,tmp);  	
}
#endif

#ifndef mfrc522_bal_clr_bit_reg
void mfrc522_bal_clr_bit_reg(uint8_t reg,uint8_t bits)
{
	uint8_t tmp = 0x0;
	tmp = mfrc522_bal_read_reg(reg);
	tmp &= ~bits;
	mfrc522_bal_write_reg(reg,tmp); 	
}
#endif

#endif
//file end
//
