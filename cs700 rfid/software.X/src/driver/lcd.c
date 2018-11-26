#include "../soc/soc.h"
#include "../depend/depend.h"
#include "driver.h"
#define LCD_USED_INTERNAL_REF 1
		
 #define LCD_PIXEL_REGISTER_SIZE 24
#define LCD_STRING_LENGTH 8
 uint8_t* pLcdReg=(uint8_t*)(&LCDDATA0); 
 uint8_t pLCD[LCD_PIXEL_REGISTER_SIZE];
const uint8_t  LCDDigitalIndexTable[]="0123456789abcdefghijklmnopqrstuvwxyz.-: ";
const uint8_t LCDDigitalTable[]=
{
    LCD_CODE_0,LCD_CODE_1,LCD_CODE_2,LCD_CODE_3,
    LCD_CODE_4,LCD_CODE_5,LCD_CODE_6,LCD_CODE_7,
    LCD_CODE_8,LCD_CODE_9,LCD_CODE_A,LCD_CODE_B,
    LCD_CODE_C,LCD_CODE_D,LCD_CODE_E,LCD_CODE_F,
    LCD_CODE_G,LCD_CODE_H,LCD_CODE_I,LCD_CODE_J,
    LCD_CODE_K,LCD_CODE_L,LCD_CODE_M,LCD_CODE_N,
    LCD_CODE_O,LCD_CODE_P,LCD_CODE_Q,LCD_CODE_R,
    LCD_CODE_S,LCD_CODE_T,LCD_CODE_U,LCD_CODE_V,
    LCD_CODE_W,LCD_CODE_X,LCD_CODE_Y,LCD_CODE_Z,
    LCD_CODE_DOT,LCD_CODE__,LCD_CODE_DDOT,0x00,
};

