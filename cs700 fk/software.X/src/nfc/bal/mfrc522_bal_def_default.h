//file name:mfrc522_bal_def_default.h
#ifndef __MFRC522_BAL_DEF_H__
#define __MFRC522_BAL_DEF_H__

#include "../../soc/soc.h"
#ifdef __cplusplus
extern "C"{
#endif

	//rst--ra5
	//nss--rc2
	
	//sck--rc3
	//mosi--rc5
	//miso--rc4
	
	//irq--p1.0
	//define via pins at port
	
	//psw-- p2.4 
	
	//#define MFRC522_PSW_PIN	 	BIT4
	//#define MFRC522_PSW_PORT	P2
	

    /*
	#define mfrc522_power_on() do{ \
    P2OUT &= ~MFRC522_PSW_PIN; \
		P2DIR |= MFRC522_PSW_PIN; \
		}while(0);
     */
	#define mfrc522_power_on() ;
	/*
    
	#define mfrc522_power_off() do{ \
		P2OUT	|= MFRC522_PSW_PIN; \
		P2DIR |= MFRC522_PSW_PIN; \
		}while(0);
*/
    #define mfrc522_power_off() ;
/*
	#define	MFRC522_RST		BIT1
	#define	MFRC522_NSS		BIT0

	#define	MFRC522_SCK		BIT7
	#define	MFRC522_MOSI	BIT6
	#define	MFRC522_MISO	BIT5
	
	#define	MFRC522_IRQ		BIT0
*/
	//
/*
#define	MFRC522_RST_PORT	P3OUT
	#define	MFRC522_NSS_PORT	P3OUT	
	
	#define	MFRC522_SCK_PORT	P2OUT
	#define	MFRC522_MOSI_PORT	P2OUT	
	#define	MFRC522_MISO_PORT	P2IN

	#define	MFRC522_IRQ_PORT	P1IN
*/
	//
	/*
	#define mfrc522_port_dis() do{ \
		P3DIR &= ~(MFRC522_RST+MFRC522_NSS); \
		P2DIR  &= ~(MFRC522_SCK+MFRC522_MOSI+MFRC522_IRQ); \
		P1DIR &= ~MFRC522_IRQ; \
	}while(0);
	*/
	#define mfrc522_port_dis() (void)
	//define port mode
	
	/*
	#define mfrc522_port_init() do{ \
		P3OUT |= (MFRC522_RST+MFRC522_NSS); \
		P2OUT |= (MFRC522_SCK+MFRC522_MOSI); \
		P3DIR |= (MFRC522_RST+MFRC522_NSS); \
		P2DIR |= (MFRC522_SCK+MFRC522_MOSI); \
		P2DIR &= ~MFRC522_MISO; \
		P1DIR &= ~MFRC522_IRQ;\
		}while(0);
	*/
	#define mfrc522_port_init() do{ \
		IO_RB5_SetDigitalOutput(); \
		IO_RC2_SetDigitalOutput(); \
		IO_RC3_SetDigitalOutput(); \
		IO_RC4_SetDigitalOutput(); \
		IO_RC5_SetDigitalInput(); \
		IO_RB5_SetHigh(); \
		IO_RC2_SetHigh(); \
		IO_RC3_SetHigh(); \
		IO_RC5_SetHigh(); \
	}while(0);
		
	//define some operation
	#define	mfrc522_sleep() 	IO_RB5_SetLow();
	#define	mfrc522_wakeup() 	IO_RB5_SetHigh();
	#define	mfrc522_rst()		mfrc522_wakeup();
	
	#define	mfrc522_rst_set_hight() mfrc522_wakeup();
	#define	mfrc522_rst_set_low()	mfrc522_sleep();
	
	#define mfrc522_rst_enable() mfrc522_rst_set_hight()
	#define mfrc522_rst_disable() mfrc522_rst_set_low()
	
	#define	mfrc522_sck_set_hight()	IO_RC3_SetHigh();
	#define	mfrc522_sck_set_low()	IO_RC3_SetLow();
	
	#define	mfrc522_nss_set_hight()	IO_RC2_SetHigh();
	#define	mfrc522_nss_set_low()	IO_RC2_SetLow();

	#define mfrc522_nss_enable() mfrc522_nss_set_low()
	#define mfrc522_nss_disable() mfrc522_nss_set_hight()
	
	#define	mfrc522_mosi_set_hight()	IO_RC4_SetHigh();	
	#define	mfrc522_mosi_set_low()	IO_RC4_SetLow();
	
	#define	mfrc522_miso_get()	IO_RC5_GetValue()
	//#define	mfrc522_irq_get()	MFRC522_IRQ_PORT & MFRC522_IRQ           
    
#ifdef __cplusplus
}
#endif

#endif