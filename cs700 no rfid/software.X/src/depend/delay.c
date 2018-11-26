//#include "../../plat_includes.h"
#include "../soc/soc.h"
#include "delay.h"

void delay(uint16_t t)
{
	while(t--)
    {
        __nop();
        __nop();
        __nop();
        __nop();
    }
}
//file end @
//

