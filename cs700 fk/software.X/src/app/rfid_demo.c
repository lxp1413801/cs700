
#include "../includes/includes.h"

#define rfid_demo_error_return() do{ \
	mfapi_end_operation();	\
	}while(0)

//em_RfidDemo RfidKeyMode=RFID_DEMO_READ_NONE;

/*
6????
????
XXTea
????
????????
    0?AD7F3FB245AEDC8F0F9E3830010FE1C1
    1~254:???????
????????
    BAED03C869FC4FDE0DF3E0AE36F71ED2
 */

#ifndef cfg_CARD_FUNCTION_DEBUG
    #define cfg_CARD_FUNCTION_DEBUG 1
#endif



static const uint8_t cardKeyCreatKey[] = {
    0x11,0x33,0x55,0x77,0xaa,0xbb

};
const uint8_t ctrlByteDefault[4]={0x08,0x77,0x8F,0x69};
const uint8_t defaultKeyA[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

uint8_t userKeyB[6]  = {'t', 'w', 'd', 'l', 'b', 'g'};
uint8_t cardUidBlockBuf[18] @ 0x420;
uint8_t cardWriteBuf[18] @ 0x432 ;
uint8_t cardIncBuf[18] @ 0x444;
uint8_t cardDecBuf[18] @ 0x456;
const uint8_t defWriteBlock3[18] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0x07, 0x80, 0x69,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xD4, 0x55
};

em_PcdStatusCode piccResult;
uint8_t rdLen;
em_CardErr CardError;

#define card_led_flash() do{ \
    led5_on(); \
    delay_1s(); \
    led5_off(); \
}while(0);


