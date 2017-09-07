#include "../soc/soc.h"
#include "modbus.h"
#include "../includes/includes.h"

#define __mbStatusStartAddr__ 0xe000
void modbus_response_illgeal_function(uint8_t* buf,uint8_t errcode)
{
	st_modbusErrRespStructDef* pmdbs=(st_modbusErrRespStructDef*)buf;
	if(pmdbs->addr==0x00)return;
	pmdbs->addr=mainSystemData.shortID;
	if(errcode){
		pmdbs->func+=0x80;
	}
	pmdbs->errcord=errcode;
	crc_append(buf,3);
	my_uart1_send_poll(buf,5);		
}
void modbus_response_write_flash_sys(uint8_t* buf){
	uint16_t startAddr=0;
	uint8_t len=0;
	st_modbusComReqStructDef* pmdbs=(st_modbusComReqStructDef*)buf;
	startAddr=pmdbs->addr_hi;
	startAddr<<=8;
	startAddr |= pmdbs->addr_lo;
	len=pmdbs->len_hi;
	len<<=8;
	len = pmdbs->len_lo;
	if(startAddr< main_FLOW_CALIBRATION_TABLE_ADDR || startAddr+len > END_FLASH || \
	len >16){
		modbus_response_illgeal_function(buf,ILLEGAL_DATA_ADDRESS);
		return;
	}
	m_mem_cpy_len(comBuffer,buf+5,len);
	flashOpStr.flashAddr=startAddr;
	flashOpStr.op=emFLASH_WRITE_main_SYSTEM_DATA;		
	flashOpStr.len=len;
	m_mem_cpy_len(comBuffer,buf+5,len);
	data_api_flash_process();
	modbus_response_illgeal_function(buf,0);
	
}
void modbus_response_write_flash_clib(uint8_t* buf){	
	uint16_t startAddr=0;
	uint8_t len=0;
	st_modbusComReqStructDef* pmdbs=(st_modbusComReqStructDef*)buf;
	startAddr=pmdbs->addr_hi;
	startAddr<<=8;
	startAddr |= pmdbs->addr_lo;
	len=pmdbs->len_hi;
	len<<=8;
	len = pmdbs->len_lo;
	if(startAddr< main_FLOW_CALIBRATION_TABLE_ADDR || startAddr+len > END_FLASH || \
	len >16){
		modbus_response_illgeal_function(buf,ILLEGAL_DATA_ADDRESS);
		return;
	}
	m_mem_cpy_len(comBuffer,buf+5,len);
	flashOpStr.flashAddr=startAddr;
	flashOpStr.op=emFLASH_WRITE_main_CALIB_DATA;		
	flashOpStr.len=len;
	m_mem_cpy_len(comBuffer,buf+5,len);
	data_api_flash_process();
	modbus_response_illgeal_function(buf,0);	
}
void modbus_response_read_flash_data(uint8_t* buf)
{
	uint16_t startAddr=0;
	uint8_t len=0;
	st_modbusComReqStructDef* pmdbs=(st_modbusComReqStructDef*)buf;
	startAddr=pmdbs->addr_hi;
	startAddr<<=8;
	startAddr |= pmdbs->addr_lo;
	len=pmdbs->len_hi;
	len<<=8;
	len = pmdbs->len_lo;
	if(startAddr< main_FLOW_CALIBRATION_TABLE_ADDR || startAddr+len > END_FLASH || \
	len >64){
		modbus_response_illgeal_function(buf,ILLEGAL_DATA_ADDRESS);
		return;
	}
    m_mem_cpy_len(buf+5,(uint8_t*)startAddr,len);
    len+=5;
    crc_append(buf,len);
    len+=2;
    my_uart1_send_poll(buf,len);    
}
void modbus_response_read_status_register(uint8_t* buf)
{
	uint16_t startAddr=0;
	uint8_t len=0;
	st_modbusComReqStructDef* pmdbs=(st_modbusComReqStructDef*)buf;
	startAddr=pmdbs->addr_hi;
	startAddr<<=8;
	startAddr |= pmdbs->addr_lo;
	len=pmdbs->len_hi;
	len<<=8;
	len = pmdbs->len_lo;
	switch(startAddr){
		case (__mbStatusStartAddr__):
            if(len!=4)break;
			m_mem_cpy_len(buf+5,(uint8_t*)(&rtBalanceMoney),4);
			len+=5;
			crc_append(buf,len);
			len+=2;
			my_uart1_send_poll(buf,len);
			return;
        case (__mbStatusStartAddr__+5):
            if(len!=1)break;
            buf[5]=deviceStatus.byte;
			len+=5;
			crc_append(buf,len);
			len+=2;
			my_uart1_send_poll(buf,len);
			return;        
		default:
			break;
	}
    modbus_response_illgeal_function(buf,ILLEGAL_DATA_ADDRESS);
}
void modbus_response_write_status_register(uint8_t* buf){
	uint16_t startAddr=0;
	uint8_t len=0;
	st_modbusComReqStructDef* pmdbs=(st_modbusComReqStructDef*)buf;
	startAddr=pmdbs->addr_hi;
	startAddr<<=8;
	startAddr |= pmdbs->addr_lo;
	len=pmdbs->len_hi;
	len<<=8;
	len = pmdbs->len_lo;    
    if(startAddr != __mbStatusStartAddr__+5 || len!=1 ){
         modbus_response_illgeal_function(buf,ILLEGAL_DATA_ADDRESS);
         return;
    }
    deviceStatus.byte=buf[5];
    modbus_response_illgeal_function(buf,0);
}

void modbus_response_process(uint8_t* rbuf,uint16_t rlen){
	uint8_t resp=0;
	st_modbusComReqStructDef* pmdbs=(st_modbusComReqStructDef*)rbuf;
	if(!(pmdbs->addr ==0xff  || pmdbs->addr==mainSystemData.shortID || pmdbs->addr!=0x00))return;
	if(!(crc_verify(rbuf,rlen-2)))return ;
	//function expand expand
	switch(pmdbs->func){
		case FUNC_WRITE_FLASH_SYSDATA:
			modbus_response_write_flash_sys(rbuf);
			break;		
		case FUNC_WRITE_FLASH_CLIBDATA:
			modbus_response_write_flash_clib(rbuf);
			break;
		case FUNC_READ_FLASH_SYSDATA:
		case FUNC_READ_FLASH_CLIBDATA:
			modbus_response_read_flash_data(rbuf);   
			break;			
		case FUNC_READ_STATUS_REGISTER:
			modbus_response_read_status_register(rbuf);  
			break;
		case FUNC_WRITE_STATUS_REGISTER:
			modbus_response_write_status_register(rbuf);  
			break;           
		default:
			modbus_response_illgeal_function(rbuf,ILLEGAL_FUNCTION);break;		
	}	
}