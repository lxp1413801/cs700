#ifndef __ui_h__
#define __ui_h__

#ifdef __cplusplus
extern "C" {
#endif


	
    extern uint8_t menu;
   extern  uint8_t bkupMenu;
	extern uint8_t uiAdjbuf[0x08];
    extern uint16_t password;
    extern void ui_disp_hello_ex(uint8_t t);
    extern void ui_disp_hello(void);
    extern void ui_disp_logo(uint8_t dly);
    //extern void ui_disp_str_num(uint8_t* str, uint16_t num);
	extern void ui_disp_str_num_s(uint8_t* str, uint16_t num);
	#define ui_disp_str_num(str,num) do{ \
		ui_disp_str_num_s(str,num); \
		lcd_disp_refresh(); \
	}while(0);
	
    extern void ui_disp_str_dly(uint8_t* str, uint8_t dly);
    //
    extern void ui_start_display(void);
    //
    
    extern void ui_disp_menu(void);

    extern void ui_disp_main(void);
    //extern void ui_disp_password_enter(void);
    extern void ui_disp_clear_current(void);
    extern void ui_disp_clear_totale(void);
    extern void ui_disp_set(void);
    extern void ui_disp_balance_and_volume(void);
    /*
    
    #define ui_key_process_up() do{ \
     switch(menu & 0xf0){ \
         case 0x00: \
             ui_key_waite_up(); \
             menu+=1; \
             if(menu>3)menu=0; \
             break; \
         case 0x10: \
             key_process_up_down_variable_speed(&password,199,true); \
             break; \
         case 0x20: \
             break;	 \
     } \
    }while(0); 
    
    
    
     */
    /*
    #define ui_disp_menu() do{ \
        Event &= ~flgEventMenuMainRefrash; \
        switch (menu & 0xf0) { \
            case 0x00:ui_disp_main();break; \
            case 0x10:ui_disp_password_enter();break; \
            case 0x20:ui_disp_clear_current();break;   \
            case 0x30:ui_disp_clear_totale(); break; \
            case 0x40:ui_disp_set();break;  \
            default:break; \
        } \
    }while(0);
     */
    /*	 
	#define ui_key_process_down() do{ 	\
		switch (menu & 0xf0) { 			\
			case 0x00: 						\
				ui_key_waite_up(); 			\
				if (menu > 0){menu -= 1;} 	\
				else 						\
					menu = 3; 				\
				break; 						\
			case 0x10: 						\
				key_process_up_down_variable_speed(&password, 199, false); \
				break; 						\
			case 0x30: 						\
				ui_key_waite_up(); 			\
				data_api_save_rt_data(); 	\
				data_api_clear_total_volume(); \
				menu = 0; 					\
				password = 0; 				\
				break; 						\
			case 0x20: 						\
			default: 						\
				break; 						\
		} \
	}while(0);
	*/

    /*
     
     #define ui_key_process_set() do{ \
    switch (menu & 0xf0) { \
        case 0x00: \
            menu = 0x10;\
            ui_key_waite_up(); \
            break; \
        case 0x10: \
            ui_key_password_enter(); \
            break; \
        case 0x30: \
            menu = 0x01; \
            password = 0; \
            ui_key_waite_up(); \
            break; \
        case 0x40: \
            ui_key_flowRatio_setting(); \
            break; \
    } \
}while(0);
     
     */
    //extern void key_scan_call(void);
    
    extern void ui_disp_rfid_err(void);
#ifdef __cplusplus
}
#endif

#endif 