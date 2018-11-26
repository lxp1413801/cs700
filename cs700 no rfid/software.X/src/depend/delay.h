//file name:delay.h
#ifndef __my_DELAY_H__
#define __my_DELAY_H__
#ifdef __cplusplus
extern "C"{
#endif
	extern void delay(uint16_t t);
    /*
    #if (FREP_DCO==7340032)
        #define MCLK_DELAY_100MS 0xee20
        #define MCLK_DELAY_10MS 0x17d0
        #define MCLK_DELAY_1MS 0x261
        #define MCLK_DELAY_100US 60
		#define MCLK_DELAY_10US 6
    #elif (FREP_DCO==3670016)
        #define MCLK_DELAY_100MS 0x7710
        #define MCLK_DELAY_10MS 0xeb8
        #define MCLK_DELAY_1MS 0x130
        #define MCLK_DELAY_100US 30
		#define MCLK_DELAY_10US 3
    #elif (FREP_DCO==917504)
        #define MCLK_DELAY_100MS 0x1dc4
        #define MCLK_DELAY_10MS 0x2af
        #define MCLK_DELAY_1MS 0x4c
        #define MCLK_DELAY_100US 8
		#define MCLK_DELAY_10US 1
    #else
        #error "error Missing definition:not define dco freaency in config.h"
    #endif
     */
    #define MCLK_DELAY_200MS 0x904c
    #define MCLK_DELAY_100MS 18470
    #define MCLK_DELAY_10MS 1847
    #define MCLK_DELAY_1MS 185
    #define MCLK_DELAY_100US 20
    #define MCLK_DELAY_10US 2  
    
	#define delay_10us() delay(MCLK_DELAY_10US)
    #define delay_20us() delay(MCLK_DELAY_10US*2)
    #define delay_100us() delay(MCLK_DELAY_100US)
    #define delay_1ms() delay(MCLK_DELAY_1MS)
    #define delay_2ms() delay(MCLK_DELAY_1MS*2)
    #define delay_4ms() delay(MCLK_DELAY_1MS*4)
    #define delay_5ms() delay(MCLK_DELAY_1MS*5)
    #define delay_10ms() delay(MCLK_DELAY_10MS)
    #define delay_20ms() delay(MCLK_DELAY_10MS*2)
    #define delay_100ms() delay(MCLK_DELAY_100MS)  	
    #define delay_200ms() delay(MCLK_DELAY_200MS)  	


            
    #define delay_500ms() \
    do{ \
        delay_200ms(); \
        delay_200ms(); \
        delay_100ms(); \
    }while(0);
    #define delay_1s() do{ \
        delay_200ms(); \
        delay_200ms(); \
        delay_200ms(); \
        delay_200ms(); \
        delay_200ms(); \
    }while(0);
    //#define delay_1s() do{delay_500ms();delay_500ms();}while(0);
    #define delay_2s() do{delay_1s();delay_1s();}while(0);
    #define delay_4s() do{delay_2s();delay_2s();}while(0);
	
#ifdef __cplusplus
}
#endif

#endif
//file end
//
