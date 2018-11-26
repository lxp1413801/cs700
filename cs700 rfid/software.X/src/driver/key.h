#ifndef __KEY_H__
#define __KEY_H__

#ifdef __cplusplus
extern "C"{
#endif

#define key_port_init() do{ \
	IO_RB1_SetDigitalInput(); \
	IO_RB2_SetDigitalInput(); \
	IO_RB3_SetDigitalInput(); \
}while(0);
#define key_get_up() IO_RB3_GetValue()
#define key_get_down() IO_RB2_GetValue()
#define key_get_set() IO_RB1_GetValue()

#define upKEY_DOWN 		(1<<2)
#define downKEY_DOWN 	(1<<1)
#define setKEY_DOWN 	(1<<0)

extern uint8_t KeyValue;

extern void key_scan_call(void);


#ifdef __cplusplus
}
#endif

#endif