//file:mfrc522_mifare.h
#ifndef __MFRC522_MIFARE_H__
#define __MFRC522_MIFARE_H__

#ifdef __cplusplus
	extern "C"{
#endif
	/*
	byte6 C23_b C22_b C21_b C20_b C13_b C12_b C11_b C10_b
	byte7 C13 C12 C11 C10 C33_b C32_b C31_b C30_b
	byte8 C33 C32 C31 C30 C23 C22 C21 C20
	*/
	//define access format
	//authority
	
	typedef union __MIFARE_ACCESS_BYTE_FORMAT
	{
		uint8_t buf[4];
		struct
		{
            //fuck 
			uint8_t C1X0_b:1;   //7     
			uint8_t C1X1_b:1;   //6            
			uint8_t C1X2_b:1;   //5     
 			uint8_t C1X3_b:1;   //4           
 			uint8_t C2X0_b:1;   //3  
			uint8_t C2X1_b:1;   //2            
 			uint8_t C2X2_b:1;   //1            
			uint8_t C2X3_b:1;   //0

			uint8_t C3X0_b:1;   //7
			uint8_t C3X1_b:1;   //6            
			uint8_t C3X2_b:1;   //5   
			uint8_t C3X3_b:1;   //4            
 			uint8_t C1X0:1;     //3   
 			uint8_t C1X1:1;     //2           
 			uint8_t C1X2:1;     //1           
			uint8_t C1X3:1;     //0

            
			uint8_t C2X0:1;	    //7   
			uint8_t C2X1:1;     //6            
			uint8_t C2X2:1;     //5    
			uint8_t C2X3:1;     //4            
 			uint8_t C3X0:1;     //3   
 			uint8_t C3X1:1;     //2           
			uint8_t C3X2:1;     //1            
			uint8_t C3X3:1;     //0

			uint8_t bx:8;			
		}bits;
	}st_MifareAeccessFormat;
	// Functions for communicating with MIFARE PICCs
	
	
	
    extern em_PcdStatusCode pcd_mifare_transceive(uint8_t *sendData,uint8_t sendLen,bool acceptTimeout);


	extern em_PcdStatusCode mifare_two_Step_helper(uint8_t command,uint8_t blockAddr,int32_t data);
	
	extern em_PcdStatusCode mifare_read(uint8_t blockAddr, uint8_t *buffer, uint8_t *bufferSize);
	extern em_PcdStatusCode mifare_write(uint8_t blockAddr, uint8_t *buffer, uint8_t bufferSize);
    extern em_PcdStatusCode mifare_ultralight_write(uint8_t page, uint8_t *buffer, uint8_t bufferSize);
    extern em_PcdStatusCode mifare_decrement(uint8_t blockAddr, int32_t delta);
	extern em_PcdStatusCode mifare_increment(uint8_t blockAddr, int32_t delta);
	extern em_PcdStatusCode mifare_restore(uint8_t blockAddr);
	extern em_PcdStatusCode mifare_transfer(uint8_t blockAddr);
	extern em_PcdStatusCode mifare_get_value(uint8_t blockAddr, int32_t *value);
	extern em_PcdStatusCode mifare_set_value(uint8_t blockAddr, int32_t value);
	extern em_PcdStatusCode picc_ntag216_auth(uint8_t *passWord, uint8_t* pACK);

	//Advanced functions for MIFARE

	extern void mifare_set_access_bits(uint8_t *accessBitBuffer, uint8_t g0, uint8_t g1, uint8_t g2, uint8_t g3);
	//extern bool mifare_open_uid_back_door(bool logErrors);
	//extern bool mifare_set_uid(uint8_t* newUid, uint8_t uidSize, bool logErrors); 
	//extern bool mifare_unbrick_uid_sector(uint8_t* buf,bool logErrors);
	
	//
	extern bool mifare_extract_access_form_buffer(uint8_t* buf,uint8_t* access);
	extern bool mifare_extract_access_form_buffer_ex(uint8_t* buf,uint8_t* access,uint8_t index);
	extern bool mifare_combination_buffer_form_access(uint8_t* buf,uint8_t* access);
	extern bool mifare_combination_buffer_form_access_ex(uint8_t* buf,uint8_t* access,uint8_t index);
	//extern void mifare_echo_access_infermation(uint8_t* access);
#ifdef __cplusplus
	}
#endif

#endif