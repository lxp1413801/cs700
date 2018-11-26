//file name:delay.h
#ifndef __RFID_DEMO_H__
#define __RFID_DEMO_H__
#ifdef __cplusplus
extern "C"{
#endif
	
	typedef enum
	{
		RFID_DEMO_READ_NONE,
		RFID_DEMO_READ_ALL,
		RFID_DEMO_READ_BLOCK,
		RFID_DEMO_READ_WRITE_BLOCK,
		RFID_DEMO_SET_KEYA,
		RFID_DEMO_SET_KEYB,
		RFID_DEMO_SET_ACCESS,
		RFID_DEMO_SET_UID,
		RFID_DEMO_UNBRICK,
		RFID_DEMO_DISPENSER,
		RFID_DEMO_FORMAT,
		RFID_DEMO_RECHARGE,
		RFID_DEMO_PAYMENT,
	}em_RfidDemo;
	//add at 2014.10.14
	#define M1CARD_MAX_USED_SECTOR_COUNT_M2D 8
	#define MAX_STANDARD_APLAY_COUNT 64
	#define MAX_QUERY_APLAY_SIZE (512-64*4-8)
	
	typedef enum __CARD_OPERATION_ERROR_CARD
	{
        NO_CARD,
        SELECT_CARD_ERROR,      
		
        READ_HEADER_ERROR,
        CARD_NOT_IS_MINE,         
		isNEW_CARD,
		
		HEADER_CRC_ERROR,
		isNEW_CARD_INIT_WRITE_T_ERROR,
		isNEW_CARD_INIT_OK,
		
		OPEN_ACCOUNT_OK,
		OPEN_ACCOUNT_ERROR,
		
		READ_INC_ERROR,
		READ_DEC_ERROR,
		
		UPDATE_DEC_ERROR,
		UPDATE_DEC_OK,
		
        CARD_NO_BALANCE,
		DEC_VALUE_ERR,
		WRITE_DEC_ERROR,
		
		AUTHENTICATE_DEC_ERROR,
		
		UPDATE_DEC,
		VALUE_ERR,
                RECHAERGE_OVERFLOW,
        NO_ERROR,

	}em_CardErr;
	extern em_CardErr CardError;
	extern uint8_t AplayCount;
	extern void rfid_demo_loop(void);
	//extern void rfid_reader_setup(void);
    //#define rfid_reader_setup() mfapi_pcd_init()
	extern void rfid_reader_remove(void);
	extern bool rfid_card_dection(void);
	extern void rfid_demo_read_block_ex(uint8_t* buf,uint8_t block);
	extern bool rfid_comm_process(void);
    extern void rfid_test(void);
	
	extern uint8_t cardReadingFlag;
#ifdef __cplusplus
}
#endif
#endif
//file end
