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
	if(up){
		menu++;
		if(menu>2)menu=0;
	}else{
		if(menu==0)menu=2;
		else
			menu--;
	}
    bkupMenu=menu;
	
}
uint8_t  ui_key_process_down(void){
    switch(menu){
    case 0x20://clear total volumeResolution
        flashOpStr.flashAddr= \
            __get_offset(st_SystemData,BalanceMoney);
        flashOpStr.op=emFLASH_WRITE_main_SYSTEM_DATA;
        flashOpStr.len=4;
        m_mem_set(flashOpStr.buf,0,4);
        menu=bkupMenu; return 1;           
    case 0x30://clear total
        flashOpStr.flashAddr= \
            __get_offset(st_SystemData,totalConsumeVolume);
        flashOpStr.op=emFLASH_WRITE_main_SYSTEM_DATA;
        flashOpStr.len=4;
        m_mem_set(flashOpStr.buf,0,4);
        menu=bkupMenu;return 1;
	case 0xd0:
        flashOpStr.flashAddr= \
            __get_offset(st_SystemData,cardID);
        flashOpStr.op=emFLASH_WRITE_main_SYSTEM_DATA;
        flashOpStr.len=4;
        //m_mem_set(flashOpStr.buf,0,4);
		*((uint32_t*)(flashOpStr.buf))=0;
        menu=0xd1; return 1;  			
        default:
            return 0;
		
    }
            
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
			key_process_up_down_variable_speed_ex((int16_t*)uiAdjbuf,500, 1555, up);
			break;
        case 0x50:
			key_process_up_down_variable_speed_ex((int16_t*)uiAdjbuf,0, 3000, up);
			break;            
		case 0xa0:	//485地址
			key_process_up_down_variable_speed_ex((int16_t*)uiAdjbuf,0, 255, up);
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
 //#if cfg_KEY_FUCTION_WHOLE==1    
/* 
		case 0xc0: //加值
			key_process_up_down_variable_speed_ex((int16_t*)(uiAdjbuf),-1000, 1000, up);
			break;
            //0x50 --shft--0x51;
         
		case 0x51:	//save volumeUnit;			
		case 0x52:	//save currVolumeUnit	
		case 0x53:	//save imdFlowUnit
			//key_process_up_down_variable_speed_ex((int16_t*)(&__t16),0, 1, up);	
			if(uiAdjbuf[0])uiAdjbuf[0]=0;
			else 
				uiAdjbuf[0]=1;
			break;
		case 0x61:	//volumeResolution				
		case 0x62:	//currVolumeResolution						
		case 0x63:	//imdFlowResolution	
			key_process_up_down_variable_speed_ex((int16_t*)(&__t16),0, 3, up);
			break;
		case 0x81:	//报警方式1 d1=xx
		case 0x91:	//报警方式2 d2=xx
			key_process_up_down_variable_speed_ex((int16_t*)(&__t16),0, 2, up);
			break;
		case 0xa0:	//485地址
			key_process_up_down_variable_speed_ex((int16_t*)(&__t16),1, 253, up);
			break;
		case 0xe0:
			//__t16=uiAdjbuf[0];
			//key_process_up_down_variable_speed_ex((int16_t*)(&__t16),0, 3, up);
			//uiAdjbuf[0]=(uint8_t)__t16;
			key_process_up_down_variable_speed_ex((int16_t*)(&__t16),'A', 'C', up);
			break;	
		case 0x82:	//		
		case 0x83:			
		case 0x84:			
		case 0x85:
		case 0x92:	//			
		case 0x93:				
		case 0x94:			
		case 0x95:
			//m_mem_cpy_len((uint8_t*)(&__t32),uiAdjbuf,4);
			//__t16=(uint16_t)__t32;
			//key_process_up_down_variable_speed_ex((int16_t*)(&__t16),0, 5000, up);
			//__t32=__t16;
			//m_mem_cpy_len(uiAdjbuf,(uint8_t*)(&__t32),4);
			key_process_up_down_variable_speed_ex((int16_t*)(&__t16),0, 5000, up);
			break;		

		case 0xd1:
		case 0xd2:
		case 0xd3:
		case 0xd4:
			//m_mem_cpy_len((uint8_t*)(&__t32),uiAdjbuf,4);
			//__t16=(uint16_t)__t32;
			//key_process_up_down_variable_speed_ex((int16_t*)(&__t16),-1000, 3000, up);
			//__t32=__t16;
			//m_mem_cpy_len(uiAdjbuf,(uint8_t*)(&__t32),4);
			key_process_up_down_variable_speed_ex((int16_t*)(&__t16),-1000, 3000, up);

			break;		
//#endif
*/
	}
}

