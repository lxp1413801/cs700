
#include "./src/includes/includes.h"
void m_system_init(void)
{
    SYSTEM_Initialize();
    //TMR1_SetInterruptHandler(timer1_irq_callback);
    //TMR0_SetInterruptHandler(timer0_irq_callback);
    
    key_port_init();
    led_port_init();
    other_gpio_port_init();       
    
    lcd_init();
    lcd_on(); 
    //INTERRUPT_GlobalInterruptEnable();
    //INTERRUPT_PeripheralInterruptEnable();
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
//const str[]="UUUUUUUUUUUUUUUUUUUUUUUUUUUUU";
//static void timer0_irq_callback(void);
void main(void)
{

    // initialize the device
    //__nop();
    m_system_init();
	//data_api_write_eeprom_clr();
	//while(1);
    m_system_start_self_test();
    ui_start_display();
    my_uart1_received_disable();
    //flash
    data_api_system_data_init();
    data_api_calibration_table_data_init();
    //pcd
    mfapi_pcd_init();
    //valve_off();
    my_uart1_received_ready();
    while (1){
        if(!door_monitor_get_status()){
            //door_open_precess();
            deviceStatus.bits.doorOpen=1;
        }
        if(pwr_monitor_get_status()){
            //door_open_precess();
            deviceStatus.bits.acPowerDown=1;
            Event |= flgEventSaveRtflow;
            lcd_blck_off();
        }else{
            deviceStatus.bits.acPowerDown=0;
            lcd_blck_on();
        }         
        if(Event & flgEventMenuTimer0Irq){timer0_irq_callback();}
        if(Event & flgEventMenuTimer1Irq){timer1_irq_callback();}
        if(Event & flgEventReadCardTimerUp){
           rfid_comm_process(); 


        }
		if(Event & flgEvenAdcSamplling){
            
           flow_sampling_process();
		}
        if(Event & flgEventSaveRtflow){
            //led4_on();
            data_api_rt_data_save();
            Event &= ~flgEventSaveRtflow;
            //led4_off();
        }
		if(Event & flgEventMenuMainRefrash){

			ui_disp_menu();
            if(warningStatus.bits.moneyLowWarning){
                led4_change();
            }else{
                led4_off();
            }

		}
        
		if(Event & flgEventUart1Received){

            my_uart1_received_disable()
			modbus_response_process(uart1Buffer,uart1ReceivedCount);
            Event &= ~flgEventUart1Received;
            my_uart1_received_ready();

		}
        else {

			ui_key_process();

		}

		if(flashOpStr.op!=enFLASH_NOTHING){

			data_api_flash_process();    
			flashOpStr.op = enFLASH_NOTHING;

		}
        //open valve
        if(deviceStatus.byte==0 && valveStatus==VALVE_CLOSED){
            in_app_vavle_on();
        }
        if(noKeyEventTimeOut==0 ){
            if(menu!=0x50){
             menu=bkupMenu;
            }
            //password=0x00;
        }        
    }
}
/**
 End of File
*/