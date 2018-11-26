//file name:mfrc522_picc.c

//add other header file
//...
#include "../mfrc522_config.h"
#include "../mfrc522_includes.h"

em_PcdStatusCode mifare_two_Step_helper
(	
	uint8_t command,
	uint8_t blockAddr,
	int32_t data
) 
{
	em_PcdStatusCode result;
	uint8_t cmdBuffer[2];
	
	cmdBuffer[0] = command;
	cmdBuffer[1] = blockAddr;
	result = pcd_mifare_transceive(	cmdBuffer, 2,false);
	if (result != STATUS_OK) 
	{
		return result;
	}
	

	result = pcd_mifare_transceive(	(uint8_t *)&data, 4, true);
	if (result != STATUS_OK) 
	{
		return result;
	}	
	return STATUS_OK;
}

em_PcdStatusCode mifare_read
(
uint8_t blockAddr, 
uint8_t *buffer, 
uint8_t *bufferSize
)
{
	em_PcdStatusCode result;
	if (buffer == NULL || *bufferSize < 18) 
	{
		return STATUS_NO_ROOM;
	}
	buffer[0] = PICC_CMD_MF_READ;
	buffer[1] = blockAddr;
	result = mfrc522_hal_calculate_crc(buffer, 2, &buffer[2]);
	if (result != STATUS_OK) {
		return result;
	}
	//return pcd_transceive(buffer, 4, buffer, bufferSize, NULL, 0, true);	
   return  pcd_comm_with_picc(PCD_Transceive,0x30,buffer, 4, buffer, bufferSize, NULL, 0, true);
}

