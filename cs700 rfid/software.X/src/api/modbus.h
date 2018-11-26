#ifndef __modbus_h__
#define __modbus_h__
#include <stdint.h>

#ifdef __cplusplus
extern "C"{
#endif
	#define 	FUNC_READ_COILS						0x01
	#define 	FUNC_READ_DISCRETE_INPUTS			0x02
	#define 	FUNC_READ_HOLDING_REGISTERS			0x03
	#define 	FUNC_READ_INPUT_REGISTERS			0x04
	#define 	FUNC_WRITE_SINGLE_COIL				0x05
	#define 	FUNC_WRITE_SINGLE_REGISTER			0x06
	#define 	FUNC_WRITE_MULTIPLE_COILS			0x0F
	#define 	FUNC_WRITE_MULTIPLE_REGISTERS		0x10
	#define 	FUNC_READ_FILE_RECORD				0x14
	#define 	FUNC_WRITE_FILE_RECORD				0x15
	#define 	FUNC_MASK_WRITE_REGISTER			0x16
	#define 	FUNC_READWRITE_MULTIPLE_REGISTERS	0x17
	#define 	FUNC_READ_DEVICE_IDENTIFICATION		0x2B

	#define		FUNC_WRITE_FLASH_SYSDATA			100
	#define 	FUNC_WRITE_FLASH_CLIBDATA			101
    #define		FUNC_WRITE_FLASH_CLIBDATA_COVER		110
	#define 	FUNC_READ_FLASH_SYSDATA    			102
	#define 	FUNC_READ_FLASH_CLIBDATA    		103
	#define     FUNC_READ_STATUS_REGISTER           104	
	#define 	FUNC_WRITE_STATUS_REGISTER 			105
	

	#define 	MODBUS_ADDR_OFFSET					0
	#define 	MODBUS_FUNC_OFFSET					1
	#define 	MODBUS_SIZE_OFFSET					2
	#define 	MODBUS_DATA_OFFSET					4
	typedef struct{
		uint8_t 	addr;
		uint8_t		func;
		uint8_t		addr_hi;
		uint8_t 	addr_lo;
		uint8_t		len_hi;
		uint8_t		len_lo;
	}st_modbusComReqStructDef;
	typedef struct{
		uint8_t 	addr;
		uint8_t		func;
		uint8_t		errcord;
		uint8_t		crc16;	
	}st_modbusErrRespStructDef;	
	typedef struct{
		uint8_t 	addr;
		uint8_t		func;
		uint8_t		addr_hi;
		uint8_t 	addr_lo;
		uint8_t		len_hi;
		uint8_t		len_lo;
		uint16_t	crc16;
	}st_modbusReadClib;
	typedef struct{
		uint8_t 	addr;
		uint8_t		func;
		uint8_t		addr_hi;
		uint8_t 	addr_lo;
		uint8_t		data_hi;
		uint8_t		data_lo;
		uint16_t	crc16;
	}st_modbusWriteSingleClib;		
	
	/* MODBUS Exception Codes */
	#define	ILLEGAL_FUNCTION					0x01
	#define	ILLEGAL_DATA_ADDRESS				0x02
	#define	ILLEGAL_DATA_VALUE					0x03
    //#define ILLEGAL_DATA_VALUE
	
    extern void modbus_response_process(uint8_t* rbuf,uint16_t rlen);
#ifdef __cplusplus
}
#endif
#endif 
/* !_modbus_h_ */

