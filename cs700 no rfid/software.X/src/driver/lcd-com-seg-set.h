#ifndef __LCD_SEG_COM_SET_H__
#define __LCD_SEG_COM_SET_H__
#ifdef __cplusplus
    extern "C" {
#endif 
	extern uint8_t pLCD[];
	//---------------------------------------------
	//com0 s0-s23
	#if 1
	//LCDDATA0 SEG<7:0>COM0
	#define lcd_data_set_c0_s00() pLCD[0] |= (1<<0)
	#define lcd_data_set_c0_s01() pLCD[0] |= (1<<1)   
	#define lcd_data_set_c0_s02() pLCD[0] |= (1<<2)
	#define lcd_data_set_c0_s03() pLCD[0] |= (1<<3)   
	#define lcd_data_set_c0_s04() pLCD[0] |= (1<<4)
	#define lcd_data_set_c0_s05() pLCD[0] |= (1<<5)   
	#define lcd_data_set_c0_s06() pLCD[0] |= (1<<6)
	#define lcd_data_set_c0_s07() pLCD[0] |= (1<<7)   
	//LCDDATA1 SEG<15:8>COM0
	#define lcd_data_set_c0_s08() pLCD[1] |= (1<<0)
	#define lcd_data_set_c0_s09() pLCD[1] |= (1<<1)   
	#define lcd_data_set_c0_s10() pLCD[1] |= (1<<2)
	#define lcd_data_set_c0_s11() pLCD[1] |= (1<<3)   
	#define lcd_data_set_c0_s12() pLCD[1] |= (1<<4)
	#define lcd_data_set_c0_s13() pLCD[1] |= (1<<5)   
	#define lcd_data_set_c0_s14() pLCD[1] |= (1<<6)
	#define lcd_data_set_c0_s15() pLCD[1] |= (1<<7)  	
	//LCDDATA2 SEG<23:16>COM0
	#define lcd_data_set_c0_s16() pLCD[2] |= (1<<0)
	#define lcd_data_set_c0_s17() pLCD[2] |= (1<<1)   
	#define lcd_data_set_c0_s18() pLCD[2] |= (1<<2)
	#define lcd_data_set_c0_s19() pLCD[2] |= (1<<3)   
	#define lcd_data_set_c0_s20() pLCD[2] |= (1<<4)
	#define lcd_data_set_c0_s21() pLCD[2] |= (1<<5)   
	#define lcd_data_set_c0_s22() pLCD[2] |= (1<<6)
	#define lcd_data_set_c0_s23() pLCD[2] |= (1<<7)   
	#endif
    //---------------------------------------------
	//com1 s0-s23
	#if 1
	// LCDDATA3 SEG<7:0>COM1
	#define lcd_data_set_c1_s00() pLCD[3] |= (1<<0)
	#define lcd_data_set_c1_s01() pLCD[3] |= (1<<1)   
	#define lcd_data_set_c1_s02() pLCD[3] |= (1<<2)
	#define lcd_data_set_c1_s03() pLCD[3] |= (1<<3)   
	#define lcd_data_set_c1_s04() pLCD[3] |= (1<<4)
	#define lcd_data_set_c1_s05() pLCD[3] |= (1<<5)   
	#define lcd_data_set_c1_s06() pLCD[3] |= (1<<6)
	#define lcd_data_set_c1_s07() pLCD[3] |= (1<<7)   
	//LCDDATA4 SEG<15:8>COM1
	#define lcd_data_set_c1_s08() pLCD[4] |= (1<<0)
	#define lcd_data_set_c1_s09() pLCD[4] |= (1<<1)   
	#define lcd_data_set_c1_s10() pLCD[4] |= (1<<2)
	#define lcd_data_set_c1_s11() pLCD[4] |= (1<<3)   
	#define lcd_data_set_c1_s12() pLCD[4] |= (1<<4)
	#define lcd_data_set_c1_s13() pLCD[4] |= (1<<5)   
	#define lcd_data_set_c1_s14() pLCD[4] |= (1<<6)
	#define lcd_data_set_c1_s15() pLCD[4] |= (1<<7)  	
	//LCDDATA5 SEG<23:16>COM1
	#define lcd_data_set_c1_s16() pLCD[5] |= (1<<0)
	#define lcd_data_set_c1_s17() pLCD[5] |= (1<<1)   
	#define lcd_data_set_c1_s18() pLCD[5] |= (1<<2)
	#define lcd_data_set_c1_s19() pLCD[5] |= (1<<3)   
	#define lcd_data_set_c1_s20() pLCD[5] |= (1<<4)
	#define lcd_data_set_c1_s21() pLCD[5] |= (1<<5)   
	#define lcd_data_set_c1_s22() pLCD[5] |= (1<<6)
	#define lcd_data_set_c1_s23() pLCD[5] |= (1<<7) 
	#endif
    //---------------------------------------------
	//com2 s0-s23
	#if 1
	// LCDDATA6 SEG<7:0>COM2
	#define lcd_data_set_c2_s00() pLCD[6] |= (1<<0)
	#define lcd_data_set_c2_s01() pLCD[6] |= (1<<1)   
	#define lcd_data_set_c2_s02() pLCD[6] |= (1<<2)
	#define lcd_data_set_c2_s03() pLCD[6] |= (1<<3)   
	#define lcd_data_set_c2_s04() pLCD[6] |= (1<<4)
	#define lcd_data_set_c2_s05() pLCD[6] |= (1<<5)   
	#define lcd_data_set_c2_s06() pLCD[6] |= (1<<6)
	#define lcd_data_set_c2_s07() pLCD[6] |= (1<<7)   
	//LCDDATA7 SEG<15:8>COM7
	#define lcd_data_set_c2_s08() pLCD[7] |= (1<<0)
	#define lcd_data_set_c2_s09() pLCD[7] |= (1<<1)   
	#define lcd_data_set_c2_s10() pLCD[7] |= (1<<2)
	#define lcd_data_set_c2_s11() pLCD[7] |= (1<<3)   
	#define lcd_data_set_c2_s12() pLCD[7] |= (1<<4)
	#define lcd_data_set_c2_s13() pLCD[7] |= (1<<5)   
	#define lcd_data_set_c2_s14() pLCD[7] |= (1<<6)
	#define lcd_data_set_c2_s15() pLCD[7] |= (1<<7)  	
	//LCDDATA8 SEG<23:16>COM2
	#define lcd_data_set_c2_s16() pLCD[8] |= (1<<0)
	#define lcd_data_set_c2_s17() pLCD[8] |= (1<<1)   
	#define lcd_data_set_c2_s18() pLCD[8] |= (1<<2)
	#define lcd_data_set_c2_s19() pLCD[8] |= (1<<3)   
	#define lcd_data_set_c2_s20() pLCD[8] |= (1<<4)
	#define lcd_data_set_c2_s21() pLCD[8] |= (1<<5)   
	#define lcd_data_set_c2_s22() pLCD[8] |= (1<<6)
	#define lcd_data_set_c2_s23() pLCD[8] |= (1<<7)
	#endif
    //---------------------------------------------
	//com3 s0-s23
	#if 1
	// LCDDATA9 SEG<7:0>COM3
	#define lcd_data_set_c3_s00() pLCD[9] |= (1<<0)
	#define lcd_data_set_c3_s01() pLCD[9] |= (1<<1)   
	#define lcd_data_set_c3_s02() pLCD[9] |= (1<<2)
	#define lcd_data_set_c3_s03() pLCD[9] |= (1<<3)   
	#define lcd_data_set_c3_s04() pLCD[9] |= (1<<4)
	#define lcd_data_set_c3_s05() pLCD[9] |= (1<<5)   
	#define lcd_data_set_c3_s06() pLCD[9] |= (1<<6)
	#define lcd_data_set_c3_s07() pLCD[9] |= (1<<7)   
	//LCDDATA10 SEG<15:8>COM3
	#define lcd_data_set_c3_s08() pLCD[10] |= (1<<0)
	#define lcd_data_set_c3_s09() pLCD[10] |= (1<<1)   
	#define lcd_data_set_c3_s10() pLCD[10] |= (1<<2)
	#define lcd_data_set_c3_s11() pLCD[10] |= (1<<3)   
	#define lcd_data_set_c3_s12() pLCD[10] |= (1<<4)
	#define lcd_data_set_c3_s13() pLCD[10] |= (1<<5)   
	#define lcd_data_set_c3_s14() pLCD[10] |= (1<<6)
	#define lcd_data_set_c3_s15() pLCD[10] |= (1<<7)  	
	//LCDDATA11 SEG<23:16>COM3
	#define lcd_data_set_c3_s16() pLCD[11] |= (1<<0)
	#define lcd_data_set_c3_s17() pLCD[11] |= (1<<1)   
	#define lcd_data_set_c3_s18() pLCD[11] |= (1<<2)
	#define lcd_data_set_c3_s19() pLCD[11] |= (1<<3)   
	#define lcd_data_set_c3_s20() pLCD[11] |= (1<<4)
	#define lcd_data_set_c3_s21() pLCD[11] |= (1<<5)   
	#define lcd_data_set_c3_s22() pLCD[11] |= (1<<6)
	#define lcd_data_set_c3_s23() pLCD[11] |= (1<<7)
	#endif	
	//---------------------------------------------
	//com0 s24-s45
	#if 1
	//LCDDATA12 SEG<31:24>COM0
	#define lcd_data_set_c0_s24() pLCD[12] |= (1<<0)
	#define lcd_data_set_c0_s25() pLCD[12] |= (1<<1)   
	#define lcd_data_set_c0_s26() pLCD[12] |= (1<<2)
	#define lcd_data_set_c0_s27() pLCD[12] |= (1<<3)   
	#define lcd_data_set_c0_s28() pLCD[12] |= (1<<4)
	#define lcd_data_set_c0_s29() pLCD[12] |= (1<<5)   
	#define lcd_data_set_c0_s30() pLCD[12] |= (1<<6)
	#define lcd_data_set_c0_s31() pLCD[12] |= (1<<7)   
	//LCDDATA13 SEG<39:32>COM0
	#define lcd_data_set_c0_s32() pLCD[13] |= (1<<0)
	#define lcd_data_set_c0_s33() pLCD[13] |= (1<<1)   
	#define lcd_data_set_c0_s34() pLCD[13] |= (1<<2)
	#define lcd_data_set_c0_s35() pLCD[13] |= (1<<3)   
	#define lcd_data_set_c0_s36() pLCD[13] |= (1<<4)
	#define lcd_data_set_c0_s37() pLCD[13] |= (1<<5)   
	#define lcd_data_set_c0_s38() pLCD[13] |= (1<<6)
	#define lcd_data_set_c0_s39() pLCD[13] |= (1<<7)  	
	//LCDDATA14 SEG<45:40>COM0
	#define lcd_data_set_c0_s40() pLCD[14] |= (1<<0)
	#define lcd_data_set_c0_s41() pLCD[14] |= (1<<1)   
	#define lcd_data_set_c0_s42() pLCD[14] |= (1<<2)
	#define lcd_data_set_c0_s43() pLCD[14] |= (1<<3)   
	#define lcd_data_set_c0_s44() pLCD[14] |= (1<<4)
	#define lcd_data_set_c0_s45() pLCD[14] |= (1<<5)   
	//#define lcd_data_set_c0_s46() pLCD[14] |= (1<<6)
	//#define lcd_data_set_c0_s46() pLCD[14] |= (1<<7)   
	#endif	
	//---------------------------------------------
	//com1 s24-s45
	#if 1
	//LCDDATA15 SEG<31:24>COM0
	#define lcd_data_set_c1_s24() pLCD[15] |= (1<<0)
	#define lcd_data_set_c1_s25() pLCD[15] |= (1<<1)   
	#define lcd_data_set_c1_s26() pLCD[15] |= (1<<2)
	#define lcd_data_set_c1_s27() pLCD[15] |= (1<<3)   
	#define lcd_data_set_c1_s28() pLCD[15] |= (1<<4)
	#define lcd_data_set_c1_s29() pLCD[15] |= (1<<5)   
	#define lcd_data_set_c1_s30() pLCD[15] |= (1<<6)
	#define lcd_data_set_c1_s31() pLCD[15] |= (1<<7)   
	//LCDDATA16 SEG<39:32>COM0
	#define lcd_data_set_c1_s32() pLCD[16] |= (1<<0)
	#define lcd_data_set_c1_s33() pLCD[16] |= (1<<1)   
	#define lcd_data_set_c1_s34() pLCD[16] |= (1<<2)
	#define lcd_data_set_c1_s35() pLCD[16] |= (1<<3)   
	#define lcd_data_set_c1_s36() pLCD[16] |= (1<<4)
	#define lcd_data_set_c1_s37() pLCD[16] |= (1<<5)   
	#define lcd_data_set_c1_s38() pLCD[16] |= (1<<6)
	#define lcd_data_set_c1_s39() pLCD[16] |= (1<<7)  	
	//LCDDATA17 SEG<45:40>COM0
	#define lcd_data_set_c1_s40() pLCD[17] |= (1<<0)
	#define lcd_data_set_c1_s41() pLCD[17] |= (1<<1)
	#define lcd_data_set_c1_s42() pLCD[17] |= (1<<2)
	#define lcd_data_set_c1_s43() pLCD[17] |= (1<<3)   
	#define lcd_data_set_c1_s44() pLCD[17] |= (1<<4)
	#define lcd_data_set_c1_s45() pLCD[17] |= (1<<5)   
	//#define lcd_data_set_c1_s46() pLCD[17] |= (1<<6)
	//#define lcd_data_set_c1_s47() pLCD[17] |= (1<<7)   
	#endif		
	//---------------------------------------------
	//com2 s24-s45
	#if 1
	//LCDDATA18 SEG<31:24>COM0
	#define lcd_data_set_c2_s24() pLCD[18] |= (1<<0)
	#define lcd_data_set_c2_s25() pLCD[18] |= (1<<1)   
	#define lcd_data_set_c2_s26() pLCD[18] |= (1<<2)
	#define lcd_data_set_c2_s27() pLCD[18] |= (1<<3)   
	#define lcd_data_set_c2_s28() pLCD[18] |= (1<<4)
	#define lcd_data_set_c2_s29() pLCD[18] |= (1<<5)   
	#define lcd_data_set_c2_s30() pLCD[18] |= (1<<6)
	#define lcd_data_set_c2_s31() pLCD[18] |= (1<<7)   
	//LCDDATA19 SEG<39:32>COM0
	#define lcd_data_set_c2_s32() pLCD[19] |= (1<<0)
	#define lcd_data_set_c2_s33() pLCD[19] |= (1<<1)   
	#define lcd_data_set_c2_s34() pLCD[19] |= (1<<2)
	#define lcd_data_set_c2_s35() pLCD[19] |= (1<<3)   
	#define lcd_data_set_c2_s36() pLCD[19] |= (1<<4)
	#define lcd_data_set_c2_s37() pLCD[19] |= (1<<5)   
	#define lcd_data_set_c2_s38() pLCD[19] |= (1<<6)
	#define lcd_data_set_c2_s39() pLCD[19] |= (1<<7)  	
	//LCDDATA20 SEG<45:40>COM0
	#define lcd_data_set_c2_s40() pLCD[20] |= (1<<0)
	#define lcd_data_set_c2_s41() pLCD[20] |= (1<<1)   
	#define lcd_data_set_c2_s42() pLCD[20] |= (1<<2)
	#define lcd_data_set_c2_s43() pLCD[20] |= (1<<3)   
	#define lcd_data_set_c2_s44() pLCD[20] |= (1<<4)
	#define lcd_data_set_c2_s45() pLCD[20] |= (1<<5)   
	//#define lcd_data_set_c2_s46() pLCD[20] |= (1<<6)
	//#define lcd_data_set_c2_s47() pLCD[20] |= (1<<7) 
	//---------------------------------------------
	//com3 s24-s45
    #endif
	#if 1
	//LCDDATA21 SEG<31:24>COM0
	#define lcd_data_set_c3_s24() pLCD[21] |= (1<<0)
	#define lcd_data_set_c3_s25() pLCD[21] |= (1<<1)   
	#define lcd_data_set_c3_s26() pLCD[21] |= (1<<2)
	#define lcd_data_set_c3_s27() pLCD[21] |= (1<<3)   
	#define lcd_data_set_c3_s28() pLCD[21] |= (1<<4)
	#define lcd_data_set_c3_s29() pLCD[21] |= (1<<5)   
	#define lcd_data_set_c3_s30() pLCD[21] |= (1<<6)
	#define lcd_data_set_c3_s31() pLCD[21] |= (1<<7)   
	//LCDDATA22 SEG<39:32>COM0
	#define lcd_data_set_c3_s32() pLCD[22] |= (1<<0)
	#define lcd_data_set_c3_s33() pLCD[22] |= (1<<1)   
	#define lcd_data_set_c3_s34() pLCD[22] |= (1<<2)
	#define lcd_data_set_c3_s35() pLCD[22] |= (1<<3)   
	#define lcd_data_set_c3_s36() pLCD[22] |= (1<<4)
	#define lcd_data_set_c3_s37() pLCD[22] |= (1<<5)   
	#define lcd_data_set_c3_s38() pLCD[22] |= (1<<6)
	#define lcd_data_set_c3_s39() pLCD[22] |= (1<<7)  	
	//LCDDATA23 SEG<45:40>COM0
	#define lcd_data_set_c3_s40() pLCD[23] |= (1<<0)
	#define lcd_data_set_c3_s41() pLCD[23] |= (1<<1)   
	#define lcd_data_set_c3_s42() pLCD[23] |= (1<<2)
	#define lcd_data_set_c3_s43() pLCD[23] |= (1<<3)   
	#define lcd_data_set_c3_s44() pLCD[23] |= (1<<4)
	#define lcd_data_set_c3_s45() pLCD[23] |= (1<<5)   
	//#define lcd_data_set_c3_s46() pLCD[23] |= (1<<6)
	//#define lcd_data_set_c3_s47() pLCD[23] |= (1<<7) 
    #endif				
#ifdef __cplusplus
    }
#endif
#endif