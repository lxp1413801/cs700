#ifndef __ui_key_h__
#define __ui_key_h__

#ifdef __cplusplus
extern "C" {
#endif

    //#define psw_FLOW_LINE_RESET           129
	//#define psw_SET_SMALL_FLOW_HOW_CARE 	133
	//#define psw_TEST_RECHARGE 			95
	//#define psw_CLEAR_EEPROM				14

    #define psw_SET_DEVICE_ID               6
    #define psw_SET_balance_Warning_Off		7
    #define psw_SET_transferMoneyLimit		8
    #define psw_SET_PRICE                   9
    #define psw_SET_FLOW_ZERO               60
    #define psw_CLEAR_CARD_ID               76
    #define psw_CLEAR_TransferMoney         100
    #define psw_CLEAR_VOLUME                103
    #define psw_CLEAR_BALANCE               107
    #define psw_ADJUST_FLOW_LINE            109
    #define psw_SET_SMALL_FLOW_HOW_CARE     133

	#define psw_SET_imdFLOW_UNIT            136
    #define psw_SET_CALIB_TABLE             168
    #define psw_SET_MODBUS_ADDR             178
    
    extern uint8_t calibLocation;
    extern uint8_t noKeyEventTimeOut;
	extern void ui_key_process(void);
#ifdef __cplusplus
}
#endif
#endif
//file end
