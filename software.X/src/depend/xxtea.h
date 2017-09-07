#ifndef __XXTEA_H__
#define __XXTEA_H__
//#include "msp430.h"
	#ifdef __cplusplus
	extern "C"
	{
	#endif

        extern const uint32_t XxteaKey[4];
        extern void btea(uint32_t* v, int16_t n, uint32_t* key);

    #ifdef __cplusplus
    }
    #endif
#endif