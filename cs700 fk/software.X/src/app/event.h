#ifndef __EVENT_H__
#define __EVENT_H__

#ifdef __cplusplus
extern "C"{
#endif
extern   uint8_t Event;
#define flgEventReadCardTimerUp 	(0x01<<0)
#define flgEvenAdcSamplling			(0x01<<1)
#define flgEventUart1Received 		(0x01<<2)
#define flgEventMenuMainRefrash		(0x01<<3)
#define flgEventMenuTimer0Irq     (0x01<<4)
#define flgEventMenuTimer1Irq       (0x01<<5)
#define flgEventSaveRtflow      (0x01<<6)

extern uint8_t event_process_card_read(void);
extern void event_process_flow_sampling(void);

#ifdef __cplusplus
}
#endif

#endif