#include "../soc/soc.h"
#include "../includes/includes.h"
#include <string.h>


st_flashOpStr flashOpStr = {0};
uint32_t __t32;
int32_t __q31;
uint16_t __t16;
int16_t __q15;
uint8_t __t8;
int8_t __q7;
uint8_t flowCalibrationLocation=0;
bool flowCalibrationPointSingle=true;
st_flowCalibrationTable Main_flowCalibrationTable ={0};

st_SystemData mainSystemData ={
	0,//uint32_t 	deviceID;				//??id?????485??????		
	
	1000,//int16_t		flowRatio;	
	0,//uint8_t		shortID;                //??id??id?modbus?
	0,//uint8_t     menuBackUp;
	
	0,//uint8_t		volumeUnit;				//????
	1,//uint8_t		volumeResolution;		//?????	
	0,//uint8_t		imdFlowUnit;		
	1,//uint8_t		imdFlowResolution;		//???????
	
	
	0,//int32_t		totalConsumeVolume;		//??
	
	2,//uint8_t		smallFlowNotCare;			
	{0},//st_deviceStatusStructDef	status;	
	{0},//uint8_t		reverse[2];
	0,//uint16_t	crc16;	
};

uint8_t comBuffer[COM_BUFFER_SIZE];
st_deviceStatusStructDef deviceStatus={0};
void data_api_system_data_get(void) {
    deviceStatus.byte=mainSystemData.status.byte;
    //rtBalanceMoney=mainSystemData.BalanceMoney;
}

