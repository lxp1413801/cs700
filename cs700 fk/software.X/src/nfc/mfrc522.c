
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


char mfrc522_pcd_comm_witch_picc(
uint8_t Command  ,
uint8_t *pInData , 
uint8_t InLenByte,
uint8_t *pOutData, 
uint16_t  *pOutLenBit                       )
{
  char status = MI_ERR                          ;
  uint8_t irqEn   = 0x00                  ;
  uint8_t waitFor = 0x00                  ;
  uint8_t lastBits                        ;
  uint8_t n                               ;
  uint16_t  i                               ;
  switch (Command)
  {
    case PCD_AUTHENT:
      irqEn   = 0x12                            ;
      waitFor = 0x10                            ;
      break                                     ;
    case PCD_TRANSCEIVE:
      irqEn   = 0x77                            ;
      waitFor = 0x30                            ;
      break                                     ;
    default:
      break                                     ;
  }
  mfrc522_bal_write_reg(ComIEnReg,irqEn|0x80)              ; //
  mfrc522_bal_clr_bit_reg(ComIrqReg,0x80)                  ;
  mfrc522_bal_write_reg(CommandReg,PCD_IDLE)               ;
  mfrc522_bal_set_bit_reg(FIFOLevelReg,0x80)                 ; // ���FIFO 
  for(i=0; i<InLenByte; i++)
    mfrc522_bal_write_reg(FIFODataReg,pInData[i])          ; // ����д��FIFO 
  mfrc522_bal_write_reg(CommandReg, Command)               ; // ����д������Ĵ���
  if(Command == PCD_TRANSCEIVE)
    mfrc522_bal_set_bit_reg(BitFramingReg,0x80)              ; // ��ʼ����     
  i = 6000                                      ; //����ʱ��Ƶ�ʵ���������M1�����ȴ�ʱ��25ms
  do 
  {
    n = mfrc522_bal_read_reg(ComIrqReg)                    ;
    i--                                         ;
  }
  while((i!=0)&&!(n&0x01)&&!(n&waitFor))        ;
  mfrc522_bal_clr_bit_reg(BitFramingReg,0x80)              ;
  if(i!=0)
  {
    if(!(mfrc522_bal_read_reg(ErrorReg)&0x1B))
    {
      status = MI_OK                            ;
      if (n&irqEn&0x01)
        status = MI_NOTAGERR                    ;
      if(Command==PCD_TRANSCEIVE)
      {
        n = mfrc522_bal_read_reg(FIFOLevelReg)             ;
        lastBits = mfrc522_bal_read_reg(ControlReg)&0x07   ;
        if(lastBits)
          *pOutLenBit = (n-1)*8 + lastBits      ;
        else
          *pOutLenBit = n*8                     ;
        if(n==0)
          n = 1                                 ;
        if(n>MAXRLEN)
          n = MAXRLEN                           ;
        for (i=0; i<n; i++)
          pOutData[i] = mfrc522_bal_read_reg(FIFODataReg)  ; 
      }
    }
    else
      status = MI_ERR                           ;        
  }
  mfrc522_bal_set_bit_reg(ControlReg,0x80)                   ;// stop timer now
  mfrc522_bal_write_reg(CommandReg,PCD_IDLE)               ; 
  return status;
}

//******************************************************************/
//��    �ܣ�Ѱ��                                                    /
//����˵��: req_code[IN]:Ѱ����ʽ                                   /
//                0x52 = Ѱ��Ӧ�������з���14443A��׼�Ŀ�           /
//                0x26 = Ѱδ��������״̬�Ŀ�                       /
//          pTagType[OUT]����Ƭ���ʹ���                             /
//                0x4400 = Mifare_UltraLight                        /
//                0x0400 = Mifare_One(S50)                          /
//                0x0200 = Mifare_One(S70)                          /
//                0x0800 = Mifare_Pro(X)                            /
//                0x4403 = Mifare_DESFire                           /
//��    ��: �ɹ�����MI_OK                                           /
//******************************************************************/
char mfrc522_pcd_request(uint8_t req_code,uint8_t *pTagType)
{
  char status                                        ;  
  uint16_t  unLen                                ;
  uint8_t ucComMF522Buf[MAXRLEN]               ; 

  mfrc522_bal_clr_bit_reg(Status2Reg,0x08)                      ;
  mfrc522_bal_write_reg(BitFramingReg,0x07)                     ;
  mfrc522_bal_set_bit_reg(TxControlReg,0x03)                      ;
 
  ucComMF522Buf[0] = req_code                        ;

  status = mfrc522_pcd_comm_witch_picc(PCD_TRANSCEIVE,ucComMF522Buf,
                       1,ucComMF522Buf,&unLen       );
  if ((status == MI_OK) && (unLen == 0x10))
  {    
    *pTagType     = ucComMF522Buf[0]                 ;
    *(pTagType+1) = ucComMF522Buf[1]                 ;
  }
  else
    status = MI_ERR                                  ;
  return status                                      ;
}

