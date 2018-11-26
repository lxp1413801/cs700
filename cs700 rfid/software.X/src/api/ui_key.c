#include "../soc/soc.h"
#include "../includes/includes.h"
uint8_t noKeyEventTimeOut=30;
uint8_t calibLocation=0;
/*
void ui_key_waite_up(void)
{
    do{
		
    }
}
 */
//#define ui_key_waite_up() do{key_scan_call();}while(KeyValue);

uint16_t ui_key_waite_up(void) {
    //uint16_t em = get_globle_ticker();
    uint16_t now=0;
    
     
     key_scan_call();
     if (KeyValue == 0)return 0;
     set_globle_ticker(0);
    do {
        key_scan_call();
        now = get_globle_ticker();
        if (KeyValue == 0)break;
        if (now  > 2000)break;
    } while (1);
    return (uint16_t) (now );
}
uint16_t ui_key_waite_up_ex(uint16_t tm) {
    //uint16_t em = get_globle_ticker();
    uint16_t now=0;
    
     
     key_scan_call();
     if (KeyValue == 0)return 0;
     set_globle_ticker(0);
    do {
        key_scan_call();
        now = get_globle_ticker();
        if (KeyValue == 0)break;
        if (now  > tm)break;
    } while (1);
    return (uint16_t) (now );
}
void key_process_up_down_variable_speed_ex(
	int16_t *val, int16_t min,int16_t max, bool up) {
    bool change = true;
    uint16_t speed = 0, t32;
    uint8_t key = KeyValue;
	if(min>=max)return;
    set_globle_ticker(0);
    do {
        if (change == true) {
            change = false;
            if (up) {
                (*val) += 1;
                if (*val > max)*val = min;
            } else {
                if (*val <= min)*val = max;
                else {
                    (*val) -= 1;
                }
            }
			ui_disp_menu();//bug stack meby over flow
        }
        key_scan_call();
        if (key != KeyValue)break;
        t32 = get_globle_ticker();
        if (t32 <= 1000) {
            if (t32 - speed >= 200) {
                speed = t32;
                change = true;
            }
        } else if (t32 <= 2000) {
            if (t32 - speed >= 100) {
                speed = t32;
                change = true;
            }
        } else {
            if (t32 - speed >= 10) {
                speed = t32;
                change = true;
            }
        }
    } while (1);
}
void ui_key_process_up_down_main(bool up) 
{
    if(!up){

		menu++;
		if(menu>2)menu=0;        
    } 
    bkupMenu=menu;	
}
uint8_t  ui_key_process_down(void){
    switch(menu){
    case 0xe0:
        calibLocation=0;    
        flashOpStr.op=emFLASH_WRITE_main_CALIB_DATA;   
        *((uint16_t*)(flashOpStr.buf))=0;
        data_api_calibration_table_data_save(calibLocation,true);
        //flow=0;
        *((uint16_t*)uiAdjbuf)=0;
        break;
    case 0x20://clear total volumeResolution
        flashOpStr.flashAddr= \
            __get_offset(st_SystemData,BalanceMoney);
        flashOpStr.op=emFLASH_WRITE_main_SYSTEM_DATA;
        flashOpStr.len=4;
        m_mem_set(flashOpStr.buf,0,4);
        break;
        //menu=bkupMenu; return 1;           
    case 0x30://clear total
        flashOpStr.flashAddr= \
            __get_offset(st_SystemData,totalConsumeVolume);
        flashOpStr.op=emFLASH_WRITE_main_SYSTEM_DATA;
        flashOpStr.len=4;
        m_mem_set(flashOpStr.buf,0,4);
        //menu=bkupMenu;return 1;
        break;
        /*
	case 0xd0:
        flashOpStr.flashAddr= \
            __get_offset(st_SystemData,cardID);
        flashOpStr.op=emFLASH_WRITE_main_SYSTEM_DATA;
        flashOpStr.len=4;
        //m_mem_set(flashOpStr.buf,0,4);
		*((uint32_t*)(flashOpStr.buf))=0;
        //menu=0xd1; return 1;  
        break;
        */
    case 0xc0:
        
        flashOpStr.flashAddr=      __get_offset(st_SystemData,transferMoney);
        flashOpStr.op=emFLASH_WRITE_main_SYSTEM_DATA;
        flashOpStr.len=6;
        m_mem_set(flashOpStr.buf,0,6);
		//*((uint32_t*)(flashOpStr.buf))=0;   
        data_api_rt_data_save();
        flashOpStr.flashAddr=  __get_offset(st_SystemData,cardID);
        flashOpStr.op=emFLASH_WRITE_main_SYSTEM_DATA;
        flashOpStr.len=4;
		*((uint32_t*)(flashOpStr.buf))=0;  
        data_api_rt_data_save();
        break;
        default:
            return 0;
    }
    return 0;
}
void ui_key_process_up_down(bool up) {
	if(menu<3){
        ui_key_process_up_down_main(up);
        return;
    }
	switch(menu){

		case 0x10:	//密码选择
			key_process_up_down_variable_speed_ex((int16_t*)uiAdjbuf,0, 200, up);
            break;
		case 0x40:	//线性系数
			key_process_up_down_variable_speed_ex((int16_t*)uiAdjbuf,1, 9999, up);
			break;
        case 0x50:
			key_process_up_down_variable_speed_ex((int16_t*)uiAdjbuf,0, 9999, up);
			break;            
        case 0x60:
			key_process_up_down_variable_speed_ex((int16_t*)uiAdjbuf,0, 9999, up);
			break;              
        case 0xa0:	//485地址
			key_process_up_down_variable_speed_ex((int16_t*)uiAdjbuf,1, 253, up);
			break; 
        
		case 0x70:	//小信号切除	
			key_process_up_down_variable_speed_ex((int16_t*)uiAdjbuf,0, 20, up);
			break; 
		case 0x80:	//单价（0-10.00）
			key_process_up_down_variable_speed_ex((int16_t*)uiAdjbuf,10, 1000, up);
			break; 
		case 0x90:	//传输限额(0-1000)
			key_process_up_down_variable_speed_ex((int16_t*)uiAdjbuf,0, 1000, up);
			break;		
		case 0xb0:	//报警值
			key_process_up_down_variable_speed_ex((int16_t*)uiAdjbuf,-100, 100, up);
			break;		
		case 0xb1:	//关阀值
			key_process_up_down_variable_speed_ex((int16_t*)uiAdjbuf,-100, 100, up);
			break;				
		case 0xf1:
			key_process_up_down_variable_speed_ex((int16_t*)uiAdjbuf,0, 1, up);
			break;
	}
}

