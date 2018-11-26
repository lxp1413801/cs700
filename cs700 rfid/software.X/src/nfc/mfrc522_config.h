//file name : mfrc522_config.h
#ifndef __MFRC522_CONFIG_H__
#define __MFRC522_CONFIG_H__

#ifdef __cplusplus
extern "C"{
#endif

#ifndef __MFRC522_INTERFACE_TYPE
	#define __MFRC522_INTERFACE_TYPE MFRC522_INTERFACE_TYPE_SPI
#endif



//define spi interface 
#define USED_CUSTOM_INTERFACE_FUNCTION_EN 0
#define mfrc522_bal_port_init() mfrc522_port_init();
#define mfrc522_bal_rst_set_low() mfrc522_rst_set_low();
#define mfrc522_bal_rst_set_hight() mfrc522_rst_set_hight();

//define ehco enable
#define MFRC522_ECHO_EN 0
#if MFRC522_ECHO_EN==1
#define echo_send_polling uart_send_polling
#endif

//define somd delay
#define MFRC522_DELAY_10MS MCLK_DELAY_10MS
#define MFRC522_DELAY_100MS MCLK_DELAY_100MS
#ifdef delay_100ms
    #define mfrc522_delay_100ms() delay_100ms();
#else
    #define mfrc522_delay_100ms() ;    
#endif

#define mfrc522_delay_10ms()  delay_10ms()

#ifndef ___no_operation
#define  ___no_operation __nop
#endif    
    
#ifdef __cplusplus
}
#endif


#endif
//file end