//******************************************************************/
//��    �ܣ�����ײ                                                  /
//����˵��: pSnr[OUT]:��Ƭ���кţ�4�ֽ�                             /
//��    ��: �ɹ�����MI_OK                                           /
//******************************************************************/
char mfrc522_pcd_anticoll(uint8_t *pSnr)
{
    char status;
    uint8_t i,snr_check=0;
    uint16_t  unLen;
    uint8_t ucComMF522Buf[MAXRLEN]; 
    
    mfrc522_bal_clr_bit_reg(Status2Reg,0x08);
    mfrc522_bal_write_reg(BitFramingReg,0x00);
    mfrc522_bal_clr_bit_reg(CollReg,0x80);
 
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x20;

    status = mfrc522_pcd_comm_witch_picc(PCD_TRANSCEIVE,ucComMF522Buf,2,ucComMF522Buf,&unLen);

    if (status == MI_OK)
    {
    	 for (i=0; i<4; i++)
         {   
             *(pSnr+i)  = ucComMF522Buf[i];
             snr_check ^= ucComMF522Buf[i];
         }
         if (snr_check != ucComMF522Buf[i])
         {   status = MI_ERR;    }
    }
    
    mfrc522_bal_set_bit_reg(CollReg,0x80);
    return status;
}

/////////////////////////////////////////////////////////////////////
//��    �ܣ�ѡ����Ƭ
//����˵��: pSnr[IN]:��Ƭ���кţ�4�ֽ�
//��    ��: �ɹ�����MI_OK
/////////////////////////////////////////////////////////////////////
char mfrc522_pcd_card_select(uint8_t *pSnr)
{
    char status;
    uint8_t i;
    uint16_t  unLen;
    uint8_t ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x70;
    ucComMF522Buf[6] = 0;
    for (i=0; i<4; i++)
    {
    	ucComMF522Buf[i+2] = *(pSnr+i);
    	ucComMF522Buf[6]  ^= *(pSnr+i);
    }
    CalulateCRC(ucComMF522Buf,7,&ucComMF522Buf[7]);
  
    mfrc522_bal_clr_bit_reg(Status2Reg,0x08);

    status = mfrc522_pcd_comm_witch_picc(PCD_TRANSCEIVE,ucComMF522Buf,9,ucComMF522Buf,&unLen);
    
    if ((status == MI_OK) && (unLen == 0x18))
    {   status = MI_OK;  }
    else
    {   status = MI_ERR;    }

    return status;
}

//******************************************************************/
//��    �ܣ���֤��Ƭ����
//����˵��: auth_mode[IN]: ������֤ģʽ
//                 0x60 = ��֤A��Կ
//                 0x61 = ��֤B��Կ 
//          addr[IN]�����ַ
//          pKey[IN]������
//          pSnr[IN]����Ƭ���кţ�4�ֽ�
//��    ��: �ɹ�����MI_OK
//******************************************************************/
char mfrc522_pcd_auth(uint8_t auth_mode,uint8_t addr,
                  uint8_t *pKey,uint8_t *pSnr    )
{
    char status;
    uint16_t  unLen;
    uint8_t i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = auth_mode;
    ucComMF522Buf[1] = addr;
    for (i=0; i<6; i++)
    {    ucComMF522Buf[i+2] = *(pKey+i);   }
    for (i=0; i<6; i++)
    {    ucComMF522Buf[i+8] = *(pSnr+i);   }
    
    status = mfrc522_pcd_comm_witch_picc(PCD_AUTHENT,ucComMF522Buf,12,ucComMF522Buf,&unLen);
    if ((status != MI_OK) || (!(mfrc522_bal_read_reg(Status2Reg) & 0x08)))
    {   status = MI_ERR;   }
    
    return status;
}

