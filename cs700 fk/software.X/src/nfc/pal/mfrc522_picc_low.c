#include "../mfrc522_config.h"
#include "../mfrc522_includes.h"
uint8_t CardUid[5] = {0};
uint8_t nowCardUid[5] = {0};
#define picc_comm_with_pcd_low pcd_comm_with_picc_low
/*
bool mfapi_pcd_init(void) {
    uint8_t v;
    mfrc522_hal_init();
    v = mfrc522_bal_read_reg(VersionReg);
    if (!((v == 0x90) || (v == 0x91) || (v == 0x92))) {
        return false;
    }
    return true;
}
*/
em_PcdStatusCode pcd_comm_with_picc_low
(
        uint8_t cmd, uint8_t *sbuf,
        uint8_t slen, uint8_t *rbuf,
        uint16_t *rblen) {
    em_PcdStatusCode status = STATUS_ERROR;
    uint8_t irqEn = 0x00;
    uint8_t waitFor = 0x00;
    uint8_t lastBits;
    uint8_t n;
    uint16_t i;
    uint8_t pOutLenBit;
    switch (cmd) {
        case PCD_MFAuthent:
            irqEn = 0x12;
            waitFor = 0x10;
            break;
        case PCD_Transceive:
            irqEn = 0x77;
            waitFor = 0x30;
            break;
        default:
            break;
    }
    mfrc522_bal_write_reg(CommandReg, PCD_Idle);
    mfrc522_bal_write_reg(ComIrqReg, 0x7F);
    mfrc522_bal_set_bit_reg(FIFOLevelReg, 0x80);
    for (i = 0; i < slen; i++)
        mfrc522_bal_write_reg(FIFODataReg, sbuf[i]);
    mfrc522_bal_write_reg(CommandReg, cmd);
    if (cmd == PCD_Transceive)
        mfrc522_bal_set_bit_reg(BitFramingReg, 0x80); // ????     
    i = 6000; //???????????M1???????25ms
    do {
        n = mfrc522_bal_read_reg(ComIrqReg);
        i--;
    } while ((i != 0)&&!(n & 0x01)&&!(n & waitFor));
    mfrc522_bal_clr_bit_reg(BitFramingReg, 0x80);
    if (i != 0) {
        if (!(mfrc522_bal_read_reg(ErrorReg)&0x1B)) {
            status = STATUS_OK;
            if (n & irqEn & 0x01)
                status = STATUS_ERROR;
            if (cmd == PCD_Transceive) {
                n = mfrc522_bal_read_reg(FIFOLevelReg);
                lastBits = mfrc522_bal_read_reg(ControlReg)&0x07;
                if (lastBits)
                    pOutLenBit = (n - 1)*8 + lastBits;
                else
                    pOutLenBit = n * 8;
                if (n == 0)
                    n = 1;
                if (n > 18)
                    n = 18;
                for (i = 0; i < n; i++)
                    rbuf[i] = mfrc522_bal_read_reg(FIFODataReg);
                *rblen = pOutLenBit;
            }
        } else
            status = STATUS_ERROR;
    }
    //mfrc522_bal_set_bit_reg(ControlReg, 0x80); // stop timer now
    //mfrc522_bal_write_reg(CommandReg, PCD_Idle);
    return status;
}

em_PcdStatusCode picc_requset_low(uint8_t req_code, uint8_t *pTagType) {
    em_PcdStatusCode status = STATUS_ERROR;
    ;
    uint16_t len;
    uint8_t buf[18];

    mfrc522_bal_clr_bit_reg(Status2Reg, 0x08);
    mfrc522_bal_write_reg(BitFramingReg, 0x07);
    mfrc522_bal_set_bit_reg(TxControlReg, 0x03);

    buf[0] = req_code;

    status = pcd_comm_with_picc_low(PCD_Transceive, buf,
            1, buf, &len);
    if ((status == STATUS_OK) && (len == 0x10)) {
        *pTagType = buf[0];
        *(pTagType + 1) = buf[1];
    } else
        status = STATUS_ERROR;
    return status;
}

em_PcdStatusCode picc_antilcoll_low(uint8_t *pSnr) {
    em_PcdStatusCode status = STATUS_ERROR;
    uint8_t i, snr_check = 0;
    uint16_t unLen;
    uint8_t buf[18];

    mfrc522_bal_clr_bit_reg(Status2Reg, 0x08);
    mfrc522_bal_write_reg(BitFramingReg, 0x00);
    mfrc522_bal_set_bit_reg(CollReg, 0x80);

    buf[0] = PICC_CMD_SEL_CL1;
    buf[1] = 0x20;

    status = pcd_comm_with_picc_low(PCD_Transceive, buf, 2, buf, &unLen);

    if (status == STATUS_OK) {
        for (i = 0; i < 4; i++) {
            *(pSnr + i) = buf[i];
            snr_check ^= buf[i];
        }
        if (snr_check != buf[i]) {
            status = STATUS_ERROR;
        }
    }

    mfrc522_bal_set_bit_reg(CollReg, 0x80);
    return status;
}

