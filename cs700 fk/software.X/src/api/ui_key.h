#ifndef __ui_key_h__
#define __ui_key_h__

#ifdef __cplusplus
extern "C" {
#endif
	#define psw_CLEAR_VOLUME 				1
	#define psw_CLEAR_BALANCE 				2
	#define psw_ADJUST_FLOW_LINE 			3
    #define psw_FLOW_LINE_RESET             4
	#define psw_SET_SMALL_FLOW_HOW_CARE 	5
	#define psw_SET_PRICE					6
	
	#define psw_SET_transferMoneyLimit		7
	#define psw_SET_balance_Warning_Off		8
	#define psw_TEST_RECHARGE 				9
	#define psw_CLEAR_CARD_ID				10
	#define psw_SET_FLOW_ZERO				11
	#define psw_SET_CALIB_TABLE 			12
	#define psw_SET_MODBUS_ADDR 			13
	#define psw_CLEAR_EEPROM				14

    
    extern uint8_t calibLocation;
    extern uint8_t noKeyEventTimeOut;
	extern void ui_key_process(void);
    extern uint16_t ui_key_waite_up(void); 
#ifdef __cplusplus
}
#endif
#endif
//file end
