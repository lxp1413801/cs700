//file name:mfrc522_hal.c

//add other header file
//...
#include "../mfrc522_config.h"
#include "../mfrc522_includes.h"
#include"../../app/appdata.h"

//declear some macro
/*
const uint8_t MFRC522_firmware_referenceV0_0[]= {
	0x00, 0x87, 0x98, 0x0f, 0x49, 0xFF, 0x07, 0x19,
	0xBF, 0x22, 0x30, 0x49, 0x59, 0x63, 0xAD, 0xCA,
	0x7F, 0xE3, 0x4E, 0x03, 0x5C, 0x4E, 0x49, 0x50,
	0x47, 0x9A, 0x37, 0x61, 0xE7, 0xE2, 0xC6, 0x2E,
	0x75, 0x5A, 0xED, 0x04, 0x3D, 0x02, 0x4B, 0x78,
	0x32, 0xFF, 0x58, 0x3B, 0x7C, 0xE9, 0x00, 0x94,
	0xB4, 0x4A, 0x59, 0x5B, 0xFD, 0xC9, 0x29, 0xDF,
	0x35, 0x96, 0x98, 0x9E, 0x4F, 0x30, 0x32, 0x8D
};

// Version 1.0 (0x91)
// NXP Semiconductors; Rev. 3.8 - 17 September 2014; 16.1.1 Self test
const uint8_t MFRC522_firmware_referenceV1_0[]= {
	0x00, 0xC6, 0x37, 0xD5, 0x32, 0xB7, 0x57, 0x5C,
	0xC2, 0xD8, 0x7C, 0x4D, 0xD9, 0x70, 0xC7, 0x73,
	0x10, 0xE6, 0xD2, 0xAA, 0x5E, 0xA1, 0x3E, 0x5A,
	0x14, 0xAF, 0x30, 0x61, 0xC9, 0x70, 0xDB, 0x2E,
	0x64, 0x22, 0x72, 0xB5, 0xBD, 0x65, 0xF4, 0xEC,
	0x22, 0xBC, 0xD3, 0x72, 0x35, 0xCD, 0xAA, 0x41,
	0x1F, 0xA7, 0xF3, 0x53, 0x14, 0xDE, 0x7E, 0x02,
	0xD9, 0x0F, 0xB5, 0x5E, 0x25, 0x1D, 0x29, 0x79
};

// Version 2.0 (0x92)
// NXP Semiconductors; Rev. 3.8 - 17 September 2014; 16.1.1 Self test
const uint8_t MFRC522_firmware_referenceV2_0[]= {
	0x00, 0xEB, 0x66, 0xBA, 0x57, 0xBF, 0x23, 0x95,
	0xD0, 0xE3, 0x0D, 0x3D, 0x27, 0x89, 0x5C, 0xDE,
	0x9D, 0x3B, 0xA7, 0x00, 0x21, 0x5B, 0x89, 0x82,
	0x51, 0x3A, 0xEB, 0x02, 0x0C, 0xA5, 0x00, 0x49,
	0x7C, 0x84, 0x4D, 0xB3, 0xCC, 0xD2, 0x1B, 0x81,
	0x5D, 0x48, 0x76, 0xD5, 0x71, 0x61, 0x21, 0xA9,
	0x86, 0x96, 0x83, 0x38, 0xCF, 0x9D, 0x5B, 0x6D,
	0xDC, 0x15, 0xBA, 0x3E, 0x7D, 0x95, 0x3B, 0x2F
};

// Clone
// Fudan Semiconductor FM17522 (0x88)
const uint8_t FM17522_firmware_reference[]= {
	0x00, 0xD6, 0x78, 0x8C, 0xE2, 0xAA, 0x0C, 0x18,
	0x2A, 0xB8, 0x7A, 0x7F, 0xD3, 0x6A, 0xCF, 0x0B,
	0xB1, 0x37, 0x63, 0x4B, 0x69, 0xAE, 0x91, 0xC7,
	0xC3, 0x97, 0xAE, 0x77, 0xF4, 0x37, 0xD7, 0x9B,
	0x7C, 0xF5, 0x3C, 0x11, 0x8F, 0x15, 0xC3, 0xD7,
	0xC1, 0x5B, 0x00, 0x2A, 0xD0, 0x75, 0xDE, 0x9E,
	0x51, 0x64, 0xAB, 0x3E, 0xE9, 0x15, 0xB5, 0xAB,
	0x56, 0x9A, 0x98, 0x82, 0x26, 0xEA, 0x2A, 0x62
};
*/
#ifndef MFRC522_DELAY_10MS
    #warning "MFRC522_DELAY_10MS must define in mfrc522_config.h!!"
    #warning "MFRC522_DELAY_10MS The definition depends on processor clock!!"
    #define MFRC522_DELAY_10MS 2000