//******************************************************************/
//��    �ܣ���ȡM1��һ������
//����˵��: addr[IN]�����ַ
//          pData[OUT]�����������ݣ�16�ֽ�
//��    ��: �ɹ�����MI_OK
//******************************************************************/
char mfrc522_pcd_read(uint8_t addr,uint8_t *pData)
{
    char status                                          ;
    uint16_t  unLen                                  ;
    uint8_t i,ucComMF522Buf[MAXRLEN]               ; 

    ucComMF522Buf[0] = PICC_READ                         ;
    ucComMF522Buf[1] = addr                              ;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2])       ;   
    status = mfrc522_pcd_comm_witch_picc(PCD_TRANSCEIVE,ucComMF522Buf,4,
                         ucComMF522Buf,&unLen           );
    if ((status == MI_OK) && (unLen == 0x90))
    {
        for (i=0; i<16; i++)
            *(pData+i) = ucComMF522Buf[i];   
    }
    else
      status = MI_ERR;       
    return status;
}

//******************************************************************/
//��    �ܣ���ȡM1��һ������
//����˵��: addr[IN]�����ַ
//          pData[OUT]�����������ݣ�16�ֽ�
//��    ��: �ɹ�����MI_OK
//******************************************************************/
char Read_Block(uint8_t Block,uint8_t *Buf)
{
  char result                                             ;
  result = mfrc522_pcd_auth(0x60,Block,Password_Buffer,UID)   ;
  if(result!=MI_OK)
    return result                                         ;
  result = mfrc522_pcd_read(Block,Buf)                             ;
//  return result; // 2011.01.03
  
  if(result!=MI_OK)     return   result                   ;
  if(Block!=0x00&&des_on)
  {
    Des_Decrypt((char *)Buf    ,KK,(char *)Buf    )       ;
    Des_Decrypt((char *)&Buf[8],KK,(char *)&Buf[8])       ;  
  }
  return SUCCESS                                          ; 
}

//******************************************************************/
//��    �ܣ�д���ݵ�M1��һ��
//����˵��: addr[IN]�����ַ
//          pData[IN]��д������ݣ�16�ֽ�
//��    ��: �ɹ�����MI_OK
//******************************************************************/
char mfrc522_pcd_write(uint8_t addr,uint8_t *pData)
{
  char status                                             ;
  uint16_t  unLen                                     ;
  uint8_t i,ucComMF522Buf[MAXRLEN]                  ; 
    
  ucComMF522Buf[0] = PICC_WRITE                           ;
  ucComMF522Buf[1] = addr                                 ;
  CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2])          ;
  status = mfrc522_pcd_comm_witch_picc(PCD_TRANSCEIVE,ucComMF522Buf,4,
                       ucComMF522Buf,&unLen          )    ;
  if(  ( status != MI_OK)||(unLen != 4)
     ||((ucComMF522Buf[0]&0x0F)!= 0x0A))
    status = MI_ERR                                       ;           
  if (status == MI_OK)
  {
    for (i=0; i<16; i++)
      ucComMF522Buf[i] = *(pData+i)                       ;  
    CalulateCRC(ucComMF522Buf,16,&ucComMF522Buf[16])      ;
    status = mfrc522_pcd_comm_witch_picc(PCD_TRANSCEIVE,ucComMF522Buf,
                         18,ucComMF522Buf,&unLen     )    ;
    if(  (status != MI_OK)||(unLen != 4 )
       ||((ucComMF522Buf[0]&0x0F)!= 0x0A))
      status = MI_ERR                                     ;   
  }    
  return status                                           ;
}
//******************************************************************/
//��    �ܣ�д���ݵ�M1��һ��
//����˵��: addr[IN]�����ַ
//          pData[IN]��д������ݣ�16�ֽ�
//��    ��: �ɹ�����MI_OK
//******************************************************************/

char Write_Block(uint8_t Block)
{
  char result                                             ;
  if(des_on)
  {
    Des_Encrypt((char *)RF_Buffer    ,KK,
                (char *)RF_Buffer        )                ;// for debug
    Des_Encrypt((char *)&RF_Buffer[8],KK,
                (char *)&RF_Buffer[8]    )                ;// for debug  
  }
  result = mfrc522_pcd_auth(0x60,Block,Password_Buffer,UID)   ;
  if(result!=MI_OK)
    return result                                         ;  
  result = mfrc522_pcd_write(Block,RF_Buffer)                      ;
  return result                                           ;  
}