void ui_key_process_enter(void) {
	switch(menu ){
		case 0x10:            
			switch(password){
				case psw_CLEAR_VOLUME:
					ui_key_waite_up();
					menu=0x20;break;
				case psw_CLEAR_BALANCE:	
					ui_key_waite_up();
					menu=0x30;break;
				case psw_ADJUST_FLOW_LINE:
					m_mem_cpy_len(uiAdjbuf,(uint8_t*)(&mainSystemData.flowRatio),2);
					menu=0x40;
					break;
                case psw_SET_CALIB_TABLE:
                    calibLocation=0;
                    *((uint16_t*)uiAdjbuf) = Main_flowCalibrationTable.flowCalibrationArr[calibLocation].rtflowValue;
                    menu=0x50;break;
                case psw_FLOW_LINE_RESET:
                    menu=0xf0;break;
                    
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
				case psw_SET_balance_Warning_Off:
					*(int16_t*)uiAdjbuf=(mainSystemData.balanceWarning/100);
					menu=0xb0;
					break;

				case psw_SET_MODBUS_ADDR:			
					uiAdjbuf[0]=mainSystemData.shortID;
					menu=0xa0;break;                    
				case psw_TEST_RECHARGE:	
                    //__q15==(int16_t)rtBalanceMoney;
					//m_mem_cpy_len(uiAdjbuf,(uint8_t*)(&__q15),2);
					menu=0xc0;break;	
                case psw_CLEAR_CARD_ID:
                    menu=0xd0;break;											
        //#if cfg_KEY_FUCTION_WHOLE==1    
		/*
				case psw_SET_FLOW_UNIT:				menu=0x50;break;//设定单位
				case psw_SET_DEC_NUM:				menu=0x60;break;
				case psw_SET_SMALL_FLOW_HOW_CARE:
					uiAdjbuf[0]=mainSystemData.smallFlowNotCare;
					menu=0x70;
					break;					
				case psw_SET_imdFLOW_WARNING_0:		menu=0x80;break;
				case psw_SET_imdFLOW_WARNING_1:		menu=0x90;break;
				case psw_SET_CALIB_TABLE:			menu=0xb0;break;				
				case psw_SET_MONEY_SETTING:			menu=0xd0;break;
				case psw_SET_DCLS:					
					uiAdjbuf[0]=mainSystemData.dcls;
					menu=0xe0;break;
				case psw_CLEAR_ALL:					menu=0xf0;break;
				*/
//#endif
				default:break; 
			}
            break;
        // #if cfg_KEY_FUCTION_WHOLE==1     
		/*
		case 0x50:	//enter c0: 单位
			password=0x00;menu=0x51;break;
		case 0x60:	//enter c0: 
			password=0x00;menu=0x61;break;			
		case 0xd0:	//enter c0: 
			password=0x00;menu=0xd1;break;
		//case 0x70:没有子菜单
		case 0x80:	//save  warning solution 1		
            password=0x00;menu=0x81;
			break;
		case 0x90://save  warning solution 2
			password=0x00;menu=0x91;break;			
		case 0x51:	//save volumeUnit;
			flashOpStr.flashAddr= \
				__get_offset(st_SystemData,volumeUnit);					
		case 0x52:	//save currVolumeUnit
			flashOpStr.flashAddr= \
				__get_offset(st_SystemData,currVolumeUnit);		
		case 0x53:	//save imdFlowUnit
			flashOpStr.flashAddr= \
				__get_offset(st_SystemData,imdFlowUnit);					
		case 0x61:	//volumeResolution
			flashOpStr.flashAddr= \
				__get_offset(st_SystemData,volumeResolution);				
		case 0x62:	//currVolumeResolution
			flashOpStr.flashAddr= \
				__get_offset(st_SystemData,currVolumeResolution);						
		case 0x63:	//imdFlowResolution
			flashOpStr.flashAddr= \
				__get_offset(st_SystemData,imdFlowResolution);				
			
		case 0x81:	//报警方式1 d1=xx
			flashOpStr.flashAddr= \
				__get_offset(st_SystemData,warningSolution0);	
		case 0x91:	//报警方式2 d2=xx
			flashOpStr.flashAddr= \
				__get_offset(st_SystemData,warningSolution1);	

		case 0xe0:
			flashOpStr.flashAddr= \
				__get_offset(st_SystemData,dcls);
			
			flashOpStr.op=emFLASH_WRITE_main_SYSTEM_DATA;		
			flashOpStr.len=1;
			m_mem_cpy_len(flashOpStr.buf,uiAdjbuf ,1);
			password=0x00;
			menu=0x00;
			break;
			//上面这些是写入一个数据的调节菜单			
		case 0x82:	//	
			flashOpStr.flashAddr= \
				__get_offset(st_SystemData,imdFlowUpWarning0);		
		case 0x83:
			flashOpStr.flashAddr= \
				__get_offset(st_SystemData,imdFlowUpWarningDiff0);				
		case 0x84:
			flashOpStr.flashAddr= \
				__get_offset(st_SystemData,imdFlowLowWarning0);				
		case 0x85:
			flashOpStr.flashAddr= \
				__get_offset(st_SystemData,imdFlowLowWarningDiff0);				
			//break;
		case 0x92:	//	
			flashOpStr.flashAddr= \
				__get_offset(st_SystemData,imdFlowUpWarning0);		
		case 0x93:
			flashOpStr.flashAddr= \
				__get_offset(st_SystemData,imdFlowUpWarningDiff0);				
		case 0x94:
			flashOpStr.flashAddr= \
				__get_offset(st_SystemData,imdFlowLowWarning0);				
		case 0x95:
			flashOpStr.flashAddr= \
				__get_offset(st_SystemData,imdFlowLowWarningDiff0);	

		case 0xd1:
			flashOpStr.flashAddr= \
				__get_offset(st_SystemData,balanceWarning);	
		case 0xd2:
			flashOpStr.flashAddr= \
				__get_offset(st_SystemData,balanceOff);
		case 0xd3:
			flashOpStr.flashAddr= \
				__get_offset(st_SystemData,transferMoneyLimit);
		case 0xd4:
			flashOpStr.flashAddr= \
				__get_offset(st_SystemData,maxBalanceLimit);
		
			flashOpStr.op=emFLASH_WRITE_main_SYSTEM_DATA;		
			flashOpStr.len=4;
			m_mem_cpy_len(flashOpStr.buf,uiAdjbuf ,4);
			password=0x00;
			menu=0x00;
		break;
		case 0xb0://save 标定数据
			password=0x00;menu=0xb1;break;
    #endif
    */
	}
}

