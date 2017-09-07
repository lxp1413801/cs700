#ifndef __LCD_H__
#define __LCD_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <xc.h>
//#include "lcd-com-seg.h"
//#include
#include "lcd-com-seg-set.h"
#include "lcd-com-seg-clr.h"
#ifdef __cplusplus
    extern "C" {
#endif 
       
        
	#define LCD_DSEG_A 	0x01
	#define LCD_DSEG_B 	0x02
	#define LCD_DSEG_C 	0x04
	#define LCD_DSEG_D 	0x08	
	//
	#define LCD_DSEG_E 	0x10
	#define LCD_DSEG_F 	0x20
	#define LCD_DSEG_G 	0x40
	#define LCD_DSEG_DP 0x80
	//

	#define LCD_CODE_0 (LCD_DSEG_F + LCD_DSEG_E + LCD_DSEG_D + LCD_DSEG_C + LCD_DSEG_B + LCD_DSEG_A)
	#define LCD_CODE_1 (LCD_DSEG_F + LCD_DSEG_E)
	#define LCD_CODE_2 (LCD_DSEG_A + LCD_DSEG_B + LCD_DSEG_G + LCD_DSEG_E + LCD_DSEG_D)      
	#define LCD_CODE_3 (LCD_DSEG_A + LCD_DSEG_B + LCD_DSEG_G + LCD_DSEG_C + LCD_DSEG_D)  
	#define LCD_CODE_4 (LCD_DSEG_F + LCD_DSEG_G + LCD_DSEG_B + LCD_DSEG_C) 
	#define LCD_CODE_5 (LCD_DSEG_F + LCD_DSEG_G + LCD_DSEG_C + LCD_DSEG_D + LCD_DSEG_A)
	#define LCD_CODE_6 (LCD_DSEG_A + LCD_DSEG_F + LCD_DSEG_E + LCD_DSEG_D + LCD_DSEG_C + LCD_DSEG_G)
	#define LCD_CODE_7 (LCD_DSEG_A + LCD_DSEG_B + LCD_DSEG_C)  
	#define LCD_CODE_8 (LCD_DSEG_A + LCD_DSEG_B + LCD_DSEG_C + LCD_DSEG_D + LCD_DSEG_E + LCD_DSEG_F + LCD_DSEG_G)
	#define LCD_CODE_9 (LCD_DSEG_A + LCD_DSEG_F + LCD_DSEG_G + LCD_DSEG_B + LCD_DSEG_C+ LCD_DSEG_D)

	#define LCD_CODE_A (LCD_DSEG_A + LCD_DSEG_F + LCD_DSEG_E + LCD_DSEG_B + LCD_DSEG_C + LCD_DSEG_G)
	#define LCD_CODE_B (LCD_DSEG_F + LCD_DSEG_E + LCD_DSEG_G + LCD_DSEG_C + LCD_DSEG_D)
	#define LCD_CODE_C (LCD_DSEG_A + LCD_DSEG_F + LCD_DSEG_E + LCD_DSEG_D)
	#define LCD_CODE_D (LCD_DSEG_B + LCD_DSEG_E + LCD_DSEG_G + LCD_DSEG_C + LCD_DSEG_D)
	#define LCD_CODE_E (LCD_DSEG_A + LCD_DSEG_F + LCD_DSEG_E + LCD_DSEG_D + LCD_DSEG_G)
	#define LCD_CODE_F (LCD_DSEG_A + LCD_DSEG_F + LCD_DSEG_E + LCD_DSEG_G)
	#define LCD_CODE_G (LCD_DSEG_A + LCD_DSEG_F + LCD_DSEG_G + LCD_DSEG_B + LCD_DSEG_C+ LCD_DSEG_D)

	#define LCD_CODE_H (LCD_DSEG_B + LCD_DSEG_C + LCD_DSEG_E + LCD_DSEG_F + LCD_DSEG_G)
	#define LCD_CODE_I LCD_CODE_1
	#define LCD_CODE_J (LCD_DSEG_B + LCD_DSEG_C + LCD_DSEG_D)
	#define LCD_CODE_K 0
	#define LCD_CODE_L (LCD_DSEG_F +LCD_DSEG_E +LCD_DSEG_D)
	#define LCD_CODE_M 0
	#define LCD_CODE_N (LCD_DSEG_F +LCD_DSEG_E +LCD_DSEG_A+LCD_DSEG_B+LCD_DSEG_C)
	
	#define LCD_CODE_O LCD_CODE_0
	#define LCD_CODE_P (LCD_DSEG_A +LCD_DSEG_B +LCD_DSEG_E+LCD_DSEG_F+LCD_DSEG_G)
	#define LCD_CODE_Q (LCD_DSEG_A +LCD_DSEG_B +LCD_DSEG_C+LCD_DSEG_F+LCD_DSEG_G)

	#define LCD_CODE_R (LCD_DSEG_A +LCD_DSEG_F +LCD_DSEG_E)
	#define LCD_CODE_S LCD_CODE_5
	#define LCD_CODE_T (LCD_DSEG_F +LCD_DSEG_E +LCD_DSEG_D+LCD_DSEG_G)
	
	#define LCD_CODE_U (LCD_DSEG_B +LCD_DSEG_C +LCD_DSEG_D+LCD_DSEG_E+LCD_DSEG_F)
	#define LCD_CODE_V 0
	#define LCD_CODE_W 0

	#define LCD_CODE_X 0
	#define LCD_CODE_Y (LCD_DSEG_B +LCD_DSEG_C +LCD_DSEG_D+LCD_DSEG_G+LCD_DSEG_F)
	#define LCD_CODE_Z LCD_CODE_2
	
	#define LCD_CODE_DP LCD_DSEG_DP
	#define LCD_CODE__ (LCD_DSEG_G)
	#define LCD_CODE_DOT	(LCD_DSEG_G)
	#define LCD_CODE_DDOT	(LCD_DSEG_G+LCD_DSEG_D)

	//extern uint8_t* pLCD;
	#define LCD_TABLE_LEN 40
	//extern const uint8_t LCDDigitalTable[LCD_TABLE_LEN];
	extern const uint8_t LCDDigitalTable[];
    	#define lcd_disp_com_seg(c,s,d) do{  \
		lcd_data_clr_c##c##_s##s(); \
		if(d){ \
		lcd_data_set_c##c##_s##s(); \
		}}while(0); \
	extern void lcd_disp_m(uint8_t loc,uint8_t m);
	extern void lcd_disp_segment(uint8_t seg,uint8_t com);
	extern void lcd_clear_segment(uint8_t seg,uint8_t com);
	void lcd_disp_refresh(void);
	extern void lcd_disp_all(uint8_t x);
	extern void lcd_init(void);
	extern void lcd_disp_str(uint8_t* str);
    extern void lcd_disp_dp(uint8_t dec,bool show);
    extern void lcd_disp_cln(bool show);
    extern void lcd_clear_buffer(void);
	//#	
	#define lcd_on() do{LCDCON=0x8F;}while(0);
	#define lcd_off() do{LCDCON=0x00;}while(0);
	#define lcd_show_all()   lcd_disp_all(0xff);  
	#define lcd_clear_all()   lcd_disp_all(0x00);	
	#define lcd_enable() do{LCDCON=0x8F;}while(0);
	#define lcd_disable() do{LCDCON=0x00;}while(0);
    
#ifdef __cplusplus
    }
#endif
#endif