void create_card_key_b(uint8_t* keyb, uint8_t* uid,uint8_t* cckey){
    uint8_t i;
    for(i=0;i<6;i++){
        *keyb=*uid ^ *cckey;
        keyb++;
        uid++;
        cckey++;
    }
}
uint8_t  rfid_comm_process_fk(void)
{
	em_PcdStatusCode result;
	uint8_t ret;
	uint8_t i;
	do{
		ret=0;
		if (!(mfapi_is_new_card_present()))break;
		result = picc_select(&PiccUid, 0);
		if (result != STATUS_OK)break;

		result = picc_authenticate(PICC_CMD_MF_AUTH_KEY_A, 3, (st_MifareKey*) defaultKeyA, &PiccUid);
		if (result != STATUS_OK)return 0;
		rdLen=18;
		result = mifare_read(0, cardUidBlockBuf, &rdLen);//读取块0,保存块0
		if (result != STATUS_OK) return 0;	//
		//creat card key;
		m_mem_cpy_len(comBuffer,cardUidBlockBuf,18);
		create_card_key_b(userKeyB,cardUidBlockBuf,(uint8_t*)cardKeyCreatKey);
		m_mem_set(comBuffer,0,48);
		st_cardHeaderDef* stp=(st_cardHeaderDef*)comBuffer;   
		stp->ver=100;
		stp->cardType=0xaa;
		stp->deviceID=4321;
		stp->CompanyCode=1234;
		stp->keyUpdate=0xff;
        m_mem_cpy_len(stp->cardkeyBcreatKey,(uint8_t*)cardKeyCreatKey,6);
		crc_append(comBuffer,sizeof(st_cardHeaderDef)-2);	
		result = picc_authenticate(PICC_CMD_MF_AUTH_KEY_A, 7, (st_MifareKey*) defaultKeyA, &PiccUid);
		for(__t8=4;__t8<7;__t8++){
			m_mem_cpy_len(cardWriteBuf,comBuffer+(__t8-4)*16,16);
			result = mifare_write(__t8, cardWriteBuf, 16);
			if(result!=STATUS_OK)break;
		}  
		m_mem_cpy_len(cardWriteBuf,(uint8_t*)userKeyB,6);
		m_mem_cpy_len(cardWriteBuf+6,(uint8_t*)ctrlByteDefault,4);
		m_mem_cpy_len(cardWriteBuf+10,userKeyB,6);	
		//blk 7 11 15
		for(i=11;i<16;i+=4)	
		{
			result = picc_authenticate(PICC_CMD_MF_AUTH_KEY_A,i, (st_MifareKey*) defaultKeyA, &PiccUid);
			result = mifare_write(i, cardWriteBuf, 16);
			if(result!=STATUS_OK)return 0;
		}    
		st_cardValueStrucrtDef* stpIncDec=(st_cardValueStrucrtDef*)comBuffer;	
		m_mem_set(comBuffer,0,18);
		stpIncDec->Times=1;
		stpIncDec->Value=300000;
		crc_append(comBuffer,sizeof(st_cardValueStrucrtDef)-2);	
		result = picc_authenticate(PICC_CMD_MF_AUTH_KEY_B, 11, (st_MifareKey*) userKeyB, &PiccUid);
		result = mifare_write(8, comBuffer, 16);
		m_mem_set(comBuffer,0,18);
		crc_append(comBuffer,sizeof(st_cardValueStrucrtDef)-2);	
		result = picc_authenticate(PICC_CMD_MF_AUTH_KEY_B, 15, (st_MifareKey*) userKeyB, &PiccUid);
		result = mifare_write(12, comBuffer, 16);
		if(result!=STATUS_OK);
		ret=1;
	}while(0);
	mfapi_end_operation();
    return ret;
}
uint8_t  rfid_comm_process_hs(void)
{
	em_PcdStatusCode result;
	uint8_t ret;
	uint8_t i;
	do{
		ret=0;
		if (!(mfapi_is_new_card_present()))break;
		result = picc_select(&PiccUid, 0);
		if (result != STATUS_OK)break;

		result = picc_authenticate(PICC_CMD_MF_AUTH_KEY_A, 3, (st_MifareKey*) defaultKeyA, &PiccUid);
		if (result != STATUS_OK)return 0;
		rdLen=18;
		result = mifare_read(0, cardUidBlockBuf, &rdLen);//读取块0,保存块0
		if (result != STATUS_OK) return 0;	//
		//creat card key;
		m_mem_cpy_len(comBuffer,cardUidBlockBuf,18);
		create_card_key_b(userKeyB,cardUidBlockBuf,(uint8_t*)cardKeyCreatKey);
		m_mem_set(comBuffer,0,48);	
		result = picc_authenticate(PICC_CMD_MF_AUTH_KEY_A, 7, (st_MifareKey*) defaultKeyA, &PiccUid);
		for(__t8=4;__t8<7;__t8++){
			m_mem_cpy_len(cardWriteBuf,comBuffer+(__t8-4)*16,16);
			result = mifare_write(__t8, cardWriteBuf, 16);
			if(result!=STATUS_OK)break;
		}  
		m_mem_cpy_len(cardWriteBuf,(uint8_t*)defWriteBlock3,18);
		//blk 7 11 15
		for(i=11;i<16;i+=4)	
		{
			result = picc_authenticate(PICC_CMD_MF_AUTH_KEY_B,i, (st_MifareKey*) userKeyB, &PiccUid);
			result = mifare_write(i, cardWriteBuf, 16);
			if(result!=STATUS_OK)return 0;
		}    
		st_cardValueStrucrtDef* stpIncDec=(st_cardValueStrucrtDef*)comBuffer;	
		m_mem_set(comBuffer,0,18);
		result = picc_authenticate(PICC_CMD_MF_AUTH_KEY_A, 11, (st_MifareKey*) defaultKeyA, &PiccUid);
		result = mifare_write(8, comBuffer, 16);
		m_mem_set(comBuffer,0,18);
		result = picc_authenticate(PICC_CMD_MF_AUTH_KEY_A, 15, (st_MifareKey*) defaultKeyA, &PiccUid);
		result = mifare_write(12, comBuffer, 16);
		if(result!=STATUS_OK);
		ret=1;
	}while(0);
	mfapi_end_operation();
    return ret;
}