void data_api_write_eeprom(uint8_t* buf,uint8_t addr,uint8_t len){
    uint8_t i;
    for(i=0;i<len;i++){
        DATAEE_WriteByte(addr,*buf);
        buf++;
        addr++;
    }
}
void data_api_write_eeprom_clr(void){
    uint16_t i;
    for(i=0;i<256;i++){
        DATAEE_WriteByte((uint8_t)i,0);
        //buf++;
        //addr++;
    }
}
void data_api_read_eeprom(uint8_t* buf,uint8_t addr,uint8_t len){
    uint8_t i;
    for(i=0;i<len;i++){
        *buf=DATAEE_ReadByte(addr);
        buf++;
        addr++;
    }
}
uint8_t data_api_calibration_table_data_init(void) {
    uint8_t i,sta;
    st_flowCalibrationTable* stp=( st_flowCalibrationTable*)comBuffer;
	data_api_read_eeprom(comBuffer,addr_CALIB_DATA_ADDR_EEPROM,sizeof(st_flowCalibrationTable));
	sta=crc_verify(comBuffer,sizeof (st_flowCalibrationTable));
	if(!sta){
		for(i=0;i<FLOW_CALIBRATION_POINT_NUM;i++){
			stp->flowCalibrationArr[i].adcValue=2000*(i+1);
			stp->flowCalibrationArr[i].rtflowValue=100*i;
		}
		stp->maxFlow=stp->flowCalibrationArr[i-1].rtflowValue;
		crc_append(comBuffer,sizeof (st_flowCalibrationTable)-2);			
		data_api_write_eeprom(comBuffer,addr_CALIB_DATA_ADDR_EEPROM,sizeof(st_flowCalibrationTable));		
	}
	m_mem_cpy_len((uint8_t*)(&Main_flowCalibrationTable),comBuffer,sizeof (st_flowCalibrationTable));
	sta=crc_verify((uint8_t*)(&Main_flowCalibrationTable), sizeof (st_flowCalibrationTable) );
	return sta;	
    /*
	sta=crc_verify((uint8_t*)(&Main_flowCalibrationTable),sizeof (st_flowCalibrationTable));
    if(!sta){
		data_api_read_eeprom(comBuffer,addr_CALIB_DATA_ADDR_EEPROM,sizeof(st_flowCalibrationTable));
       sta=crc_verify(comBuffer,sizeof (st_flowCalibrationTable));
		if(!sta){
			for(i=0;i<FLOW_CALIBRATION_POINT_NUM;i++){
				stp->flowCalibrationArr[i].adcValue=2000*(i+1);
				stp->flowCalibrationArr[i].rtflowValue=100*i;
			}
            stp->maxFlow=stp->flowCalibrationArr[i-1].rtflowValue;
			crc_append(comBuffer,sizeof (st_flowCalibrationTable)-2);			
			data_api_write_eeprom(comBuffer,addr_CALIB_DATA_ADDR_EEPROM,sizeof(st_flowCalibrationTable));
		}
		m_flash_erase(main_FLOW_CALIBRATION_TABLE_ADDR, sizeof (st_SystemData));
        m_flash_copy(main_FLOW_CALIBRATION_TABLE_ADDR,comBuffer,sizeof(st_flowCalibrationTable));	
       sta=crc_verify((uint8_t*)(&Main_flowCalibrationTable), sizeof (st_flowCalibrationTable) );
	}    
	
    return sta;
     */
}
uint8_t data_api_system_data_init(void) {
    uint8_t sta = 0;
	data_api_read_eeprom(comBuffer,addr_SYSTEM_DATA_ADDR_EEPROM,sizeof (st_SystemData));
	sta=crc_verify(comBuffer,sizeof (st_SystemData));
	if(!sta){
		crc_append((uint8_t*)(&mainSystemData),sizeof (st_SystemData)-2); 
		data_api_write_eeprom((uint8_t*)(&mainSystemData),addr_SYSTEM_DATA_ADDR_EEPROM,sizeof(st_SystemData));
		//return sta; 
	}else{
		m_mem_cpy_len((uint8_t*)(&mainSystemData),comBuffer,sizeof (st_SystemData));
	}
	data_api_system_data_get();
	return sta;
	/*
    sta=crc_verify((uint8_t*)(&mainSystemData),sizeof (st_SystemData));
    if (!sta){
        //sta=crc_verify((uint8_t*)(&mainSystemData),sizeof (st_SystemData));
        if(!sta){
            m_mem_cpy_len(comBuffer,(uint8_t*)(&bkupSystemData),sizeof (st_SystemData)-2);
            crc_append(comBuffer,sizeof (st_SystemData)-2); 
			data_api_write_eeprom(comBuffer,addr_SYSTEM_DATA_ADDR_EEPROM,sizeof(st_SystemData));
        }
        m_flash_erase(main_SYSTEM_DATA_START_ADDR, sizeof (st_SystemData));
        m_flash_copy(main_SYSTEM_DATA_START_ADDR,comBuffer, sizeof (st_SystemData));
        sta=crc_verify((uint8_t*)(&mainSystemData),sizeof (st_SystemData));
	}
	
	data_api_system_data_get();
	return sta;
	*/
}
uint8_t data_api_rt_data_save(void) {
    st_SystemData* stp=(st_SystemData*)comBuffer;
    m_mem_cpy_len(comBuffer,(uint8_t*)(&mainSystemData),sizeof(st_SystemData));
    //__t32=
    stp->totalConsumeVolume+=rtVolume_m3;
    //stp->BalanceMoney=rtBalanceMoney;
    stp->status.byte=deviceStatus.byte;
    if(menu<=2){
        stp->menuBackUp=menu;
    }else{
        stp->menuBackUp=bkupMenu;
    }
    if(flashOpStr.op == emFLASH_WRITE_main_SYSTEM_DATA){
        flashOpStr.op=enFLASH_NOTHING;
        __t16=flashOpStr.flashAddr;	
		m_mem_cpy_len(comBuffer+__t16,flashOpStr.buf,flashOpStr.len);
        //flashOpStr.len=0;
    }   
    //m_mem_cpy_len(comBuffer+__t16,flashOpStr.buf,flashOpStr.len);
    crc_append(comBuffer,sizeof(st_SystemData)-2);
    m_mem_cpy_len((uint8_t*)(&mainSystemData),comBuffer,sizeof(st_SystemData));
	/*
    m_flash_copy(main_SYSTEM_DATA_START_ADDR, 
        comBuffer, sizeof (st_SystemData));
	*/
	data_api_write_eeprom(comBuffer,addr_SYSTEM_DATA_ADDR_EEPROM,sizeof(st_SystemData));
	
	//__t8=crc_verify((uint8_t*)(&mainSystemData),sizeof (st_SystemData));
	data_api_system_data_get();
    rtVolume_m3=0;
    //rtBalanceMoney=0;
	if(!__t8){
		return 0;
	}
    return 1;
}
/*
uint8_t data_api_calibration_table_data_save(uint8_t loc,bool single){
    uint8_t i;
    st_flowCalibrationTable* stp=( st_flowCalibrationTable*)comBuffer;
    m_mem_cpy_len(comBuffer,(uint8_t*)(&Main_flowCalibrationTable),sizeof (st_flowCalibrationTable));
    stp->flowCalibrationArr[loc].adcValue=adcAverageValue;
     stp->flowCalibrationArr[loc].rtflowValue=*((uint16_t*)(flashOpStr.buf));
     m_mem_cpy_len(flashOpStr.buf,comBuffer+loc*sizeof(st_CalibrationPoint),4);
    if(!single && loc>0){
        stp->maxFlow=stp->flowCalibrationArr[loc].rtflowValue;
        for(i=loc+1;i<FLOW_CALIBRATION_POINT_NUM;i++){
            m_mem_cpy_len(comBuffer+i*sizeof(st_CalibrationPoint),flashOpStr.buf,4);
        }
    }
    crc_append(comBuffer,sizeof (st_flowCalibrationTable)-2);
    //m_flash_copy(main_FLOW_CALIBRATION_TABLE_ADDR,comBuffer, sizeof (st_flowCalibrationTable));  
	data_api_write_eeprom(comBuffer,addr_CALIB_DATA_ADDR_EEPROM,sizeof(st_flowCalibrationTable));
    m_mem_cpy_len((uint8_t*)(&Main_flowCalibrationTable),comBuffer,sizeof (st_flowCalibrationTable));

	i=crc_verify((uint8_t*)(&Main_flowCalibrationTable), sizeof (st_flowCalibrationTable) );
    flashOpStr.op = enFLASH_NOTHING;
    return i;
}
*/
uint8_t data_api_calibration_table_data_save(uint8_t loc,bool single){
    uint8_t i;
	st_flowCalibrationTable* stp=( st_flowCalibrationTable*)comBuffer;
	m_mem_cpy_len(comBuffer,(uint8_t*)(&Main_flowCalibrationTable),sizeof (st_flowCalibrationTable));
	stp->flowCalibrationArr[loc].adcValue=adcAverageValue;
	stp->flowCalibrationArr[loc].rtflowValue=*((uint16_t*)(flashOpStr.buf));
	m_mem_cpy_len(flashOpStr.buf,comBuffer+loc*sizeof(st_CalibrationPoint),4);
     //add at 20170605
	if(loc>1){
		for(i=1;i<=loc;i++){
			//if((stp->flowCalibrationArr[i].adcValue < stp->flowCalibrationArr[i-1].adcValue) ||
			//( stp->flowCalibrationArr[i].rtflowValue <  stp->flowCalibrationArr[i-1].rtflowValue)){
            if(stp->flowCalibrationArr[i].rtflowValue <  stp->flowCalibrationArr[i-1].rtflowValue){
				flashOpStr.op = enFLASH_NOTHING;
				return 0;
			}
		}
	}
	if(loc>1){
		for(i=0;i<loc;i++){
			//if((stp->flowCalibrationArr[i].adcValue > stp->flowCalibrationArr[i+1].adcValue) ||
			//( stp->flowCalibrationArr[i].rtflowValue >  stp->flowCalibrationArr[i+1].rtflowValue)){
				//flashOpStr.op = enFLASH_NOTHING;
				//return 0;
            if(( stp->flowCalibrationArr[i].rtflowValue >  stp->flowCalibrationArr[i+1].rtflowValue)){
				single=false;
			}
		}
	}	
     //
    if(!single && loc>0){
        stp->maxFlow=stp->flowCalibrationArr[loc].rtflowValue;
        for(i=loc+1;i<FLOW_CALIBRATION_POINT_NUM;i++){
            m_mem_cpy_len(comBuffer+i*sizeof(st_CalibrationPoint),flashOpStr.buf,4);
        }
    }
    crc_append(comBuffer,sizeof (st_flowCalibrationTable)-2);
    //m_flash_copy(main_FLOW_CALIBRATION_TABLE_ADDR,comBuffer, sizeof (st_flowCalibrationTable));  
	data_api_write_eeprom(comBuffer,addr_CALIB_DATA_ADDR_EEPROM,sizeof(st_flowCalibrationTable));
    m_mem_cpy_len((uint8_t*)(&Main_flowCalibrationTable),comBuffer,sizeof (st_flowCalibrationTable));

	i=crc_verify((uint8_t*)(&Main_flowCalibrationTable), sizeof (st_flowCalibrationTable) );
    flashOpStr.op = enFLASH_NOTHING;
    return 1;
}
uint8_t data_api_flash_process(void) {
    //if (flashOpStr.op == enFLASH_NOTHING)return 0;
     if (flashOpStr.op == emFLASH_WRITE_main_CALIB_DATA) {
        data_api_calibration_table_data_save(flowCalibrationLocation,flowCalibrationPointSingle);
        flashOpStr.op=enFLASH_NOTHING;
    }
    if (flashOpStr.op == emFLASH_WRITE_main_SYSTEM_DATA) {
        
		data_api_rt_data_save();
        flashOpStr.len=0;
    }
    return 1;
}

