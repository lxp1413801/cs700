//file name:mfrc522_picc.c
//add other header file
//...
#include "../mfrc522_config.h"
#include "../mfrc522_includes.h"
/*
em_PcdStatusCode pcd_transceive
(
        uint8_t *sbuf,
        uint8_t slen,
        uint8_t *rbuf,
        uint8_t *rlen,
        uint8_t *validBits,
        uint8_t rxAlign,
        bool checkCRC
        ) {
    em_PcdStatusCode status;
    uint8_t waitIrq = 0x30;

    status = pcd_comm_with_picc( \
        PCD_Transceive, \
        waitIrq, \
        sbuf, \
        slen, \
        rbuf, \
        rlen, \
        validBits, \
        rxAlign, \
        checkCRC);
    return status;


}
*/
em_PcdStatusCode pcd_comm_with_picc
(
        uint8_t cmd,
        uint8_t waitIRQ,
        uint8_t* sbuf,
        uint8_t slen,
        uint8_t* rbuf,
        uint8_t* rlen,
        uint8_t *validBits,
        uint8_t rxAlign,
        bool checkCRC
        ) {
    uint8_t tmp8, _validBits;
    uint16_t i;

    uint8_t txLastBits = validBits ? (*validBits) : 0;
    uint8_t bitFraming = (rxAlign << 4) + txLastBits;

    mfrc522_bal_write_reg(CommandReg, PCD_Idle);
    mfrc522_bal_write_reg(ComIrqReg, 0x7F);
    mfrc522_bal_set_bit_reg(FIFOLevelReg, 0x80);
    mfrc522_bal_write_reg_len(FIFODataReg, sbuf, slen);
    mfrc522_bal_write_reg(BitFramingReg, bitFraming);
    mfrc522_bal_write_reg(CommandReg, cmd);
    if (cmd == PCD_Transceive) {
        mfrc522_bal_set_bit_reg(BitFramingReg, 0x80);
    }

    i = 2000;//(MFRC522_DELAY_10MS);
    while (i) {
        tmp8 = mfrc522_bal_read_reg(ComIrqReg);
        if (tmp8 & waitIRQ) break;
        if (tmp8 & 0x01) return STATUS_TIMEOUT;
        i--;
        if(i==0)return STATUS_TIMEOUT;
    }
    uint8_t errorRegValue = mfrc522_bal_read_reg(ErrorReg);
    if (errorRegValue & 0x13)return STATUS_ERROR;
    if ((rbuf != NULL)&& (rlen != NULL)) {
        tmp8 = mfrc522_bal_read_reg(FIFOLevelReg);
        if (tmp8 > *rlen) return STATUS_NO_ROOM;
        *rlen = tmp8;
        mfrc522_bal_read_fifo_len_ex(FIFODataReg, rbuf, tmp8, rxAlign);
        _validBits = mfrc522_bal_read_reg(ControlReg) & 0x07;

        if (validBits != NULL) {
            *validBits = _validBits;
        }
    }
    if (errorRegValue & 0x08) return STATUS_COLLISION;
    if ((rbuf != NULL) && (rlen != NULL) && checkCRC) {
        if (*rlen == 1 && _validBits == 4) return STATUS_MIFARE_NACK;
        if (*rlen < 2 || _validBits != 0) return STATUS_CRC_WRONG;
        uint8_t controlBuffer[2];
        em_PcdStatusCode status = mfrc522_hal_calculate_crc(rbuf, *rlen - 2, controlBuffer);
        if (status != STATUS_OK) return status;

        if ((rbuf[*rlen - 2] != controlBuffer[0]) || (rbuf[*rlen - 1] != controlBuffer[1])) {
            return STATUS_CRC_WRONG;
        }
    }
    return STATUS_OK;

}
/*
em_PcdStatusCode picc_requset_a(uint8_t *bufferATQA, uint8_t *bufferSize) {
    return picc_req_a_or_wakeup_a(PICC_CMD_REQA, bufferATQA, bufferSize);
}

em_PcdStatusCode picc_wakeup_a(uint8_t *bufferATQA, uint8_t *bufferSize) {
    return picc_req_a_or_wakeup_a(PICC_CMD_WUPA, bufferATQA, bufferSize);
}
*/
em_PcdStatusCode picc_req_a_or_wakeup_a( uint8_t cmd, uint8_t *bufferATQA, uint8_t *bufferSize) {
    uint8_t validBits;
    em_PcdStatusCode status;
    uint8_t __cmd=cmd;
    //cmd=cmd;
    if (bufferATQA == NULL || *bufferSize < 2) {
        return STATUS_NO_ROOM;
    }
    mfrc522_bal_clr_bit_reg(CollReg, 0x80);
    validBits = 7;
    //em_PcdStatusCode pcd_transceive(uint8_t *sbuf,uint8_t slen,uint8_t *rbuf,uint8_t *rlen,uint8_t *validBits,uint8_t rxAlign,bool checkCRC);
    //status = pcd_transceive(&cmd, 1, bufferATQA, bufferSize, &validBits, 0, false);
    status=pcd_comm_with_picc(PCD_Transceive,0x30,&__cmd, 1, bufferATQA, bufferSize, &validBits, 0, false);
    if (status != STATUS_OK) return status;
    if (*bufferSize != 2 || validBits != 0) return STATUS_ERROR;
    return STATUS_OK;
}
//uint8_t buffer[9];
em_PcdStatusCode picc_select(st_PiccUid *uid, uint8_t validBits) {
    bool uidComplete;
    bool selectDone;
    bool useCascadeTag;
    uint8_t cascadeLevel = 1;
    em_PcdStatusCode result;
    uint8_t count;
    uint8_t index;
    uint8_t uidIndex;
    uint8_t currentLevelKnownBits;
    uint8_t buffer[9];
    uint8_t bufferUsed;
    uint8_t rxAlign;
    uint8_t txLastBits;
    uint8_t *responseBuffer;
    uint8_t responseLength;


    if (validBits > 80) return STATUS_INVALID;
    mfrc522_bal_clr_bit_reg(CollReg, 0x80);
    uidComplete = false;
    while (!uidComplete) {
        if (cascadeLevel == 1) {
            buffer[0] = PICC_CMD_SEL_CL1;
            uidIndex = 0;
            useCascadeTag = validBits && uid->size > 4;
        } else if (cascadeLevel == 2) {
            buffer[0] = PICC_CMD_SEL_CL2;
            uidIndex = 3;
            useCascadeTag = validBits && uid->size > 7;
        } else if (cascadeLevel == 3) {
            buffer[0] = PICC_CMD_SEL_CL3;
            uidIndex = 6;
            useCascadeTag = false;
        } else {
            return STATUS_INTERNAL_ERROR;
        }

        if (validBits < (8 * uidIndex)) {
            currentLevelKnownBits = 0;
        } else {
            currentLevelKnownBits = validBits - (8 * uidIndex);
        }
        index = 2;
        if (useCascadeTag) {
            buffer[index++] = PICC_CMD_CT;
        }
        uint8_t bytesToCopy;
        bytesToCopy = currentLevelKnownBits / 8 + (currentLevelKnownBits % 8 ? 1 : 0);
        if (bytesToCopy) {
            uint8_t maxBytes = useCascadeTag ? 3 : 4;
            if (bytesToCopy > maxBytes) {
                bytesToCopy = maxBytes;
            }
            for (count = 0; count < bytesToCopy; count++) {
                buffer[index++] = uid->uidByte[uidIndex + count];
            }
        }
        if (useCascadeTag) {
            currentLevelKnownBits += 8;
        }

        selectDone = false;
        while (!selectDone) {
            if (currentLevelKnownBits >= 32) {

                buffer[1] = 0x70;
                buffer[6] = buffer[2] ^ buffer[3] ^ buffer[4] ^ buffer[5];
                result = mfrc522_hal_calculate_crc(buffer, 7, &buffer[7]);
                if (result != STATUS_OK) {
                    return result;
                }
                txLastBits = 0;
                bufferUsed = 9;

                responseBuffer = &buffer[6];
                responseLength = 3;
            } else {
                txLastBits = currentLevelKnownBits % 8;
                count = currentLevelKnownBits / 8;
                index = 2 + count;
                buffer[1] = (index << 4) + txLastBits;
                bufferUsed = index + (txLastBits ? 1 : 0);
                responseBuffer = &buffer[index];
                responseLength = sizeof (buffer) - index;
            }

            rxAlign = txLastBits;
            mfrc522_bal_write_reg(BitFramingReg, (rxAlign << 4) + txLastBits);

            //result = pcd_transceive(buffer, bufferUsed, responseBuffer, &responseLength, &txLastBits, rxAlign, false);
           result = pcd_comm_with_picc(PCD_Transceive,0x30,buffer, bufferUsed, responseBuffer, &responseLength, &txLastBits, rxAlign, false);
            if (result == STATUS_COLLISION) {
                uint8_t valueOfCollReg = mfrc522_bal_read_reg(CollReg);
                if (valueOfCollReg & 0x20) {
                    return STATUS_COLLISION;
                }
                uint8_t collisionPos = valueOfCollReg & 0x1F;
                if (collisionPos == 0) {
                    collisionPos = 32;
                }
                if (collisionPos <= currentLevelKnownBits) {
                    return STATUS_INTERNAL_ERROR;
                }
                currentLevelKnownBits = collisionPos;
                count = (currentLevelKnownBits - 1) % 8;
                index = 1 + (currentLevelKnownBits / 8) + (count ? 1 : 0);
                buffer[index] |= (1 << count);
            } else if (result != STATUS_OK) {
                return result;
            } else {
                if (currentLevelKnownBits >= 32) {
                    selectDone = true;
                } else {
                    currentLevelKnownBits = 32;
                }
            }
        }
        index = (buffer[2] == PICC_CMD_CT) ? 3 : 2;
        bytesToCopy = (buffer[2] == PICC_CMD_CT) ? 3 : 4;
        for (count = 0; count < bytesToCopy; count++) {
            uid->uidByte[uidIndex + count] = buffer[index++];
        }
        if (responseLength != 3 || txLastBits != 0) {
            return STATUS_ERROR;
        }
        result = mfrc522_hal_calculate_crc(responseBuffer, 1, &buffer[2]);
        if (result != STATUS_OK) {
            return result;
        }
        if ((buffer[2] != responseBuffer[1]) || (buffer[3] != responseBuffer[2])) {
            return STATUS_CRC_WRONG;
        }
        if (responseBuffer[0] & 0x04) {
            cascadeLevel++;
        } else {
            uidComplete = true;
            //__no_operation();
            uid->sak = responseBuffer[0];
            //__no_operation();
        }
    }
    uid->size = 3 * cascadeLevel + 1;
    return STATUS_OK;
}