lcd_set_com_seg(uint8_t com,uint8_t seg,bool show)
{
    uint8_t *p;
    uint8_t addr,bloc;
    if(seg>45 || com>3)return;
    addr=(seg/24)*12;
    addr+=(com*3);
    seg%=24;
    addr+=(seg/8);
    bloc=seg%8;
    p=pLCD+addr;
    if(show)
        *p |= (1<<bloc);    
    else
        *p &= ~(1<<bloc);   
}
void lcd_disp_dp(uint8_t dec,bool show)
{
    switch(dec){
        case 1:lcd_set_com_seg(3,24,show);break;
        case 2:lcd_set_com_seg(3,26,show);break;
        case 3:lcd_set_com_seg(3,44,show);break;
        case 4:lcd_set_com_seg(3,21,show);break;
        case 5:lcd_set_com_seg(3, 4,show);break;
        case 6:lcd_set_com_seg(3, 2,show);break;
        case 7:lcd_set_com_seg(3, 0,show);break;
        case 0:
        default:break;
    }
}
void lcd_disp_cln(bool show)
{
    //show colon
    lcd_set_com_seg(0, 42,show);
    lcd_set_com_seg(1, 42,show);
}
void lcd_disp_code_0(uint8_t code)
{
	lcd_disp_com_seg(0,31,code&LCD_DSEG_F);
	lcd_disp_com_seg(1,31,code&LCD_DSEG_G);
    lcd_disp_com_seg(2,31,code&LCD_DSEG_E);
    lcd_disp_com_seg(3,31,code&LCD_DSEG_D);
    
    lcd_disp_com_seg(0,00,code&LCD_DSEG_A);
    lcd_disp_com_seg(1,00,code&LCD_DSEG_B);
    lcd_disp_com_seg(2,00,code&LCD_DSEG_C);
    //lcd_disp_com_seg(3,00,code&LCD_DSEG_DP);
}
void lcd_disp_code_1(uint8_t code)
{
	lcd_disp_com_seg(0,01,code&LCD_DSEG_F);
	lcd_disp_com_seg(1,01,code&LCD_DSEG_G);
    lcd_disp_com_seg(2,01,code&LCD_DSEG_E);
    lcd_disp_com_seg(3,01,code&LCD_DSEG_D);
    
    lcd_disp_com_seg(0,02,code&LCD_DSEG_A);
    lcd_disp_com_seg(1,02,code&LCD_DSEG_B);
    lcd_disp_com_seg(2,02,code&LCD_DSEG_C);
    //lcd_disp_com_seg(3,02,code&LCD_DSEG_DP);
}
void lcd_disp_code_2(uint8_t code)
{
	lcd_disp_com_seg(0,03,code&LCD_DSEG_F);
	lcd_disp_com_seg(1,03,code&LCD_DSEG_G);
    lcd_disp_com_seg(2,03,code&LCD_DSEG_E);
    lcd_disp_com_seg(3,03,code&LCD_DSEG_D);
    
    lcd_disp_com_seg(0,04,code&LCD_DSEG_A);
    lcd_disp_com_seg(1,04,code&LCD_DSEG_B);
    lcd_disp_com_seg(2,04,code&LCD_DSEG_C);
    //lcd_disp_com_seg(3,04,code&LCD_DSEG_DP);
}
void lcd_disp_code_3(uint8_t code)
{
	lcd_disp_com_seg(0,20,code&LCD_DSEG_F);
	lcd_disp_com_seg(1,20,code&LCD_DSEG_G);
    lcd_disp_com_seg(2,20,code&LCD_DSEG_E);
    lcd_disp_com_seg(3,20,code&LCD_DSEG_D);
    
    lcd_disp_com_seg(0,21,code&LCD_DSEG_A);
    lcd_disp_com_seg(1,21,code&LCD_DSEG_B);
    lcd_disp_com_seg(2,21,code&LCD_DSEG_C);
    //lcd_disp_com_seg(3,21,code&LCD_DSEG_DP);
}
void lcd_disp_code_4(uint8_t code)
{
	lcd_disp_com_seg(0,43,code&LCD_DSEG_F);
	lcd_disp_com_seg(1,43,code&LCD_DSEG_G);
    lcd_disp_com_seg(2,43,code&LCD_DSEG_E);
    lcd_disp_com_seg(3,43,code&LCD_DSEG_D);
    
    lcd_disp_com_seg(0,44,code&LCD_DSEG_A);
    lcd_disp_com_seg(1,44,code&LCD_DSEG_B);
    lcd_disp_com_seg(2,44,code&LCD_DSEG_C);
    //lcd_disp_com_seg(3,44,code&LCD_DSEG_DP);
}
void lcd_disp_code_5(uint8_t code)
{
	lcd_disp_com_seg(0,45,code&LCD_DSEG_F);
	lcd_disp_com_seg(1,45,code&LCD_DSEG_G);
    lcd_disp_com_seg(2,45,code&LCD_DSEG_E);
    lcd_disp_com_seg(3,45,code&LCD_DSEG_D);
    
    lcd_disp_com_seg(0,26,code&LCD_DSEG_A);
    lcd_disp_com_seg(1,26,code&LCD_DSEG_B);
    lcd_disp_com_seg(2,26,code&LCD_DSEG_C);
    //lcd_disp_com_seg(3,26,code&LCD_DSEG_DP);
}
void lcd_disp_code_6(uint8_t code)
{
	lcd_disp_com_seg(0,25,code&LCD_DSEG_F);
	lcd_disp_com_seg(1,25,code&LCD_DSEG_G);
    lcd_disp_com_seg(2,25,code&LCD_DSEG_E);
    lcd_disp_com_seg(3,25,code&LCD_DSEG_D);
    
    lcd_disp_com_seg(0,24,code&LCD_DSEG_A);
    lcd_disp_com_seg(1,24,code&LCD_DSEG_B);
    lcd_disp_com_seg(2,24,code&LCD_DSEG_C);
    //lcd_disp_com_seg(3,24,code&LCD_DSEG_DP);
}
void lcd_disp_code_7(uint8_t code)
{
	lcd_disp_com_seg(0,23,code&LCD_DSEG_F);
	lcd_disp_com_seg(1,23,code&LCD_DSEG_G);
    lcd_disp_com_seg(2,23,code&LCD_DSEG_E);
    lcd_disp_com_seg(3,23,code&LCD_DSEG_D);
    
    lcd_disp_com_seg(0,22,code&LCD_DSEG_A);
    lcd_disp_com_seg(1,22,code&LCD_DSEG_B);
    lcd_disp_com_seg(2,22,code&LCD_DSEG_C);
    //lcd_disp_com_seg(3,22,code&LCD_DSEG_DP);
}
void lcd_disp_code(uint8_t code,uint8_t loc)
{
    switch(loc){
        case 0:lcd_disp_code_0(code);break;
        case 1:lcd_disp_code_1(code);break;
        case 2:lcd_disp_code_2(code);break;
        case 3:lcd_disp_code_3(code);break;
        case 4:lcd_disp_code_4(code);break;
        case 5:lcd_disp_code_5(code);break;
        case 6:lcd_disp_code_6(code);break;
        case 7:lcd_disp_code_7(code);break;     
        default:break;
    }	
}
/*
void lcd_disp_code_ex(uint8_t code,uint8_t loc,uint8_t dp)
{
    if(dp){
        code |= LCD_DSEG_DP;
    }
    lcd_disp_code(code,loc);
}
*/
void lcd_disp_str(uint8_t* str)
{
    uint8_t loc=0;
    uint8_t index=0;
    while(*str!='\0'){    
        for(index=0;index<LCD_TABLE_LEN;index++){        
            if(*str==LCDDigitalIndexTable[index])break;            
        }
        if(index<LCD_TABLE_LEN)lcd_disp_code(LCDDigitalTable[index],loc);
        str++;
        loc++;
    }
	lcd_disp_refresh();
}
void lcd_disp_all(uint8_t x)
{
    uint8_t i;
    for(i=0;i<LCD_PIXEL_REGISTER_SIZE;i++)
    {
        pLcdReg[i]=x;
    }
}
void lcd_disp_refresh(void)
{
	m_mem_cpy_len(pLcdReg,pLCD,LCD_PIXEL_REGISTER_SIZE);
}

