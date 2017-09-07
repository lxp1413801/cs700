#ifndef __OTHER_GPIO_H__
#define __OTHER_GPIO_H__
#ifdef __cplusplus
extern "C" {
#endif
    //ra4 pin28
#define rs485_rxtx_ctrl_port_init() do{IO_RA4_SetDigitalOutput();}while(0);
#define rs485_set_rx() do{IO_RA4_SetLow();}while(0);
#define rs485_set_tx() do{IO_RA4_SetHigh();}while(0);

    //power monitor
    //rb5 pin42
#define pwr_monitor_port_init() do{ \
    /*IO_RA5_SetDigitalMode();*/ \
	IO_RA5_SetDigitalInput();}while(0);	
#define pwr_monitor_get_status() IO_RA5_GetValue()

    //door status
    //rb1 pin46
#define door_monitor_port_init() do{ \
	IO_RB4_SetDigitalInput();}while(0);
#define door_monitor_get_status() IO_RB4_GetValue()

    //kkz power
    //rb0
#define kz_ddf_close_init() do{ \
	IO_RB0_SetDigitalOutput(); \
	}while(0);
#define kz_ddf_close_hight() IO_RB0_SetHigh()
#define kz_ddf_close_low() IO_RB0_SetLow()

    //#define KZ_DDF_POWER(0)     

    //kz ddf close
    //rd7 pin49
#define kz_ddf_power_init() do{ \
	IO_RD7_SetDigitalOutput(); \
	}while(0);
#define kz_ddf_power_hight() IO_RD7_SetHigh()
#define kz_ddf_power_low() IO_RD7_SetLow()

#define other_gpio_init() do{ \
	rs485_rxtx_ctrl_port_init(); \
	pwr_monitor_port_init(); \
	door_monitor_port_init(); \
	kz_ddf_close_init(); \
	kz_ddf_power_low(); \
	kz_ddf_power_init(); \
}while(0);

    typedef enum {
        VALVE_STOPED = 0,
        VALVE_CLOSED = 1,
        VALVE_OPENED = 2,
        VALVE_OPENING = 3,
        VALVE_CLOSEING = 4,
    } em_ValveStatus;
    extern em_ValveStatus valveStatus;
    extern uint8_t valveTimer;
    extern void other_gpio_port_init(void);
    extern void valve_on(void);
    extern void valve_off(void);
    extern void valve_stop(void);
    //extern void valve_process_call_back_in_timer(void);



    //void valve_process_call_back_in_timer(void)
#define valve_process_call_back_in_timer() do{ \
    if(valveStatus== VALVE_CLOSEING || valveStatus==VALVE_OPENING){ \
        valveTimer++; \
        if(valveStatus== VALVE_CLOSEING && valveTimer>=5){ \
            valveStatus=VALVE_CLOSED; \
            valve_stop(); \
        } \
        if(valveStatus== VALVE_OPENING && valveTimer>=10){ \
            valveStatus=VALVE_OPENED; \
            valve_stop();             \
        } \
    } \
    else{ \
        valve_stop();  \
    } \
}while(0);
#ifdef __cplusplus
}
#endif
#endif