#endif


em_PcdStatusCode mfrc522_hal_calculate_crc(uint8_t *data, uint8_t length, uint8_t *result)
{
	uint16_t i = MFRC522_DELAY_10MS;
	uint8_t t8;
	mfrc522_bal_write_reg(CommandReg, PCD_Idle);
	mfrc522_bal_write_reg(DivIrqReg, 0x04);
	mfrc522_bal_set_bit_reg(FIFOLevelReg, 0x80);
	mfrc522_bal_write_reg_len(FIFODataReg,data,length);
	mfrc522_bal_write_reg(CommandReg, PCD_CalcCRC);
	while(i) 
	{
        i--;
		t8 = mfrc522_bal_read_reg(DivIrqReg);
		if (t8 & 0x04)break;			
	}
	if(i==0)return STATUS_TIMEOUT;
	mfrc522_bal_write_reg(CommandReg, PCD_Idle);
	result[0] = mfrc522_bal_read_reg(CRCResultRegL);
	result[1] = mfrc522_bal_read_reg(CRCResultRegH);
	return STATUS_OK;		
}


#ifndef mfrc522_delay_10ms
    #warning "mfrc522_delay_10ms must by define or declear in file mrfc_config.h."
    #warning "In order to remove the compiler error prompt, in flowing,"
    #warning "mfrc522_delay_10ms will be defined ';'!!!,but echo. can't used!!!"

    #define mfrc522_delay_10ms() ;
#endif

void mfrc522_hal_init(void)
{
    
	mfrc522_bal_port_init(); 
	//add 2016.08.23
	mfrc522_power_on();
    mfrc522_delay_10ms();
	//
	mfrc522_bal_rst_set_low();
	mfrc522_delay_10ms();
	mfrc522_bal_rst_set_hight();
	
	mfrc522_delay_10ms();
    mfrc522_hal_reset_sf();
    mfrc522_delay_10ms();
	
    
	mfrc522_bal_write_reg(TModeReg, 0x80);
	mfrc522_bal_write_reg(TPrescalerReg, 0xA9);
	mfrc522_bal_write_reg(TReloadRegH, 0x04);
	mfrc522_bal_write_reg(TReloadRegL, 0xE8);	
	mfrc522_bal_write_reg(TxASKReg, 0x40);
	mfrc522_bal_write_reg(ModeReg, 0x3D);
	mfrc522_delay_10ms();
	mfrc522_hal_set_antenna_gain(RxGain_max);//RxGain_33dB,RxGain_max
    mfrc522_hal_antenna_on();
    mfrc522_delay_10ms();	
}

void mfrc522_hal_reset_sf(void)
{
	mfrc522_bal_write_reg(CommandReg, PCD_SoftReset);	// Issue the SoftReset command.
	mfrc522_delay_100ms();
	while (mfrc522_bal_read_reg(CommandReg) & (1<<4)) 
	{
	}		
}

void mfrc522_hal_antenna_on(void)
{
	uint8_t t8;
	t8= mfrc522_bal_read_reg(TxControlReg);
	if((t8&0x03)!=0x03) 
	{
		mfrc522_bal_write_reg(TxControlReg, t8 | 0x03);
	}		
}
void mfrc522_hal_antenna_off(void)
{
	mfrc522_bal_clr_bit_reg(TxControlReg, 0x03);	
}

