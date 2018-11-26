#ifndef __appdata_h__
#define __appdata_h__

#include <stdint.h>

#define get_offset_addr_of_str(TYPE, MEMBER) ((uint16_t) (&((TYPE *)0)->MEMBER))
#define __get_offset(TYPE, MEMBER)  get_offset_addr_of_str(TYPE, MEMBER)
#ifdef __cplusplus
extern "C"{
#endif

extern uint32_t __t32;
extern int32_t __q31;

extern uint16_t __t16;
extern int16_t __q15;

extern uint8_t __t8;
extern int8_t __q7;

typedef enum{
    OP_REASON_NONE,
    OFF_REASON_LA,
    OFF_REASON_MAN,
	ON_REASON_HA,
	ON_REASON_MAN,
	ON_REASON_STAR
}opReason_t;

	volatile opReason_t opReason;
    #define addr_SYSTEM_DATA_ADDR_EEPROM 0x00
    #define addr_CALIB_DATA_ADDR_EEPROM 0x80

    #define COM_BUFFER_SIZE 128
    extern uint8_t comBuffer[COM_BUFFER_SIZE];
    //extern uint8_t flash
	#define FLOW_CALIBRATION_POINT_NUM 30
	//#define main_FLOW_CALIBRATION_TABLE_ADDR (0x4000-0x80)
    typedef struct
    {
        uint16_t adcValue;
        uint16_t rtflowValue;
    }st_CalibrationPoint;
	typedef struct{
		st_CalibrationPoint flowCalibrationArr[FLOW_CALIBRATION_POINT_NUM];
		uint16_t maxFlow;
		uint16_t crc16;
	}st_flowCalibrationTable;
	//#define FLOW_CALIBRATION_TABLE_SIZE (sizeof(st_flowCalibrationTable))
	
	extern  st_flowCalibrationTable Main_flowCalibrationTable;
    //extern eeprom st_flowCalibrationTable Main_flowCalibrationTable;
	//extern const st_flowCalibrationTable Bkup_flowCalibrationTable;
    //flash ???????
    typedef enum
    {
        enFLASH_NOTHING,
        emFLASH_WRITE_main_CALIB_DATA,
        emFLASH_WRITE_main_SYSTEM_DATA,    
    }em_flashOperation;
    typedef struct
    {
        uint16_t flashAddr;
        uint16_t len;
        uint8_t buf[16];
        em_flashOperation op;
    }st_flashOpStr;    
    extern st_flashOpStr flashOpStr;
    
	
	typedef union{
		uint8_t byte;
		struct{
			uint8_t flowUpWarnging0:1;
			uint8_t flowDownWarnging0:1;
			uint8_t flowUpWarnging1:1;
			uint8_t flowDownWarnging1:1;
			uint8_t moneyLowWarning:1;
			uint8_t reverse:3;
		}bits;
	}st_warningStatus;
    extern st_warningStatus warningStatus;
    //
 	typedef union{
		uint8_t byte;
		struct{
			uint8_t flowWarnging0:1;
            uint8_t noEnough:1;
			uint8_t doorOpen:1;
			uint8_t acPowerDown:1;
            
			uint8_t valveOpened:1;
			uint8_t hardDefaults:1;
			uint8_t reverse:2;
		}bits;
	}st_deviceStatusStructDef;   
    extern st_deviceStatusStructDef deviceStatus;
	typedef enum {
        OFF_REASON_NONE,
		OFF_REASON_OVERDRAFT,
		OFF_REASON_SEVER,
	}em_offReason;
	extern em_offReason offReason;
		
typedef struct{
	int32_t		imdFlowUpWarning;	
	int32_t		imdFlowUpWarningDiff;
	int32_t		imdFlowLowWarning;	
	int32_t		imdFlowLowWarningDiff;		
}st_warningSetting;
typedef struct{
	uint32_t 	deviceID;				//??id?????485??????		
	uint32_t	cardID;					//?id		
	uint8_t		dcls;					//life cycle
	uint8_t		shortID;                //??id??id?modbus?
	uint16_t	companyCode;			//????????
	int32_t 	transferMoney;			//????
	//0x10
	uint16_t	transferTime;			//????
	int16_t		price;					//??
	int16_t		balanceWarning;			//???????	
	int16_t		balanceOff;				//?????	
	int16_t		BalanceVolume;			//??????????????????????????
	int16_t		flowRatio;		
	int32_t 	payMoney;				//????
	//0x20
	uint16_t 	payTimes;				//????
    uint8_t     menuBackUp;
	uint8_t     opReasen;               //uint8_t		reverse04[1];           //??	
	int32_t 	transferMoneyLimit;		//????????
	int32_t		maxBalanceLimit;		//????	
	int32_t		BalanceMoney;			//??
	//0x30
	int32_t		totalConsumeVolume;		//??
	int32_t		totalConsumeMoney;		//????????????????????	
	uint8_t		keyIndex;				//?????
	uint8_t		smallFlowNotCare;		//?????		
	uint8_t		volumeUnit;				//????
	uint8_t		volumeResolution;		//?????

	uint8_t		imdFlowUnit;			//??????	
	uint8_t		imdFlowResolution;		//???????		
    st_deviceStatusStructDef	status;	//????
	uint8_t     vavleSta;           //uint8_t reverse03xx;   
	//0x40
	uint8_t		cardKeyCreateKey[6];	//M1???????	
	uint16_t	crc16;		
	//??
}st_SystemData;
extern st_SystemData mainSystemData;
//extern const st_SystemData bkupSystemData;

extern uint8_t get_int8_from_flash(const uint8_t *p);
extern uint16_t get_int16_from_flash(const uint16_t *p);
extern uint32_t get_int32_from_flash(const uint32_t *p);
extern uint8_t data_api_flash_process(void);
extern void m_flash_test(void);
extern uint8_t data_api_system_data_init(void);
extern uint8_t data_api_calibration_table_data_init(void);

//******************************************************************************
typedef struct{
	uint8_t ver;
	uint8_t cardType;
	uint16_t CompanyCode;
	uint32_t cardID;
	uint32_t deviceID;
	uint8_t keyUpdate;
	uint8_t	reverse[3];
	uint8_t	SecInf[16];
    //
	uint8_t	cardkeyBcreatKey[14];
	uint16_t crc16;	
}st_cardHeaderDef;

typedef struct{
	uint32_t Value;
	uint16_t Times;
	uint16_t crc16;		
}st_cardValueStrucrtDef;

extern uint8_t data_api_rt_data_save(void);
extern uint8_t data_api_calibration_table_data_save(uint8_t loc,bool single);
extern void data_api_read_eeprom(uint8_t* buf,uint8_t addr,uint8_t len);
#ifdef __cplusplus
}
#endif
#endif