void ui_key_process_cancel(void){    
    if(menu<3){
        uiAdjbuf[0]=0;
        uiAdjbuf[1]=0;
        bkupMenu=menu;
        password=0x00;
        menu=0x10;
    }else if(menu==0x50){
        if(*((uint16_t*)uiAdjbuf) != Main_flowCalibrationTable.flowCalibrationArr[calibLocation].rtflowValue){
			flashOpStr.op=emFLASH_WRITE_main_CALIB_DATA;   
            *((uint16_t*)(flashOpStr.buf))=*((uint16_t*)uiAdjbuf);
            data_api_calibration_table_data_save(calibLocation,true);
         }
			//menu=0x00;
        calibLocation++;
        if(calibLocation>=FLOW_CALIBRATION_POINT_NUM)calibLocation=0;
        *(uint16_t*)uiAdjbuf = Main_flowCalibrationTable.flowCalibrationArr[calibLocation].rtflowValue;       
    }
    else{
        menu=bkupMenu;
        password=0x00;
    }	
}
/*
void ui_key_process_shfit(void){
    #if cfg_KEY_FUCTION_WHOLE==1
	switch(menu ){
		case 0x50:
			uiAdjbuf[0]=mainSystemData.volumeUnit;
			menu=0x51;break;
		case 0x51:	
			uiAdjbuf[0]=mainSystemData.currVolumeUnit;
			menu=0x52;break;
		case 0x52:
			uiAdjbuf[0]=mainSystemData.imdFlowUnit;
			menu=0x53;break;
		case 0x53:
			menu=0x50;
			break;
		case 0x60:
			uiAdjbuf[0]=mainSystemData.volumeResolution;
			menu=0x61;break;
		case 0x61:
			uiAdjbuf[0]=mainSystemData.currVolumeResolution;
			menu=0x62;break;
		case 0x62:
			uiAdjbuf[0]=mainSystemData.imdFlowResolution;
			menu=0x63;break;
		case 0x63:
			menu=0x60;
		case 0x80:
			uiAdjbuf[0]=mainSystemData.warningSolution0;
			menu=0x81;break;
		case 0x81:
			__t16=(uint16_t)(mainSystemData.imdFlowUpWarning0);
			m_mem_cpy_len(uiAdjbuf,(uint8_t*)(&__t16),2);
			menu=0x82;break;
		case 0x82:
			__t16=(uint16_t)(mainSystemData.imdFlowUpWarningDiff0);
			m_mem_cpy_len(uiAdjbuf,(uint8_t*)(&__t16),2);
			menu=0x83;break;		
		case 0x83:
			__t16=(uint16_t)(mainSystemData.imdFlowLowWarning0);
			m_mem_cpy_len(uiAdjbuf,(uint8_t*)(&__t16),2);
			menu=0x84;break;	
		case 0x84:
			__t16=(uint16_t)(mainSystemData.imdFlowLowWarningDiff0);
			m_mem_cpy_len(uiAdjbuf,(uint8_t*)(&__t16),2);
			menu=0x85;break;
		case 0x85:
			menu=0x80;break;
			//
		case 0x90:
			uiAdjbuf[0]=mainSystemData.warningSolution1;
			menu=0x91;break;
		case 0x91:
			__t16=(uint16_t)(mainSystemData.imdFlowUpWarning1);
			m_mem_cpy_len(uiAdjbuf,(uint8_t*)(&__t16),2);
			menu=0x92;break;
		case 0x92:
			__t16=(uint16_t)(mainSystemData.imdFlowUpWarningDiff1);
			m_mem_cpy_len(uiAdjbuf,(uint8_t*)(&__t16),2);
			menu=0x93;break;		
		case 0x93:
			__t16=(uint16_t)(mainSystemData.imdFlowLowWarning1);
			m_mem_cpy_len(uiAdjbuf,(uint8_t*)(&__t16),2);
			menu=0x94;break;	
		case 0x94:
			__t16=(uint16_t)(mainSystemData.imdFlowLowWarningDiff1);
			m_mem_cpy_len(uiAdjbuf,(uint8_t*)(&__t16),2);
			menu=0x95;break;
		case 0x95:
			menu=0x90;break;	
		//
		case 0xd0:
			__q15=(int16_t)(mainSystemData.balanceWarning);
			m_mem_cpy_len(uiAdjbuf,(uint8_t*)(&__q15),2);
			menu=0xd1;break;				
		case 0xd1:
			__q15=(int16_t)(mainSystemData.balanceOff);
			m_mem_cpy_len(uiAdjbuf,(uint8_t*)(&__q15),2);
			menu=0xd2;break;				
		case 0xd2:
			__q15=(int16_t)(mainSystemData.transferMoneyLimit);
			m_mem_cpy_len(uiAdjbuf,(uint8_t*)(&__q15),2);
			menu=0xd3;break;	
		case 0xd3:
			__q15=(int16_t)(mainSystemData.maxBalanceLimit);
			m_mem_cpy_len(uiAdjbuf,(uint8_t*)(&__q15),2);
			menu=0xd4;break;	
		case 0xd4:
			menu=0xd0;break;
	}
#endif
}
*/
void ui_key_set_process()
{
	switch(menu){
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
            data_api_calibration_table_data_save(calibLocation,false);
            calibLocation=0;
            menu=bkupMenu;break;
            
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
			*((uint32_t*)uiAdjbuf)=__t32;
			flashOpStr.len=4;	
			flashOpStr.buf[0]=uiAdjbuf[0];
			flashOpStr.buf[1]=uiAdjbuf[1];
			flashOpStr.buf[3]=uiAdjbuf[2];
			flashOpStr.buf[4]=uiAdjbuf[4];			
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
 			flashOpStr.flashAddr= \
				__get_offset(st_SystemData,flowRatio);
			flashOpStr.op=emFLASH_WRITE_main_SYSTEM_DATA;
			flashOpStr.len=2;
			//m_mem_set(flashOpStr.buf,0,2);
            __t16=1000;
            m_mem_cpy_len(flashOpStr.buf,(uint8_t*)(&__t16) ,2);
			menu=bkupMenu;break; 			    
	}    

}
void ui_key_process(void){
    key_scan_call();
    if(KeyValue)noKeyEventTimeOut=30;
    switch (KeyValue) {
        case upKEY_DOWN: ui_key_process_up_down(true);
            break;
        case downKEY_DOWN: 
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