void ui_key_process_enter(void) {
	if(menu!=0x10)return;        
	switch(password){
		case psw_CLEAR_BALANCE:	
			ui_key_waite_up();
			menu=0x20;
			break;
		case psw_CLEAR_VOLUME:
			ui_key_waite_up();
			menu=0x30;
			break;
		case psw_ADJUST_FLOW_LINE:
			m_mem_cpy_len(uiAdjbuf,(uint8_t*)(&mainSystemData.flowRatio),2);
			menu=0x40;
			break;
		case psw_SET_CALIB_TABLE:
			calibLocation=0;
			*((uint16_t*)uiAdjbuf) = Main_flowCalibrationTable.flowCalibrationArr[calibLocation].rtflowValue;
			menu=0x50;break;
		case psw_SET_DEVICE_ID:
			*(int16_t*)uiAdjbuf=(uint16_t)(mainSystemData.deviceID);
			menu=0x60;
			break;      
		case psw_SET_SMALL_FLOW_HOW_CARE:
			uiAdjbuf[0]=mainSystemData.smallFlowNotCare;
			menu=0x70;
			break; 
		case psw_SET_PRICE:
			*(uint16_t*)uiAdjbuf=mainSystemData.price;
			menu=0x80;
			break;
		case psw_SET_transferMoneyLimit:
			//__q31=mainSystemData.price;
			*(int16_t*)uiAdjbuf=(mainSystemData.transferMoneyLimit/100);
			menu=0x90;
			break;	
		case psw_SET_MODBUS_ADDR:			
			uiAdjbuf[0]=mainSystemData.shortID;
			menu=0xa0;
			break;   
		case psw_SET_balance_Warning_Off:
			*(int16_t*)uiAdjbuf=(mainSystemData.balanceWarning/100);
			menu=0xb0;
			break;
		//case psw_TEST_RECHARGE:	
			//menu=0xc0;break; 	
        case psw_CLEAR_TransferMoney:
            menu=0xc0;break;
           // break;
		//case psw_CLEAR_CARD_ID:
			//menu=0xd0;break;
		case psw_SET_FLOW_ZERO:
			calibLocation=0;
			*((int16_t*)(uiAdjbuf))=flow;
			menu=0xe0;break;			
		//case psw_FLOW_LINE_RESET:
			//menu=0xf0;break;
		case psw_SET_imdFLOW_UNIT:
			*((int16_t*)(uiAdjbuf))=(uint16_t)(mainSystemData.imdFlowUnit);
			menu=0xf1;
			break;
         
		default:break; 
    }
}

