#ifndef __EVENT_H__
#define __EVENT_H__

#ifdef __cplusplus
extern "C"{
#endif
extern   uint8_t Event;
#define flgEventReadCardTimerUp 	(uint8_t)(0x01<<0)
#define flgEvenAdcSamplling			(uint8_t)(0x01<<1)
#define flgEventUart1Received 		(uint8_t)(0x01<<2)
#define flgEventMenuMainRefrash		(uint8_t)(0x01<<3)
#define flgEventMenuTimer0Irq     (uint8_t)(0x01<<4)
#define flgEventMenuTimer1Irq       (uint8_t)(0x01<<5)
#define flgEventSaveRtflow      (uint8_t)(0x01<<6)

extern uint8_t event_process_card_read(void);
extern void event_process_flow_sampling(void);

#ifdef __cplusplus
}
#endif

#endif