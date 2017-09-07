//file name: mfrc522_includes.h
#ifndef __MFRC522_INCLUDES_H__
#define __MFRC522_INCLUDES_H__

	#ifdef __cplusplus
		extern "C"{
	#endif

    #include "../soc/soc.h"
           
    #include "../depend/depend.h"      
	//
	#include "./bal/mfrc522_bal.h"
	
	#include "./hal/mfrc522_hal_def.h"
	#include "./hal/mfrc522_hal.h"
	//
	#include "./pal/mfrc522_picc.h"
    #include "./pal/mfrc522_picc_low.h"
	#include "./pal/mfrc522_mifare.h"
	//
	#include "./apl/mfrc522_apl.h"
            
            
	#ifdef __cplusplus
		}
	#endif
#endif
//file end