em_PcdStatusCode picc_select_low(uint8_t *pSnr) {
    em_PcdStatusCode status = STATUS_ERROR;
    uint8_t i;
    uint16_t unLen;
    uint8_t buf[18];

    buf[0] = PICC_CMD_SEL_CL1;
    buf[1] = 0x70;
    buf[6] = 0;
    for (i = 0; i < 4; i++) {
        buf[i + 2] = *(pSnr + i);
        buf[6] ^= *(pSnr + i);
    }
    mfrc522_hal_calculate_crc(buf, 7, &buf[7]);

    mfrc522_bal_clr_bit_reg(Status2Reg, 0x08);

    status = pcd_comm_with_picc_low(PCD_Transceive, buf, 9, buf, &unLen);

    if ((status == STATUS_OK) && (unLen == 0x18)) {
        status = STATUS_OK;
    } else {
        status = STATUS_ERROR;
    }

    return status;
}

em_PcdStatusCode picc_authenticate_low(bool keyb, uint8_t addr,
        uint8_t *pKey, uint8_t *pSnr) {
    em_PcdStatusCode status;
    uint16_t unLen;
    uint8_t i, buf[18];

    if (keyb) {
        buf[0] = PICC_CMD_MF_AUTH_KEY_B;
    } else {
        buf[0] = PICC_CMD_MF_AUTH_KEY_A;
    }
    buf[1] = addr;
    for (i = 0; i < 6; i++) {
        buf[i + 2] = *(pKey + i);
    }
    for (i = 0; i < 6; i++) {
        buf[i + 8] = *(pSnr + i);
    }

    return pcd_comm_with_picc_low(PCD_MFAuthent, buf, 12, buf, &unLen);

}

em_PcdStatusCode mifare_read_low(uint8_t blk, uint8_t *rbuf) {
    em_PcdStatusCode status;
    uint16_t unLen;
    uint8_t i, buf[18];

    buf[0] = PICC_CMD_MF_READ;
    buf[1] = blk;
    status = mfrc522_hal_calculate_crc(buf, 2, &buf[2]);
    if (status != STATUS_OK) {
        return status;
    }

    status = pcd_comm_with_picc_low(PCD_Transceive, buf, 4,
            buf, &unLen);
    m_mem_cpy_len(rbuf, buf, unLen / 8);

    return status;
}

em_PcdStatusCode mifare_write_low(uint8_t blk, uint8_t *sbuf, uint8_t slen) {
    em_PcdStatusCode result;
    if (sbuf == NULL || slen < 16) {
        return STATUS_INVALID;
    }
    uint8_t buf[18];
    uint16_t len;
    buf[0] = PICC_CMD_MF_WRITE;
    buf[1] = blk;
    result = pcd_comm_with_picc_low(PCD_Transceive, buf, 2, buf, &len);
    if (result != STATUS_OK) {
        return result;
    }
    m_mem_cpy_len(buf, sbuf, slen);
    result = mfrc522_hal_calculate_crc(buf, 16, &buf[16]);
    result = pcd_comm_with_picc_low(PCD_Transceive, buf, 18, buf, &len);
    return result;
}

em_PcdStatusCode picc_halt_a_low(void) {
    em_PcdStatusCode result;
    uint16_t unLen;
    uint8_t buf[18];

    buf[0] = PICC_CMD_HLTA;
    buf[1] = 0;
    result = mfrc522_hal_calculate_crc(buf, 2, &buf[2]);
    if (result != STATUS_OK)return result;
    result = pcd_comm_with_picc_low(PCD_Transceive, buf, 4, buf, &unLen);
    return result;
}

void picc_stop_cryptol_low(void) {
    mfrc522_bal_clr_bit_reg(Status2Reg, 0x08);
}

bool mfapi_is_new_card_find_low(void) {
    em_PcdStatusCode result;
    uint8_t buf[18];
    m_mem_set(nowCardUid, 0, sizeof (nowCardUid));
    result = picc_requset_low(PICC_CMD_WUPA, buf);
    if (!(result == STATUS_OK || result == STATUS_COLLISION))return false;
    //if (result != STATUS_OK)return false;
    result = picc_antilcoll_low(buf);
    if (result != STATUS_OK)return false;
    m_mem_cpy_len(nowCardUid, buf, sizeof (nowCardUid));
    //if(m_str_cmp_len(nowCardUid,CardUid,5))return false;
    //m_mem_cpy_len(CardUid, nowCardUid, sizeof(nowCardUid));
    result = picc_select_low(nowCardUid);
    if (result != STATUS_OK)return false;
    return true;
}