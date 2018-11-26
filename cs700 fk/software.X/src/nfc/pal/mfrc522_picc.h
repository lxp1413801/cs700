//file:mfrc522_picc.h
#ifndef __MFRC522_PICC_H__
#define __MFRC522_PICC_H__

	#ifdef __cplusplus
		extern "C"{
	#endif

	//extern em_PcdStatusCode pcd_transceive(uint8_t *sbuf,uint8_t slen,uint8_t *rbuf,uint8_t *rlen,uint8_t *validBits,uint8_t rxAlign,bool checkCRC);
	extern em_PcdStatusCode pcd_comm_with_picc(uint8_t cmd,uint8_t waitIRQ,uint8_t* sbuf,uint8_t slen,uint8_t* rbuf,uint8_t* rlen,uint8_t *validBits,uint8_t rxAlign,bool checkCRC);
    #define pcd_transceive(sbuf,slen,rbuf,rlen,validBits,rxAlign,checkCRC) do{ \
        pcd_comm_with_picc(PCD_Transceive,0x30,sbuf,slen,rbuf,rlen,validBits,rxAlign,checkCRC); \
        }while(0);

    //#define picc_comm_with_pcd pcd_comm_with_picc
	//extern em_PcdStatusCode picc_requset_a(uint8_t *bufferATQA,uint8_t *bufferSize); 	
	//extern em_PcdStatusCode picc_wakeup_a(uint8_t *bufferATQA,uint8_t *bufferSize); 
    extern em_PcdStatusCode picc_req_a_or_wakeup_a(uint8_t cmd,uint8_t *bufferATQA,uint8_t *bufferSize);
    #define picc_requset_a(bufferATQA,bufferSize) do{ \
        picc_req_a_or_wakeup_a(PICC_CMD_REQA,bufferATQA,bufferSize); \
        }while(0);
    #define picc_wakeup_a(bufferATQA,bufferSize)  do{ \
        uint8_t __cmd=PICC_CMD_WUPA; \
        picc_req_a_or_wakeup_a(__cmd,bufferATQA,bufferSize); \
        }while(0);
	
    
	extern em_PcdStatusCode picc_select(st_PiccUid *uid,uint8_t validBits);
    extern void picc_stop_cryptol(void);
	extern em_PcdStatusCode picc_halt_a(void);

	extern em_PcdStatusCode picc_authenticate(uint8_t command, uint8_t blockAddr, st_MifareKey *key, st_PiccUid *uid);

	//extern uint8_t* get_comm_status_code_name(em_PcdStatusCode code);
	
	#ifdef __cplusplus
		}
	#endif
#endif
