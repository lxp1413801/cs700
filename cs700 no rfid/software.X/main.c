
#include "./src/includes/includes.h"
uint8_t firstPowerDown=0;
void m_system_init(void)
{
    SYSTEM_Initialize();
    key_port_init();
    led_port_init();
    other_gpio_port_init();           
    lcd_init();
    lcd_on(); 
    __irq_enable();
}
void m_system_start_self_test(void)
{
    //do some check and test,Start self-test
    //lcd
    lcd_clear_all();
    led_all_on();
    //timer0_delay_ms(2000);    
    delay_1s();
    lcd_show_all();
   // timer0_delay_ms(2000);
    delay_1s();
    led_all_off();
    lcd_blck_on();
    //led        
    //flash 
    //eeprom
    //adc  
}
const uint8_t str[]="UUUUUUUUUUUUUUUUUUUUUUUUU";
//static void timer0_irq_callback(void);
void event_power_monitor(void){
	if(pwr_monitor_get_status()){
		deviceStatus.bits.acPowerDown=1;
        /*
		if(firstPowerDown==0){
			firstPowerDown=1;
			Event |= flgEventSaveRtflow;
		}
        */
		lcd_blck_off();
	}else{
		//firstPowerDown=0;
		deviceStatus.bits.acPowerDown=0;
		//lcd_blck_on();
        if(lcdBackOn){
            lcd_blck_on();
        }
	} 	
}
void event_door_monitor(){
	if(door_monitor_get_status()){
		deviceStatus.bits.doorOpen=1;
	}	
}
void main(void)
{
    m_system_init();
    ui_start_display();
    my_uart1_received_disable();
	//flash and eeprom
    data_api_system_data_init();
    data_api_calibration_table_data_init();
    //pcd
    //mfapi_pcd_init();
	//uart;
    my_uart1_received_ready();
    calVolumeTimeSec=0;
    Event &= ~flgEvenCalVoleume;
    menu=mainSystemData.menuBackUp;
    bkupMenu=menu;
    /*
    while(1){
        my_uart1_send_poll((uint8_t*)str,10);
        delay_1s();
        //delay_1s();
        //delay_1s();
        //delay_1s();
    }
    */
    while (1){
        event_door_monitor();
        event_power_monitor();
        if(Event & flgEventMenuTimer0Irq){timer0_irq_callback();}
        if(Event & flgEventMenuTimer1Irq){timer1_irq_callback();}
        //if(Event & flgEventReadCardTimerUp){rfid_comm_process(); }
		if(Event & flgEvenAdcSamplling){
           flow_sampling_process();
		}
        if(Event & flgEventSaveRtflow){
            if(flashOpStr.op==enFLASH_NOTHING){
            flashOpStr.op=emFLASH_WRITE_main_SYSTEM_DATA;
            //flashOpStr.len=0;
            Event &= ~flgEventSaveRtflow;
            }
        }        
		if(Event & flgEventMenuMainRefrash){
			ui_disp_menu();
		}    
		if(Event & flgEventUart1Received){
            my_uart1_received_disable()
			modbus_response_process(uart1Buffer,uart1ReceivedCount);
            Event &= ~flgEventUart1Received;
            delay_10ms();
            my_uart1_received_ready();
		}
        else {
			ui_key_process();
		}
        if(bkupMenu!=mainSystemData.menuBackUp){
            if(flashOpStr.op == enFLASH_NOTHING){
                flashOpStr.op=emFLASH_WRITE_main_SYSTEM_DATA;
                flashOpStr.len=0;
            }
        }
		if(flashOpStr.op!=enFLASH_NOTHING){
            //lcd_clear_buffer();
            //lcd_disp_str((uint8_t*)" store- ");			
			data_api_flash_process();    
			flashOpStr.op = enFLASH_NOTHING;
		}

        if(noKeyEventTimeOut==0 ){
            if(menu!=0x50){
				menu=bkupMenu;
            }
        }
    }
}
/**
 End of File
*/