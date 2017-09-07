#ifndef __mrfc522_picc_low_h__
#define __mrfc522_picc_low_h__

#ifdef __cplusplus
extern "C" {
#endif
    extern em_PcdStatusCode pcd_comm_with_picc_low(uint8_t cmd, uint8_t *sbuf, uint8_t slen, uint8_t *rbuf, uint16_t *rblen);
    extern em_PcdStatusCode picc_requset_low(uint8_t req_code, uint8_t *pTagType);
    extern em_PcdStatusCode picc_antilcoll_low(uint8_t *pSnr);
    extern em_PcdStatusCode picc_select_low(uint8_t *pSnr);
    extern em_PcdStatusCode picc_authenticate_low(bool keyb, uint8_t addr, uint8_t *pKey, uint8_t *pSnr);
    extern em_PcdStatusCode mifare_read_low(uint8_t blk, uint8_t *rbuf);
    extern em_PcdStatusCode mifare_write_low(uint8_t blk, uint8_t *sbuf, uint8_t slen);
    extern em_PcdStatusCode picc_halt_a_low(void);
    extern void picc_stop_cryptol_low(void);
    //
    extern bool mfapi_is_new_card_find_low(void);
    extern bool mfapi_pcd_init(void);
    /*
#define picc_end_operation_low() do{ \
    picc_halt_a_low(); \
    picc_stop_cryptol_low(); \
    }while(0);
     */
	#define picc_end_operation_low() do{ \
			picc_halt_a_low(); \
			picc_stop_cryptol_low(); \
			picc_halt_a_low(); 	\
	}while(0);
#ifdef __cplusplus
}
#endif

#endif
