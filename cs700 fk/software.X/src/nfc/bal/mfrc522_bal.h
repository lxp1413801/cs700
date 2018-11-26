//file:mfrc_bal.h
#ifndef __MFRC522_BAL_H__
#define __MFRC522_BAL_H__

#ifdef __cplusplus
	extern "C"{
#endif

#if (__MFRC522_INTERFACE_TYPE!=MFRC522_INTERFACE_TYPE_SPI)
	#error "In addition to SPI, do not support other interface methods!!!"
#endif

#ifndef USED_CUSTOM_INTERFACE_FUNCTION_EN
	#error "must define USED_CUSTOM_INTERFACE_FUNCTION_EN to 0 or 1!!!"	
#endif

#if(USED_CUSTOM_INTERFACE_FUNCTION_EN)
	
	#ifndef mfrc522_bal_write_read_via_spi
		#error "if used constom spi funtion ,mfrc522_bal_write_read_via_spi() no defined!!!"
	#endif
	
	#ifndef mfrc522_bal_write_reg
		#error "if used constom spi funtion ,mfrc522_bal_write_reg() no defined!!!"
	#endif

	/*
	#ifndef mfrc522_bal_write_reg_len
		#error "if used constom spi funtion ,mfrc522_bal_write_reg_len() no defined!!!"
	#endif	
	*/
	
	#ifndef mfrc522_bal_read_reg
		#error "if used constom spi funtion ,mfrc522_bal_read_reg() no defined!!!"
	#endif	
	
	/*
	#ifndef mfrc522_bal_write_reg_len_ex
		#error "if used constom spi funtion ,mfrc522_bal_write_reg_len_ex() no defined!!!"
	#endif	
	
	#ifndef mfrc522_bal_write_fifo_len_ex
		#error "if used constom spi funtion ,mfrc522_bal_write_reg_len_ex() no defined!!!"
	#endif
	*/
#endif

#if ((__MFRC522_INTERFACE_TYPE==MFRC522_INTERFACE_TYPE_SPI) && (!USED_CUSTOM_INTERFACE_FUNCTION_EN))
	//declear some interface function for spi
	
	//include header file for spi port and pins

	#include "mfrc522_bal_def_default.h"
    #include "stdint.h" 
	//spi transfer data
	#ifndef mfrc522_bal_write_read_via_spi
		extern uint8_t mfrc522_bal_write_read_via_spi(uint8_t x);
	#endif
	//spi write
	#ifndef mfrc522_bal_write_reg
		extern void mfrc522_bal_write_reg(uint8_t reg,uint8_t x);
	#endif
	
	#ifndef mfrc522_bal_write_reg_len
		extern void mfrc522_bal_write_reg_len(uint8_t reg,uint8_t* buf,uint8_t len);
	#endif

	//spi read
	#ifndef mfrc522_bal_read_reg
		uint8_t mfrc522_bal_read_reg(uint8_t reg);
	#endif
	
	#ifndef mfrc522_bal_read_reg_len
		extern void mfrc522_bal_read_reg_len(uint8_t reg,uint8_t* buf,uint8_t len);
	#endif	

	#ifndef mfrc522_bal_read_reg_len_ex
		extern void mfrc522_bal_read_reg_len_ex(uint8_t reg,uint8_t* buf,uint8_t len,uint8_t rx_align);
	#endif

	#ifndef mfrc522_bal_read_fifo_len_ex
		extern void mfrc522_bal_read_fifo_len_ex(uint8_t reg,uint8_t* buf,uint8_t len,uint8_t rx_align);
	#endif		

	//define other function
	#ifndef mfrc522_bal_set_bit_reg
		//extern void mfrc522_bal_set_bit_reg(uint8_t reg,uint8_t bit);
        extern void mfrc522_bal_set_bit_reg(uint8_t reg,uint8_t bits);
	#endif
	
	#ifndef mfrc522_bal_clr_bit_reg
		//extern void mfrc522_bal_clr_bit_reg(uint8_t reg,uint8_t bit);
        extern void mfrc522_bal_clr_bit_reg(uint8_t reg,uint8_t bits);
	#endif
#endif //end macro __MFRC522_INTERFACE_TYPE

#ifdef __cplusplus
	}
#endif	//end macro __cplusplus

#endif	//end macro __MFRC522_BAL_H__
//end