em_PcdStatusCode picc_halt_a(void) {
    em_PcdStatusCode result;
    uint8_t buffer[4];

    buffer[0] = PICC_CMD_HLTA;
    buffer[1] = 0;
    result = mfrc522_hal_calculate_crc(buffer, 2, &buffer[2]);
    if (result != STATUS_OK)return result;

    //result = pcd_transceive(buffer, sizeof (buffer), NULL, null, null, 0, false);
     result = pcd_comm_with_picc(PCD_Transceive,0x30,buffer, sizeof (buffer), NULL, null, null, 0, false);
    if (result == STATUS_TIMEOUT) return STATUS_OK;
    if (result == STATUS_OK) return STATUS_ERROR;
    return result;
}

em_PcdStatusCode picc_authenticate(uint8_t command, uint8_t blockAddr, st_MifareKey *key, st_PiccUid *uid) {
    uint8_t waitIRQ = 0x10;
    uint8_t i;
    uint8_t sendData[12];
    sendData[0] = command;
    sendData[1] = blockAddr;
    for (i = 0; i < MF_KEY_SIZE; i++) {
        sendData[2 + i] = key->keyByte[i];
    }
    for (i = 0; i < 4; i++) {
        sendData[8 + i] = uid->uidByte[i];
    }
    return pcd_comm_with_picc(PCD_MFAuthent, waitIRQ, &sendData[0], sizeof (sendData), null, null, null, 0, false);
}

/**
 * Used to exit the PCD from its authenticated state.
 * Remember to call this function after communicating with an authenticated PICC - 
 * otherwise no new communications can start.
 */
void picc_stop_cryptol(void) {
    mfrc522_bal_clr_bit_reg(Status2Reg, 0x08);
}


