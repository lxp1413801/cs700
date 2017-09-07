#include "../includes/includes.h"
#define rfid_demo_error_return() do{ \
	mfapi_end_operation();	\
	}while(0)
//static const uint8_t cardKeyCreatKey[] = { 0x11,0x33,0x55,0x77,0xaa,0xbb};
//const uint8_t ctrlByteDefault[4]={0x08,0x77,0x8F,0x69};
const uint8_t defaultKeyA[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

uint8_t userKeyB[6]  = {'t', 'w', 'd', 'l', 'b', 'g'};
uint8_t cardUidBlockBuf[18] @ 0x420;
uint8_t cardWriteBuf[18] @ 0x432 ;
uint8_t cardIncBuf[18] @ 0x444;
uint8_t cardDecBuf[18] @ 0x456;
/*
const uint8_t defWriteBlock3[18] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0x07, 0x80, 0x69,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xD4, 0x55
};
*/
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
uint8_t rfid_comm_process_read_header(void){
	//em_PcdStatusCode result;
    uint8_t ret;
    piccResult = picc_authenticate(PICC_CMD_MF_AUTH_KEY_A, 3, (st_MifareKey*) defaultKeyA, &PiccUid);
    if (piccResult != STATUS_OK)return 0;
    rdLen=18;
    piccResult = mifare_read(0, cardUidBlockBuf, &rdLen);//读取块0,保存块0
    if (piccResult != STATUS_OK) return 0;	//
    //creat card key;
    m_mem_cpy_len(comBuffer,cardUidBlockBuf,18);
    //create_card_key_b(userKeyB,cardUidBlockBuf,(uint8_t*)cardKeyCreatKey);
    piccResult = picc_authenticate(PICC_CMD_MF_AUTH_KEY_A, 7, (st_MifareKey*) defaultKeyA, &PiccUid);	
	if(piccResult!=STATUS_OK)return 0;
    for(__t8=4;__t8<7;__t8++){
        rdLen=18;
         piccResult = mifare_read(__t8, cardWriteBuf, &rdLen);
            m_mem_cpy_len(comBuffer+(__t8-4)*16,cardWriteBuf,16);//cardWriteBuf
        if(piccResult!=STATUS_OK){ CardError=READ_HEADER_ERROR; break;}
    }    
    ret=crc_verify(comBuffer,sizeof(st_cardHeaderDef));
	if(!ret)return 0;
    //-------------------------------------------------------------------------------
    st_cardHeaderDef* stp=(st_cardHeaderDef*)comBuffer;
    if(stp->keyUpdate ==0xff ){
        flashOpStr.op=emFLASH_WRITE_main_SYSTEM_DATA;
        flashOpStr.flashAddr=__get_offset(st_SystemData,cardKeyCreateKey[0]);
        flashOpStr.len=6;
        m_mem_cpy_len(flashOpStr.buf,(uint8_t*)(stp->cardkeyBcreatKey),6);
        m_mem_cpy_len(comBuffer+80,comBuffer,48);
        ret=data_api_flash_process();
        m_mem_cpy_len(comBuffer,comBuffer+80,48);
        if(ret){
            stp->keyUpdate=0;
            m_mem_set(stp->cardkeyBcreatKey,0,6);
            crc_append(comBuffer,sizeof(st_cardHeaderDef)-2);	
           // piccResult = picc_authenticate(PICC_CMD_MF_AUTH_KEY_A, 7, (st_MifareKey*) defaultKeyA, &PiccUid);
            for(__t8=4;__t8<7;__t8++){
                m_mem_cpy_len(cardWriteBuf,comBuffer+(__t8-4)*16,16);
                piccResult = mifare_write(__t8, cardWriteBuf, 16);
                if(piccResult!=STATUS_OK)break;
            }
        }
    }
	if( mainSystemData.cardID==0)
	{	
		if(!(stp->deviceID==mainSystemData.deviceID)){
			CardError=CARD_NOT_IS_MINE;
			return 0;
		}
		flashOpStr.flashAddr=__get_offset(st_SystemData,cardID);
		flashOpStr.op=emFLASH_WRITE_main_SYSTEM_DATA;
		flashOpStr.len=4;
		m_mem_cpy_len(flashOpStr.buf,cardUidBlockBuf,4);
		ret=data_api_flash_process();

	}else if(mainSystemData.cardID!=*((uint32_t*)cardUidBlockBuf) || \
		mainSystemData.deviceID != stp->deviceID){
		CardError=CARD_NOT_IS_MINE;
		return 0;
	}
    create_card_key_b(userKeyB,cardUidBlockBuf,(uint8_t*)mainSystemData.cardKeyCreateKey);
    return 1;
} 

uint8_t rfid_comm_process_read_inc(void){
	//uint8_t len=18;
	uint8_t ret;
	//em_PcdStatusCode result;
	piccResult = picc_authenticate(PICC_CMD_MF_AUTH_KEY_B, 11, (st_MifareKey*) userKeyB, &PiccUid);
	__t8=18;
	piccResult = mifare_read(8, cardIncBuf, &__t8);
	ret=crc_verify(cardIncBuf,sizeof(st_cardValueStrucrtDef));
	return ret;	
}
uint8_t rfid_comm_process_read_dec(void){
	//uint8_t len=18;
	uint8_t ret;
	//em_PcdStatusCode result;
	piccResult = picc_authenticate(PICC_CMD_MF_AUTH_KEY_B, 15, (st_MifareKey*) userKeyB, &PiccUid);
	rdLen=18;
	piccResult = mifare_read(12, cardDecBuf, &rdLen);
	ret=crc_verify(cardDecBuf,sizeof(st_cardValueStrucrtDef));
	return ret;
}
uint8_t rfid_comm_process_recharge(void){
	//em_PcdStatusCode result;
	st_cardValueStrucrtDef* stpInc,*stpDec;
	//uint8_t len;
	uint8_t ret;
	stpInc=(st_cardValueStrucrtDef*)cardIncBuf;
	stpDec=(st_cardValueStrucrtDef*)cardDecBuf;
	piccResult = picc_authenticate(PICC_CMD_MF_AUTH_KEY_B, 15, (st_MifareKey*) userKeyB, &PiccUid);
	if(stpDec->Times < mainSystemData.payTimes)
	{
		stpDec->Value=mainSystemData.transferMoney;
		stpDec->Times=mainSystemData.payTimes;
		crc_append(cardDecBuf,sizeof(st_cardValueStrucrtDef)-2);	
		piccResult = mifare_write(12, cardDecBuf,16);  
		if(piccResult!=STATUS_OK){
			return 0;
		}
	}
	__q31=stpInc->Value;
	__q31=__q31-stpDec->Value;
	if(__q31<=0){
		CardError=CARD_NO_BALANCE;
		return 0;
	}
	if(__q31>mainSystemData.transferMoneyLimit){
		__q31=mainSystemData.transferMoneyLimit;
	}
	stpDec->Value += __q31;
	stpDec->Times+=1; 	
	__t16=stpDec->Times,
	crc_append(cardDecBuf,sizeof(st_cardValueStrucrtDef)-2);	
	//transfer to device

	flashOpStr.flashAddr=__get_offset(st_SystemData,transferMoney);
	flashOpStr.op=emFLASH_WRITE_main_SYSTEM_DATA;
	flashOpStr.len=6;
	rtBalanceMoney=rtBalanceMoney+__q31;
	if(rtBalanceMoney>=mainSystemData.maxBalanceLimit){
        rtBalanceMoney=rtBalanceMoney-__q31;
		CardError=RECHAERGE_OVERFLOW;
		return 0;
	};
	__q31=mainSystemData.transferMoney+__q31;    
	m_mem_cpy_len((uint8_t*)(&flashOpStr.buf[0]),(uint8_t*)(&(__q31)),4);
	m_mem_cpy_len((uint8_t*)(&flashOpStr.buf[4]),(uint8_t*)(&(__t16)),2);
	ret=data_api_flash_process();	
	piccResult = picc_authenticate(PICC_CMD_MF_AUTH_KEY_B, 15, (st_MifareKey*) userKeyB, &PiccUid);
	piccResult = mifare_write(12, cardDecBuf, 16);
	if(piccResult!=STATUS_OK){
		return 0;
	}
	return 1;
}

uint8_t rfid_comm_process(void) {
    //uint8_t len = 18;
    uint8_t ret = 0;
   // uint8_t sak;
    em_PcdStatusCode result;
    do {
		CardError=NO_CARD;
        if (!(mfapi_is_new_card_present()))break;
        CardError=SELECT_CARD_ERROR;
        piccResult = picc_select(&PiccUid, 0);
        if (piccResult != STATUS_OK)break;
        if (PiccUid.sak != 0x08) break;
        CardError=READ_HEADER_ERROR;;
		ret=rfid_comm_process_read_header();
		if(!ret)break;
		ret=rfid_comm_process_read_inc();
		if(!ret)break;
		ret=rfid_comm_process_read_dec();
		if(!ret)break;
        ui_disp_balance_and_volume();
        led4_on();led2_on();led1_off();led3_off();
        for(ret=0;ret<15;ret++){
            delay_200ms();
        }
		ret=rfid_comm_process_recharge();
        if(!ret){
            if(CardError==RECHAERGE_OVERFLOW){
                lcd_disp_str((uint8_t*)("--full--"));
            }
        }else{
            ui_disp_balance_and_volume();
        }
        for(ret=0;ret<12;ret++)
        {
            led4_change();
            delay_500ms();
        }
            CardError=NO_ERROR;
		//ret = 1;
    } while (0);
    mfapi_end_operation();
    /*
    if(CardError>SELECT_CARD_ERROR){
        //ui_disp_rfid_err();
        //card_led_flash();
    }
*/
    findCardTimer=0;
    Event &= ~flgEventReadCardTimerUp;
    return ret;
}