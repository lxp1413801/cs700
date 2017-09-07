//file:mfrc_hal_def.h
#ifndef __MFRC522_HAL_H__
#define __MFRC522_HAL_H__

#ifdef __cplusplus
	extern "C"{
#endif

	//#include "mfrc522_hal_def.h"
	//extern st_PiccUid uid;
	
	extern em_PcdStatusCode mfrc522_hal_calculate_crc(uint8_t *data, uint8_t length, uint8_t *result);
	 
	extern void mfrc522_hal_init(void);
	extern void mfrc522_hal_reset_sf(void);
	
	extern void mfrc522_hal_antenna_on(void);
	extern void mfrc522_hal_antenna_off(void);
	extern uint8_t mfrc522_hal_get_antenna_gain(void);
	extern void mfrc522_hal_set_antenna_gain(uint8_t gain);
	extern uint16_t mfrc522_hal_self_test(void);
	

	
#ifdef __cplusplus
	}
#endif	//end macro __cplusplus

#endif	//end macro __MFRC522_HAL_H__
//end