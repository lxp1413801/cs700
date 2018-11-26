#ifndef __TIMER_H__
#define __TIMER_H__


#ifdef __cplusplus 
extern "C"{
#endif
typedef enum {
	TICK_STATUS_STOP,
	TICK_STATUS_INC,
	TICK_STATUS_DEC,
}em_userTickerStatus;
typedef enum {
	TICK_STEP_1ms,
	TICK_STEP_10ms,
	TICK_STEP_100ms,
}em_userTickerSetp;
typedef struct{
	em_userTickerStatus userTickerStatus;
	em_userTickerSetp	userTickerStep;
	uint16_t 		Tick;
	uint16_t		Target;
}st_userTicker;

#define USER_TICK_COUNT 2
//extern st_userTicker userTicker[USER_TICK_COUNT];
extern uint16_t globlTickerms;
extern st_userTicker userTicker;
//extern uint32_t globlTicker;
//extern uint16_t findCardTimer;
extern uint16_t systemTikerms;
extern uint8_t systemTickerSec;
extern uint8_t calVolumeTimeSec;
extern uint8_t sec;
extern uint8_t min;
extern uint8_t hour;
 extern  void timer0_irq_callback(void);
extern void timer1_irq_callback(void);
//extern void timer0_delay_ms(uint16_t ms);
//extern void timer1_delay_ms(uint16_t ms);

extern void set_globle_ticker(uint16_t tk);
extern uint16_t get_globle_ticker(void);
#ifdef __cplusplus 
}
#endif
#endif