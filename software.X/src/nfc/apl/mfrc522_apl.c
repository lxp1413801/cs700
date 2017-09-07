//file name:mfrc522_apl.c

//add other header file
//...
#include "../mfrc522_config.h"
#include "../mfrc522_includes.h"

st_PiccUid PiccUid;
//bool pcdPwrOn = false;
//bool mfapiLibInit=false;
/*
em_PiccType mfapi_get_picc_type(uint8_t sak) {
    sak &= 0x7F;
    switch (sak) {
        case 0x04:
            return PICC_TYPE_NOT_COMPLETE;
        case 0x09:
            return PICC_TYPE_MIFARE_MINI;
        case 0x08:
            return PICC_TYPE_MIFARE_1K;
        case 0x18:
            return PICC_TYPE_MIFARE_4K;
        case 0x00:
            return PICC_TYPE_MIFARE_UL;
        case 0x10:
        case 0x11:
            return PICC_TYPE_MIFARE_PLUS;
        case 0x01:
            return PICC_TYPE_TNP3XXX;
        case 0x20:
            return PICC_TYPE_ISO_14443_4;
        case 0x40:
            return PICC_TYPE_ISO_18092;
        default:
            return PICC_TYPE_UNKNOWN;
    }
}

uint16_t mfapi_get_picc_sectors(em_PiccType type) {
    switch (type) {
        case PICC_TYPE_MIFARE_MINI:
            return 5;
        case PICC_TYPE_MIFARE_1K:
            return 16;
        case PICC_TYPE_MIFARE_4K:
            return 40;
        default:
            return 0;
    }
}

bool mfapi_lib_init(void) {
    //mfapiLibInit=true;
    return true;
}

uint8_t* mfapi_get_pcd_version(uint8_t* version) {
    uint8_t v = mfrc522_bal_read_reg(VersionReg);
    if (version != null)*version = v;
    return null;
}
*/
bool mfapi_pcd_init(void) {
    uint8_t v;
    mfrc522_hal_init();
    //mfapi_pcd_self_test();
    //mfrc522_hal_self_test();
    //mfapi_get_pcd_version(&v);
    v = mfrc522_bal_read_reg(VersionReg);
    if (!((v == 0x90) || (v == 0x91) || (v == 0x92))) {
        return false;
    }
    return true;
}

bool mfapi_is_new_card_present(void) {
    uint8_t cmd;
    uint8_t bufferATQA[2];
    uint8_t bufferSize = sizeof (bufferATQA);
    em_PcdStatusCode result;
    m_mem_set((uint8_t*) (&PiccUid), 0, sizeof (PiccUid));
    ; // = picc_requset_a(bufferATQA, &bufferSize);
    cmd = PICC_CMD_REQA;
    result = picc_req_a_or_wakeup_a(cmd, bufferATQA, &bufferSize);
    if (result == STATUS_OK || result == STATUS_COLLISION) {
        return true;
    }
    return false;
}
/*
bool mfapi_anti_collision_and_select(st_PiccUid* uid) {
    uint8_t sak;
    em_PiccType type;
    em_PcdStatusCode result;
    result = picc_select(uid, 0);
    sak = uid->sak;
    if (result != STATUS_OK)return false;
    type = mfapi_get_picc_type(sak);

    if (type == PICC_TYPE_MIFARE_1K) {
        return true;
    }
    picc_halt_a();
    return false;
}

void mfapi_get_picc_serial_from_st(st_PiccUid* uid, uint8_t* serial, uint8_t* size) {
    if (uid == (st_PiccUid*) null)return;
    *size = (uint8_t) uid->size;
    m_mem_cpy_len(serial, uid->uidByte, *size);
}
 */
/*
bool mfapi_authenticate_block
(
        st_PiccUid* uid,
        st_MifareKey* key,
        uint16_t block,
        bool keybuse
        ) {
    em_PcdStatusCode status;
    uint8_t sectortrailer;
    if (block > 63)return false;
    if ((block % 4) == 3)sectortrailer = block;
    else
        sectortrailer = (((uint8_t) block & ~0x03) + 3);

    if (keybuse)status = picc_authenticate(PICC_CMD_MF_AUTH_KEY_B, sectortrailer, key, uid);
    else
        status = picc_authenticate(PICC_CMD_MF_AUTH_KEY_A, sectortrailer, key, uid);
    if (status == STATUS_OK)return true;
    return false;
}

bool mfapi_read_block_only(uint8_t block, uint8_t* buf, uint8_t* readlen) {
    em_PcdStatusCode status;
    status = mifare_read(block, buf, readlen);
    if (status == STATUS_OK)return true;
    return false;
}

bool mfapi_write_block_only(uint8_t block, uint8_t* buf, uint8_t wrlen) {
    em_PcdStatusCode status;
    status = mifare_write(block, buf, wrlen);
    if (status == STATUS_OK)return true;
    return false;
}

bool mfapi_set_value(uint8_t block, int32_t value) {

    em_PcdStatusCode status;
    status = mifare_set_value(block, value);
    if (status == STATUS_OK)return true;
    return false;
}

bool mfapi_get_value(uint8_t block, int32_t *value) {

    em_PcdStatusCode status;
    status = mifare_get_value(block, value);
    if (status == STATUS_OK)return true;
    return false;
}

bool mfapi_increment(uint8_t blockAddr, int32_t delta) {
    em_PcdStatusCode status;
    status = mifare_two_Step_helper(PICC_CMD_MF_INCREMENT, blockAddr, delta);
    if (status != STATUS_OK)return false;
    status = mifare_transfer(blockAddr);
    if (status == STATUS_OK)
        return true;
    return false;
}

bool mfapi_decrement(uint8_t blockAddr, int32_t delta) {
    em_PcdStatusCode status;
    status = mifare_two_Step_helper(PICC_CMD_MF_DECREMENT, blockAddr, delta);
    if (status != STATUS_OK)return false;
    status = mifare_transfer(blockAddr);
    if (status == STATUS_OK)
        return true;
    return false;
}

bool mfapi_read_block
(
        st_PiccUid* uid,
        st_MifareKey* key,
        uint8_t* buf,
        uint8_t* trailer,
        uint8_t block,
        bool keybuse
        ) {
    uint8_t readlen = 18;
    uint8_t sectortrailer;
    em_PcdStatusCode status;

    if (block > 63)return false;
    if ((block % 4) == 3)sectortrailer = block;
    else
        sectortrailer = (block & ~0x03) + 3;

    if (keybuse)status = picc_authenticate(PICC_CMD_MF_AUTH_KEY_B, sectortrailer, key, uid);
    else
        status = picc_authenticate(PICC_CMD_MF_AUTH_KEY_A, sectortrailer, key, uid);

    if (status != STATUS_OK) {
        mfapi_end_operation();
        return false;
    }
    if (trailer != null) {
        status = mifare_read(sectortrailer, trailer, &readlen);
        if (status != STATUS_OK) {
            mfapi_end_operation();
            return false;
        }
    }

    if (block != sectortrailer || (block == sectortrailer && trailer == null)) {
        status = mifare_read(block, buf, &readlen);
        if (status != STATUS_OK) {
            mfapi_end_operation();
            return false;
        }

    } else {
        m_mem_cpy_len(buf, trailer, 18); //this is a bug
    }
    mfapi_end_operation();
    return true;

}

*/