void ui_key_process_cancel(void){
    
    uint8_t ret=0;
    if(menu<3){
        uiAdjbuf[0]=0;
        uiAdjbuf[1]=0;
        bkupMenu=menu;
        password=0x00;
        menu=0x10;
		return;
	}
	switch(menu){
		case 0x50:
			if(*((uint16_t*)uiAdjbuf) != Main_flowCalibrationTable.flowCalibrationArr[calibLocation].rtflowValue){
				flashOpStr.op=emFLASH_WRITE_main_CALIB_DATA;   
				*((uint16_t*)(flashOpStr.buf))=*((uint16_t*)uiAdjbuf);
				ret=data_api_calibration_table_data_save(calibLocation,true);
				//add at 20170605
				if(!ret)break;
				//add end
			 }
			calibLocation++;
			if(calibLocation>=FLOW_CALIBRATION_POINT_NUM)calibLocation=0;
			*(uint16_t*)uiAdjbuf = Main_flowCalibrationTable.flowCalibrationArr[calibLocation].rtflowValue;       
			break;
		case 0xb0:
			flashOpStr.flashAddr= \
				__get_offset(st_SystemData,balanceWarning);		
			flashOpStr.len=2;	
			__t16=*((uint16_t*)uiAdjbuf);
			__t16*=100;
			*((uint16_t*)flashOpStr.buf)=__t16;
			flashOpStr.op=emFLASH_WRITE_main_SYSTEM_DATA;		
			flashOpStr.len=2;
			*(int16_t*)uiAdjbuf=(mainSystemData.balanceOff/100);
			menu=0xb1;	
			break;
		case 0xb1:
			flashOpStr.flashAddr= \
				__get_offset(st_SystemData,balanceOff);		
			flashOpStr.len=2;	
			__t16=*((uint16_t*)uiAdjbuf);
			__t16*=100;
			*((uint16_t*)flashOpStr.buf)=__t16;
			flashOpStr.op=emFLASH_WRITE_main_SYSTEM_DATA;		
			flashOpStr.len=2;    
            *(int16_t*)uiAdjbuf=(mainSystemData.balanceWarning/100);
            menu=0xb0;	
			break;
	}
}

