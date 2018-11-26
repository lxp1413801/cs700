#ifndef __M_STRING_H__
#define __M_STRING_H__

	#ifdef __cplusplus
	extern "C"{
	#endif
	extern void m_mem_set(uint8_t* buf,uint8_t x,uint8_t len);
	extern void m_mem_cpy(uint8_t* d,uint8_t* s);
	extern void m_mem_cpy_len(uint8_t* d,uint8_t* s,uint8_t len);
	extern uint16_t m_str_match(uint8_t* b,uint8_t* c);
	extern int32_t m_math_pow(int32_t x,uint8_t y);
	extern uint16_t m_str_cmp(uint8_t* d,uint8_t* s);
	extern uint16_t m_str_cmp_len(uint8_t* d,uint8_t* s,uint8_t len);
	extern uint16_t HEX8(uint8_t* hex,uint8_t x);
	extern uint16_t m_str_b2h(uint8_t* h,uint8_t* b,uint16_t len);
	extern uint16_t m_str_h2b(uint8_t* b,uint8_t* h,uint16_t len);

	//add 2016.07.05
	extern void m_int_2_str(uint8_t* buf,int32_t x,uint8_t len);
	extern uint8_t m_str_head_hide(uint8_t* str,uint8_t loc);
    //add 2016.08.24
    extern const uint8_t Bcd2HexTable[];
    //#define m_bcd_2_hex(bcd) (bcd>0x99? 99: Bcd2HexTable[bcd])
    #define m_bcd_2_hex(bcd) (bcd>0x99 ? 99 : (bcd>>4)*10 + (bcd & 0x0f))
    //#define m_hex_2_bcd(hex) (hex>99 ? 0x99 : Hex2BcdTable[hex]) 
    #define m_hex_2_bcd(hex) (hex>99 ? 0x99 : (hex/10)<<4 + (hex % 10 )) 
	//add at 2016.10.15
	extern uint32_t swap_uint32(uint32_t x);
	extern uint16_t swap_uint16(uint16_t x);
    #define __SWAP16(x) swap_uint16(x)
    #define __SWAP32(x) swap_uint32(x)
	
	extern void int32_array_shift_right(int32_t *p32,uint16_t eoc);
	#ifdef __cplusplus
	}
	#endif
#endif
//file end
