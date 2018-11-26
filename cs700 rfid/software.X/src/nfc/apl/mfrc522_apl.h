//file:mfrc522_apl.h
#ifndef __MFRC522_APL_H__
#define __MFRC522_APL_H__

	#ifdef __cplusplus
		extern "C"{
	#endif

	
	//default a function point for mfrc522 api echo
	extern  st_PiccUid PiccUid;
	//extern bool mfapiLibInit;
	
	extern uint8_t* mfapi_get_pcd_version(uint8_t* version);
	//extern em_PiccType mfapi_get_picc_type(uint8_t sak);
	extern uint16_t  mfapi_get_picc_sectors(em_PiccType type);
	//extern uint8_t* mfapi_get_picc_name(em_PiccType type);
	
	extern bool mfapi_pcd_self_test(void); 
	extern bool mfapi_lib_init(void);
	extern bool mfapi_pcd_init(void);
    //extern void rfid_reader_setup(void);
	extern bool mfapi_is_new_card_present(void);
	extern bool mfapi_anti_collision_and_select(st_PiccUid* uid);
	extern void mfapi_get_picc_serial_from_st(st_PiccUid* uid,uint8_t* serial,uint8_t* size);
	
	//extern void mfapi_picc_echo_whole_data(st_PiccUid* uid,st_MifareKey* key);
	extern bool mfapi_read_block(st_PiccUid* uid,st_MifareKey* key,uint8_t* buf,uint8_t* trailer,uint8_t block,bool keybuse);
	
	extern bool mfapi_authenticate_block(st_PiccUid* uid,st_MifareKey* key,uint16_t block,	bool keybuse);
	extern bool mfapi_read_block_only(uint8_t block,uint8_t* buf,uint8_t* readlen);
	extern bool mfapi_write_block_only(uint8_t block,uint8_t* buf,uint8_t wrlen);
	extern bool mfapi_get_value(uint8_t block,int32_t *value);
	extern bool mfapi_set_value(uint8_t block,int32_t value);
	extern bool mfapi_increment(uint8_t blockAddr, int32_t delta);
	extern bool mfapi_decrement(uint8_t blockAddr, int32_t delta);
	

	#define mfapi_end_operation() do{ \
			picc_halt_a(); \
			picc_stop_cryptol(); \
			picc_halt_a(); 	\
	}while(0);
	#ifdef __cplusplus
		}
	#endif
#endif

	