void ui_key_set_process()
{
	uint8_t ret=0;
	switch(menu){
        /*case 0xe0:
           
        case 0x10:
            //menu=bkupMenu;break;
        case 0x20:
            menu=bkupMenu;break;*/
		case 0x40://save flow line流量系数
			flashOpStr.flashAddr= \
				__get_offset(st_SystemData,flowRatio);		
			flashOpStr.op=emFLASH_WRITE_main_SYSTEM_DATA;		
			flashOpStr.len=2;	
			m_mem_cpy_len(flashOpStr.buf,(uint8_t*)uiAdjbuf ,2);
			menu=bkupMenu;break;
        case 0x50:
            flashOpStr.op=emFLASH_WRITE_main_CALIB_DATA;   
            *((uint16_t*)(flashOpStr.buf))=*((uint16_t*)uiAdjbuf);
            ret=data_api_calibration_table_data_save(calibLocation,false);
			//add 20170605
			if(!ret){
				break;
			}
			//
            calibLocation=0;
            menu=bkupMenu;break;
        case 0x60:
			flashOpStr.flashAddr= \
				__get_offset(st_SystemData,deviceID);		
			flashOpStr.len=4;	
            flashOpStr.buf[0]=uiAdjbuf[0];
            flashOpStr.buf[1]=uiAdjbuf[1];
            flashOpStr.buf[2]=0;
            flashOpStr.buf[3]=0;
            flashOpStr.op=emFLASH_WRITE_main_SYSTEM_DATA;
			menu=bkupMenu;
			break; 	         
		case 0x70:	//小信号切除
			flashOpStr.flashAddr= \
				__get_offset(st_SystemData,smallFlowNotCare);		
			flashOpStr.len=1;	
			flashOpStr.buf[0]=uiAdjbuf[0];
            flashOpStr.op=emFLASH_WRITE_main_SYSTEM_DATA;
			menu=bkupMenu;
			break; 	
		case 0x80:	//单价
			flashOpStr.flashAddr= \
				__get_offset(st_SystemData,price);		
			flashOpStr.len=2;	
			flashOpStr.buf[0]=uiAdjbuf[0];
			flashOpStr.buf[1]=uiAdjbuf[1];
            flashOpStr.op=emFLASH_WRITE_main_SYSTEM_DATA;
			menu=bkupMenu;
			break; 	
		case 0x90:	//传输限额
			flashOpStr.flashAddr= \
				__get_offset(st_SystemData,transferMoneyLimit);
			__t16=*((uint16_t*)uiAdjbuf);
			__t32=__t16*100;
			//*((uint32_t*)uiAdjbuf)=__t32;
            *((uint32_t*)(flashOpStr.buf))=__t32;
			flashOpStr.len=4;	
			//flashOpStr.buf[0]=uiAdjbuf[0];
			//flashOpStr.buf[1]=uiAdjbuf[1];
			//flashOpStr.buf[3]=uiAdjbuf[2];
			//flashOpStr.buf[4]=uiAdjbuf[4];			
            flashOpStr.op=emFLASH_WRITE_main_SYSTEM_DATA;
			menu=bkupMenu;
			break; 	
		
		case 0xa0:	//485地址	
			flashOpStr.flashAddr= \
				__get_offset(st_SystemData,shortID);	            
			flashOpStr.op=emFLASH_WRITE_main_SYSTEM_DATA;		
			flashOpStr.len=1;
			//m_mem_cpy_len(flashOpStr.buf,uiAdjbuf ,1);
            flashOpStr.buf[0]=uiAdjbuf[0];

			menu=bkupMenu;
			break;  
            /*
		case 0xb0:	//金额报警
			flashOpStr.flashAddr= \
				__get_offset(st_SystemData,balanceWarning);		
			flashOpStr.len=2;	
			__t16=*((uint16_t*)uiAdjbuf);
			__t16*=100;
			*((uint16_t*)flashOpStr.buf)=__t16;
			flashOpStr.op=emFLASH_WRITE_main_SYSTEM_DATA;		
			flashOpStr.len=2;
			*(int16_t*)uiAdjbuf=(mainSystemData.balanceWarning/100);

			menu=0xb1;
			break; 	
             * */
        case 0xb0:
			flashOpStr.flashAddr= \
				__get_offset(st_SystemData,balanceWarning);		
			flashOpStr.len=2;	
			__t16=*((uint16_t*)uiAdjbuf);
			__t16*=100;
			*((uint16_t*)flashOpStr.buf)=__t16;
			flashOpStr.op=emFLASH_WRITE_main_SYSTEM_DATA;		
			flashOpStr.len=2;
			*(int16_t*)uiAdjbuf=(mainSystemData.balanceOff/100);
			menu=bkupMenu;
            break;
		case 0xb1:	//金额报警
			flashOpStr.flashAddr= \
				__get_offset(st_SystemData,balanceOff);		
			flashOpStr.len=2;	
			__t16=*((uint16_t*)uiAdjbuf);
			__t16*=100;
			*((uint16_t*)flashOpStr.buf)=__t16;
			flashOpStr.op=emFLASH_WRITE_main_SYSTEM_DATA;		
			flashOpStr.len=2;			
			menu=bkupMenu;
			break; 
            /*
		case 0xc0: //加值
            
			flashOpStr.flashAddr= \
				__get_offset(st_SystemData,BalanceMoney);	
			flashOpStr.op=emFLASH_WRITE_main_SYSTEM_DATA;		
			flashOpStr.len=4;
            __q31=100;
			m_mem_cpy_len(flashOpStr.buf,(uint8_t*)(&__q31) ,4);

			menu=bkupMenu;
             
			break;
        case 0xf0:
 			flashOpStr.flashAddr= __get_offset(st_SystemData,flowRatio);
			flashOpStr.op=emFLASH_WRITE_main_SYSTEM_DATA;
			flashOpStr.len=2;
			//m_mem_set(flashOpStr.buf,0,2);
            __t16=1000;
            m_mem_cpy_len(flashOpStr.buf,(uint8_t*)(&__t16) ,2);
			menu=bkupMenu;
           
            
            break; 
             */ 
        case 0xf1:
 			flashOpStr.flashAddr= __get_offset(st_SystemData,imdFlowUnit);
			flashOpStr.op=emFLASH_WRITE_main_SYSTEM_DATA;
			flashOpStr.len=1;
            flashOpStr.buf[0]=uiAdjbuf[0];
			menu=bkupMenu;
            break;             
        default:
            menu=bkupMenu;break;
	}    

}
void ui_key_process(void)
{
    uint16_t t16;
    key_scan_call();
    if(KeyValue)noKeyEventTimeOut=30;
    switch (KeyValue) {
        case upKEY_DOWN: ui_key_process_up_down(true);
            break;
        case downKEY_DOWN: 
            if(menu<3){
                t16=ui_key_waite_up_ex(1000);
                if(t16>=1000)return;
            }
            if(!ui_key_process_down())
                ui_key_process_up_down(false);
            break;
        case setKEY_DOWN: //ui_key_process_set();
			__t16=ui_key_waite_up();
            if(__t16>=2000){
                if(KeyValue==(setKEY_DOWN+upKEY_DOWN)){
                    ui_key_process_enter();
                }else if(KeyValue==(setKEY_DOWN)){
                    ui_key_set_process();
                    
                }
                
            }else{
                ui_key_process_cancel();
                //KeyValue=setKEY_DOWN;
            }
            KeyValue=0x08;
            break;
        default:break;
    }
	//ui_key_waite_up();
    if (KeyValue){
        ui_disp_menu();
      ui_key_waite_up();
    }
    //ui_key_waite_up();
}