void lcd_clear_buffer(void)
{
    m_mem_set(pLCD,0,LCD_PIXEL_REGISTER_SIZE);
}
void lcd_config(void)
{
	//参考数据手册，17.13，p367
	//1.使用 LCDPS 寄存器的 LP<3:0> 位可以选择帧时钟预分频比。
	LCDPSbits.WFT     = 0;   	//0:typeA,1:typeB
	LCDPSbits.BIASMD  = 0;		//
	LCDPSbits.LP=0x07;
	//Configure all LCD pins as inputs.
	//On Power on reset all pins are configured as inputs.
	//to do add Configure code
	//Configure the appropriate pins to function as segment drivers
	//using the LCDSEx registers

	// TRISDbits.TRISD0=0;//debug
	LCDSE0 = 0x1f;
	LCDSE1 = 0x00;
	LCDSE2 = 0xf0;
	LCDSE3 = 0x87; // Disable Seg30, Seg28 and Seg27
	LCDSE4 = 0x00; // Disable unused segments
	LCDSE5 = 0b00111100; // Disable unused segments
	//config LCDCON
	//LCDCON=0x8F;
	//clear all pixel data
	lcd_clear_all();
	//config reference voltage
	//LCDRL=0x8f;
	//LCDREF=0x07;
    //LCDCST=0x07;
    LCDCST=0x00;//
	#if LCD_USED_INTERNAL_REF==1
	LCDRL=0xff;
	LCDREF= 0x80  ;      //internal
	#else

	LCDREF= 0x0f  ; 
	#endif
	//LCDCON=0x8F;
    lcd_clear_all();
	lcd_on();
}
void lcd_init(void)
{
    //lcd_disp_all(0x00);
	lcd_config();
    lcd_blck_on();
    //lcd_on();
}