uint8_t mfrc522_hal_get_antenna_gain(void)
{
	uint8_t ret=0;
	ret=mfrc522_bal_read_reg(RFCfgReg);
	ret &= (0x07<<4);
	return  ret;	
}

void mfrc522_hal_set_antenna_gain(uint8_t gain)
{
	if (mfrc522_hal_get_antenna_gain() != gain) 
	{
		mfrc522_bal_clr_bit_reg(RFCfgReg, (0x07<<4));
		mfrc522_bal_set_bit_reg(RFCfgReg, gain & (0x07<<4));
	}		
}
/*
 
 uint16_t mfrc522_hal_self_test(void)
{

	uint8_t result[64]={0};
	uint16_t i;
	uint8_t tmp8;
	uint8_t* reference; 
	mfrc522_hal_reset_sf();

	mfrc522_bal_set_bit_reg(FIFOLevelReg, 0x80);
	mfrc522_bal_write_reg_len(FIFODataReg, result, 25);
	mfrc522_bal_write_reg(CommandReg, PCD_Mem);
	mfrc522_bal_write_reg(AutoTestReg, 0x09);
	mfrc522_bal_write_reg(FIFODataReg, 0x00);
	mfrc522_bal_write_reg(CommandReg, PCD_CalcCRC);

	for (i = 0; i < 0xFF; i++) 
	{
		tmp8 = mfrc522_bal_read_reg(DivIrqReg);
		if (tmp8 & 0x04)break;
	}
	mfrc522_bal_write_reg(CommandReg, PCD_Idle);
	mfrc522_bal_read_fifo_len_ex(FIFODataReg, result,64,0);
	mfrc522_bal_write_reg(AutoTestReg, 0x00);	
	tmp8 = mfrc522_bal_read_reg(VersionReg);	
	switch (tmp8) 
	{
		case 0x88:	
			reference = (uint8_t*)FM17522_firmware_reference;
			break;
		case 0x90:	
			reference = (uint8_t*)MFRC522_firmware_referenceV0_0;
			break;
		case 0x91:	
			reference = (uint8_t*)MFRC522_firmware_referenceV1_0;
			break;
		case 0x92:	
			reference = (uint8_t*)MFRC522_firmware_referenceV2_0;
			break;
		default:	
			return false; 
	}	

	tmp8=m_str_cmp_len(reference,result,64);
    
	return tmp8;	
}
 
uint16_t mfrc522_hal_self_test(void)
{

	//uint8_t result[64]={0};
	uint16_t i;
	uint8_t tmp8;
	uint8_t* reference; 
	mfrc522_hal_reset_sf();

	mfrc522_bal_set_bit_reg(FIFOLevelReg, 0x80);
	mfrc522_bal_write_reg_len(FIFODataReg, comBuffer, 25);
	mfrc522_bal_write_reg(CommandReg, PCD_Mem);
	mfrc522_bal_write_reg(AutoTestReg, 0x09);
	mfrc522_bal_write_reg(FIFODataReg, 0x00);
	mfrc522_bal_write_reg(CommandReg, PCD_CalcCRC);

	for (i = 0; i < 0xFF; i++) 
	{
		tmp8 = mfrc522_bal_read_reg(DivIrqReg);
		if (tmp8 & 0x04)break;
	}
	mfrc522_bal_write_reg(CommandReg, PCD_Idle);
	mfrc522_bal_read_fifo_len_ex(FIFODataReg, comBuffer,64,0);
	mfrc522_bal_write_reg(AutoTestReg, 0x00);	
	tmp8 = mfrc522_bal_read_reg(VersionReg);	
	switch (tmp8) 
	{
		case 0x88:	
			reference = (uint8_t*)FM17522_firmware_reference;
			break;
		case 0x90:	
			reference = (uint8_t*)MFRC522_firmware_referenceV0_0;
			break;
		case 0x91:	
			reference = (uint8_t*)MFRC522_firmware_referenceV1_0;
			break;
		case 0x92:	
			reference = (uint8_t*)MFRC522_firmware_referenceV2_0;
			break;
		default:	
			return false; 
	}	

	tmp8=m_str_cmp_len(reference,comBuffer,64);
    
	return tmp8;	
}*/
//file end 
//