//******************************************************************/
//��    �ܣ��ۿ�ͳ�ֵ
//����˵��: dd_mode[IN]��������
//               0xC0 = �ۿ�
//               0xC1 = ��ֵ
//          addr[IN]��Ǯ����ַ
//          pValue[IN]��4�ֽ���(��)ֵ����λ��ǰ
//��    ��: �ɹ�����MI_OK
//******************************************************************/
char PcdValue(uint8_t dd_mode,uint8_t addr,uint8_t *pValue)
{
    char status;
    uint16_t  unLen;
    uint8_t i,ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = dd_mode;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = mfrc522_pcd_comm_witch_picc(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = MI_ERR;   }
        
    if (status == MI_OK)
    {
        for (i=0; i<16; i++)
        {    ucComMF522Buf[i] = *(pValue+i);   }
        CalulateCRC(ucComMF522Buf,4,&ucComMF522Buf[4]);
        unLen = 0;
        status = mfrc522_pcd_comm_witch_picc(PCD_TRANSCEIVE,ucComMF522Buf,6,ucComMF522Buf,&unLen);
        if (status != MI_ERR)
        {    status = MI_OK;    }
    }
    
    if (status == MI_OK)
    {
        ucComMF522Buf[0] = PICC_TRANSFER;
        ucComMF522Buf[1] = addr;
        CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]); 
   
        status = mfrc522_pcd_comm_witch_picc(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {   status = MI_ERR;   }
    }
    return status;
}

//******************************************************************/
//��    �ܣ�����Ǯ��
//����˵��: sourceaddr[IN]��Դ��ַ
//          goaladdr[IN]��Ŀ���ַ
//��    ��: �ɹ�����MI_OK
//******************************************************************/
char PcdBakValue(uint8_t sourceaddr, uint8_t goaladdr)
{
    char status;
    uint16_t  unLen;
    uint8_t ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_RESTORE;
    ucComMF522Buf[1] = sourceaddr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = mfrc522_pcd_comm_witch_picc(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = MI_ERR;   }
    
    if (status == MI_OK)
    {
        ucComMF522Buf[0] = 0;
        ucComMF522Buf[1] = 0;
        ucComMF522Buf[2] = 0;
        ucComMF522Buf[3] = 0;
        CalulateCRC(ucComMF522Buf,4,&ucComMF522Buf[4]);
 
        status = mfrc522_pcd_comm_witch_picc(PCD_TRANSCEIVE,ucComMF522Buf,6,ucComMF522Buf,&unLen);
        if (status != MI_ERR)
        {    status = MI_OK;    }
    }
    
    if (status != MI_OK)
    {    return MI_ERR;   }
    
    ucComMF522Buf[0] = PICC_TRANSFER;
    ucComMF522Buf[1] = goaladdr;

    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = mfrc522_pcd_comm_witch_picc(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = MI_ERR;   }

    return status;
}


//******************************************************************/
//��    �ܣ����Ƭ��������״̬
//��    ��: �ɹ�����MI_OK
//******************************************************************/
char mfrc522_pcd_halt(void)
{
    char status;
    uint16_t  unLen;
    uint8_t ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_HALT;
    ucComMF522Buf[1] = 0;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = mfrc522_pcd_comm_witch_picc(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
    return status;
//    return MI_OK;
}

//******************************************************************/
//��    �ܣ����Ƭ��������״̬
//��    ��: �ɹ�����MI_OK
//******************************************************************/
char MIF_Halt(void)
{
    char status;
    uint16_t  unLen;
    uint8_t ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_HALT;
    ucComMF522Buf[1] = 0;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = mfrc522_pcd_comm_witch_picc(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
    return status ;  
//    return MI_OK;
}



//******************************************************************/
//��MF522����CRC16����
//******************************************************************/
void CalulateCRC(uint8_t *pIndata,uint8_t len,uint8_t *pOutData)
{
    uint8_t i,n;
    mfrc522_bal_clr_bit_reg(DivIrqReg,0x04);
    mfrc522_bal_write_reg(CommandReg,PCD_IDLE);
    mfrc522_bal_set_bit_reg(FIFOLevelReg,0x80);
    for (i=0; i<len; i++)
    {   mfrc522_bal_write_reg(FIFODataReg, *(pIndata+i));   }
    mfrc522_bal_write_reg(CommandReg, PCD_CALCCRC);
    i = 0xFF;
    do 
    {
        n = mfrc522_bal_read_reg(DivIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x04));
    pOutData[0] = mfrc522_bal_read_reg(CRCResultRegL);
    pOutData[1] = mfrc522_bal_read_reg(CRCResultRegM);
}