em_PcdStatusCode mifare_write
(
	uint8_t blockAddr, 
	uint8_t *buffer, 
	uint8_t bufferSize
)
{
	em_PcdStatusCode result;	
	if (buffer == NULL || bufferSize < 16) 
	{
		return STATUS_INVALID;
	}
	uint8_t cmdBuffer[2];
	cmdBuffer[0] = PICC_CMD_MF_WRITE;
	cmdBuffer[1] = blockAddr;
	result = pcd_mifare_transceive(cmdBuffer, 2,false); 
	if (result != STATUS_OK) 
	{
		return result;
	}
	result = pcd_mifare_transceive(buffer, bufferSize,false); 
	if (result != STATUS_OK) 
	{
		return result;
	}	
	return STATUS_OK;	
}
/*
em_PcdStatusCode mifare_ultralight_write
(
	uint8_t page, 
	uint8_t *buffer, 
	uint8_t bufferSize
)
{
	em_PcdStatusCode result;
	if (buffer == NULL || bufferSize < 4) 
	{
		return STATUS_INVALID;
	}

	uint8_t cmdBuffer[6];
	cmdBuffer[0] = PICC_CMD_UL_WRITE;
	cmdBuffer[1] = page;
	m_mem_cpy_len(&cmdBuffer[2], buffer, 4);

	result = pcd_mifare_transceive(cmdBuffer, 6,0); 
	if (result != STATUS_OK) 
    {
		return result;
	}
	return STATUS_OK;	
}

em_PcdStatusCode mifare_decrement(uint8_t blockAddr,int32_t delta)
{
	return mifare_two_Step_helper(PICC_CMD_MF_DECREMENT, blockAddr, delta);
}

em_PcdStatusCode mifare_increment(uint8_t blockAddr, int32_t delta)
{
	return mifare_two_Step_helper(PICC_CMD_MF_INCREMENT, blockAddr, delta);
	
}

em_PcdStatusCode mifare_restore(uint8_t blockAddr)
{
	return mifare_two_Step_helper(PICC_CMD_MF_RESTORE, blockAddr, 0L);
}
*/
em_PcdStatusCode pcd_mifare_transceive
(	
	uint8_t *sendData,
	uint8_t sendLen,
	bool acceptTimeout
) 
{
	em_PcdStatusCode result;
	uint8_t cmdBuffer[18];
	

	if (sendData == NULL || sendLen > 16) 
	{
		return STATUS_INVALID;
	}
	

	m_mem_cpy_len(cmdBuffer, sendData, sendLen);
	result = mfrc522_hal_calculate_crc(cmdBuffer, sendLen, &cmdBuffer[sendLen]);
	if (result != STATUS_OK) 
	{ 
		return result;
	}
	sendLen += 2;
	

	uint8_t waitIRq = 0x30;		
	uint8_t cmdBufferSize = sizeof(cmdBuffer);
	uint8_t validBits = 0;
	result = pcd_comm_with_picc(PCD_Transceive, waitIRq, cmdBuffer, sendLen, cmdBuffer, &cmdBufferSize, &validBits,0,false);
	if (acceptTimeout && result == STATUS_TIMEOUT) 
	{
		return STATUS_OK;
	}
	if (result != STATUS_OK) 
	{
		return result;
	}
	
	if (cmdBufferSize != 1 || validBits != 4) 
	{
		return STATUS_ERROR;
	}
	if (cmdBuffer[0] != MF_ACK) 
	{
		return STATUS_MIFARE_NACK;
	}
	return STATUS_OK;
}
/*
em_PcdStatusCode mifare_transfer(uint8_t blockAddr)
{
	em_PcdStatusCode result;
	uint8_t cmdBuffer[2];
	
	cmdBuffer[0] = PICC_CMD_MF_TRANSFER;
	cmdBuffer[1] = blockAddr;
	result = pcd_mifare_transceive(cmdBuffer, 2,false);
	if (result != STATUS_OK) 
	{
		return result;
	}
	return STATUS_OK;	
}

em_PcdStatusCode mifare_get_value(uint8_t blockAddr, int32_t *value)
{
	
	em_PcdStatusCode status;
	uint8_t buffer[18];
	uint8_t size = sizeof(buffer);
	status = mifare_read(blockAddr, buffer, &size);
	if (status == STATUS_OK) {
		*value = ((int32_t)(buffer[3])<<24) | 	\
				((int32_t)(buffer[2])<<16) |		\
				((int32_t)(buffer[1])<<8) | 		\
				(int32_t)(buffer[0]);
	}
	return status;	
}
em_PcdStatusCode mifare_set_value(uint8_t blockAddr, int32_t value)
{
	uint8_t buffer[18];
	
	buffer[0] = buffer[ 8] = (value & 0xFF);
	buffer[1] = buffer[ 9] = (value & 0xFF00) >> 8;
	buffer[2] = buffer[10] = (value & 0xFF0000) >> 16;
	buffer[3] = buffer[11] = (value & 0xFF000000) >> 24;
	buffer[4] = ~buffer[0];
	buffer[5] = ~buffer[1];
	buffer[6] = ~buffer[2];
	buffer[7] = ~buffer[3];
	buffer[12] = buffer[14] = blockAddr;
	buffer[13] = buffer[15] = ~blockAddr;
	
	return mifare_write(blockAddr, buffer, 16);	
}

em_PcdStatusCode picc_ntag216_auth(uint8_t *passWord, uint8_t* pACK)
{
	em_PcdStatusCode result;
	uint16_t i;
	uint8_t cmdBuffer[18]; 
	
	cmdBuffer[0] = 0x1B; 
	
	for (i = 0; i<4; i++)
		cmdBuffer[i+1] = passWord[i];
	
	result = mfrc522_hal_calculate_crc(cmdBuffer, 5, &cmdBuffer[5]);
	
	if (result!=STATUS_OK)
	{
		return result;
	}

	uint8_t waitIRq= 0x30;	
	uint8_t validBits= 0;
	uint8_t rxlength= 5;
	result = pcd_comm_with_picc(PCD_Transceive, waitIRq, cmdBuffer, 7, cmdBuffer, &rxlength, &validBits,0,false);
	
	pACK[0] = cmdBuffer[0];
	pACK[1] = cmdBuffer[1];
	
	if (result!=STATUS_OK) 
	{
		return result;
	}
	return STATUS_OK;	
}


void mifare_set_access_bits
(
uint8_t *accessBitBuffer, 
uint8_t g0, 
uint8_t g1, 
uint8_t g2, 
uint8_t g3)
{
	uint8_t c1 = ((g3 & 4) << 1) | ((g2 & 4) << 0) | ((g1 & 4) >> 1) | ((g0 & 4) >> 2);
	uint8_t c2 = ((g3 & 2) << 2) | ((g2 & 2) << 1) | ((g1 & 2) << 0) | ((g0 & 2) >> 1);
	uint8_t c3 = ((g3 & 1) << 3) | ((g2 & 1) << 2) | ((g1 & 1) << 1) | ((g0 & 1) << 0);
	
	accessBitBuffer[0] = (~c2 & 0xF) << 4 | (~c1 & 0xF);
	accessBitBuffer[1] =          c1 << 4 | (~c3 & 0xF);
	accessBitBuffer[2] =          c3 << 4 | c2;
	
}



bool mifare_extract_access_form_buffer(uint8_t* buf,uint8_t* access)
{
	uint8_t _access[4];
	st_MifareAeccessFormat*  p_st=(st_MifareAeccessFormat*)buf;
	access[0]=((p_st->bits.C1X0)<<2) + ((p_st->bits.C2X0)<<1) + (p_st->bits.C3X0);
	access[1]=((p_st->bits.C1X1)<<2) + ((p_st->bits.C2X1)<<1) + (p_st->bits.C3X1);
	access[2]=((p_st->bits.C1X2)<<2) + ((p_st->bits.C2X2)<<1) + (p_st->bits.C3X2);
	access[3]=((p_st->bits.C1X3)<<2) + ((p_st->bits.C2X3)<<1) + (p_st->bits.C3X3);
	
	_access[0]=((p_st->bits.C1X0_b)<<2) + ((p_st->bits.C2X0_b)<<1) + (p_st->bits.C3X0_b);
	_access[1]=((p_st->bits.C1X1_b)<<2) + ((p_st->bits.C2X1_b)<<1) + (p_st->bits.C3X1_b);
	_access[2]=((p_st->bits.C1X2_b)<<2) + ((p_st->bits.C2X2_b)<<1) + (p_st->bits.C3X2_b);
	_access[3]=((p_st->bits.C1X3_b)<<2) + ((p_st->bits.C2X3_b)<<1) + (p_st->bits.C3X3_b);
	
	if(((access[0] ^ _access[0]) & 0x07) !=0x07)return false;
	if(((access[1] ^ _access[1]) & 0x07) !=0x07)return false;	
	if(((access[2] ^ _access[2]) & 0x07) !=0x07)return false;	
	if(((access[3] ^ _access[3]) & 0x07) !=0x07)return false;	
	return true;
}
bool mifare_extract_access_form_buffer_ex(uint8_t* buf,uint8_t* access,uint8_t index)
{
	uint8_t tmp,_access;
	st_MifareAeccessFormat*  p_st=(st_MifareAeccessFormat*)buf;
	switch(index)
	{
		case 0:
			tmp=	((p_st->bits.C1X0)<<2) + 	((p_st->bits.C2X0)<<1) + 	(p_st->bits.C3X0);
			_access=((p_st->bits.C1X0_b)<<2) + 	((p_st->bits.C2X0_b)<<1) + 	(p_st->bits.C3X0_b);
			break;
		case 1:
			tmp=	((p_st->bits.C1X1)<<2) + 	((p_st->bits.C2X1)<<1) + 	(p_st->bits.C3X1);
			_access=((p_st->bits.C1X1_b)<<2) + 	((p_st->bits.C2X1_b)<<1) + 	(p_st->bits.C3X1_b);
			break;
		case 2:
			tmp=	((p_st->bits.C1X2)<<2) + 	((p_st->bits.C2X2)<<1) + 	(p_st->bits.C3X2);
			_access=((p_st->bits.C1X2_b)<<2) + 	((p_st->bits.C2X2_b)<<1) + 	(p_st->bits.C3X2_b);
			break;
		case 3:
			tmp=	((p_st->bits.C1X3)<<2) + 	((p_st->bits.C2X3)<<1) + 	(p_st->bits.C3X3);
			_access=((p_st->bits.C1X3_b)<<2) + 	((p_st->bits.C2X3_b)<<1) + 	(p_st->bits.C3X3_b);
			break;
		default:
			return false;
	}
	*access=tmp;
	if(((tmp ^ _access & 0x07) !=0x07))return false;	
	return true;
}

bool mifare_combination_buffer_form_access(uint8_t* buf,uint8_t* access)
{
	uint8_t tmp;
	st_MifareAeccessFormat*  p_st=(st_MifareAeccessFormat*)buf;
	
	tmp=access[0];
	p_st->bits.C1X0=(tmp>>2) & 0x01;
	p_st->bits.C2X0=(tmp>>1) & 0x01;
	p_st->bits.C3X0=(tmp>>0) & 0x01;
	tmp=~tmp;
	p_st->bits.C1X0_b=(tmp>>2) & 0x01;
	p_st->bits.C2X0_b=(tmp>>1) & 0x01;
	p_st->bits.C3X0_b=(tmp>>0) & 0x01;	
	
	tmp=access[1];
	p_st->bits.C1X1=(tmp>>2) & 0x01;
	p_st->bits.C2X1=(tmp>>1) & 0x01;
	p_st->bits.C3X1=(tmp>>0) & 0x01;
	tmp=~tmp;
	p_st->bits.C1X1_b=(tmp>>2) & 0x01;
	p_st->bits.C2X1_b=(tmp>>1) & 0x01;
	p_st->bits.C3X1_b=(tmp>>0) & 0x01;	

	tmp=access[2];
	p_st->bits.C1X2=(tmp>>2) & 0x01;
	p_st->bits.C2X2=(tmp>>1) & 0x01;
	p_st->bits.C3X2=(tmp>>0) & 0x01;
	tmp=~tmp;
	p_st->bits.C1X2_b=(tmp>>2) & 0x01;
	p_st->bits.C2X2_b=(tmp>>1) & 0x01;
	p_st->bits.C3X2_b=(tmp>>0) & 0x01;		

	tmp=access[3];
	p_st->bits.C1X3=(tmp>>2) & 0x01;
	p_st->bits.C2X3=(tmp>>1) & 0x01;
	p_st->bits.C3X3=(tmp>>0) & 0x01;
	tmp=~tmp;
	p_st->bits.C1X3_b=(tmp>>2) & 0x01;
	p_st->bits.C2X3_b=(tmp>>1) & 0x01;
	p_st->bits.C3X3_b=(tmp>>0) & 0x01;	
	return true;
}

bool mifare_combination_buffer_form_access_ex(uint8_t* buf,uint8_t* access,uint8_t index)
{
	uint8_t tmp;
	st_MifareAeccessFormat*  p_st=(st_MifareAeccessFormat*)buf;
	if(index==0)
	{
		tmp=access[0];
		p_st->bits.C1X0=(tmp>>2) & 0x01;
		p_st->bits.C2X0=(tmp>>1) & 0x01;
		p_st->bits.C3X0=(tmp>>0) & 0x01;
		tmp=~tmp;
		p_st->bits.C1X0_b=(tmp>>2) & 0x01;
		p_st->bits.C2X0_b=(tmp>>1) & 0x01;
		p_st->bits.C3X0_b=(tmp>>0) & 0x01;	
	}
	else if(index==1)
	{
		tmp=access[1];
		p_st->bits.C1X1=(tmp>>2) & 0x01;
		p_st->bits.C2X1=(tmp>>1) & 0x01;
		p_st->bits.C3X1=(tmp>>0) & 0x01;
		tmp=~tmp;
		p_st->bits.C1X1_b=(tmp>>2) & 0x01;
		p_st->bits.C2X1_b=(tmp>>1) & 0x01;
		p_st->bits.C3X1_b=(tmp>>0) & 0x01;	
	}
	else if(index==2)
	{
		tmp=access[2];
		p_st->bits.C1X2=(tmp>>2) & 0x01;
		p_st->bits.C2X2=(tmp>>1) & 0x01;
		p_st->bits.C3X2=(tmp>>0) & 0x01;
		tmp=~tmp;
		p_st->bits.C1X2_b=(tmp>>2) & 0x01;
		p_st->bits.C2X2_b=(tmp>>1) & 0x01;
		p_st->bits.C3X2_b=(tmp>>0) & 0x01;		
	}
	else if(index==3)
	{
		tmp=access[3];
		p_st->bits.C1X3=(tmp>>2) & 0x01;
		p_st->bits.C2X3=(tmp>>1) & 0x01;
		p_st->bits.C3X3=(tmp>>0) & 0x01;
		tmp=~tmp;
		p_st->bits.C1X3_b=(tmp>>2) & 0x01;
		p_st->bits.C2X3_b=(tmp>>1) & 0x01;
		p_st->bits.C3X3_b=(tmp>>0) & 0x01;	
	}
	else
	{
		return false;
	}
	return true;
}
*/


