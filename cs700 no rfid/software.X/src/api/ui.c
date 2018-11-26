#include "../soc/soc.h"
#include "../includes/includes.h"
//#include <stdio.h>
#ifndef cfg_UI_DEGUB
#define cfg_UI_DEGUB 0
#endif
#ifndef cfg_LCD_DIG_NUM
#define cfg_LCD_DIG_NUM 8
#endif

const uint8_t hexTable[] = "0123456789abcdef";
uint8_t lcdBuf[9] = {0};
uint8_t menu = 0x00;
uint8_t bkupMenu=0x00;
//bool login=false;
uint16_t password = 0x00;
uint8_t uiAdjbuf[0x08];
/*
#define ui_disp_str(str) do{ 	\
        lcd_disp_str(str); 		\
        lcd_disp_refresh(); 	\
        }while(0);
*/
//ui_disp_str
void ui_uint32_2_str(uint8_t* buf, int32_t x, uint8_t len) {

    if (!len)return;
    __t32 = m_math_pow(10, len);
    if (x >= __t32 - 1)x = __t32 - 1;
    while (len--) {
        __t32 = x % 10;
        *buf-- = hexTable[__t32];
        x /= 10;
    }
}
uint8_t ui_head_hide(uint8_t* str, uint8_t loc) {
    uint8_t ret=0;
    while (loc--) {
        if (*str == '0' || *str == ' ') {
            *str++ = ' ';
            ret++;
        } else {
            break;
        }
    }
    return ret;
}
/*
void ui_disp_hello(void) {
    lcd_clear_all();
    lcd_disp_str((uint8_t*) " -hello-");
    //lcd_disp_refresh();
    delay_1s();
}
*/
void ui_mem_cpy(uint8_t* d, uint8_t* s) {
    uint8_t len = 0;
    while (*s != '\0' && len < 8) {
        *d++ = *s++;
        len++;
    }
}

void ui_disp_str_num(uint8_t* str, uint16_t num) {
    //uint16_t t;
    lcd_clear_buffer();
    m_mem_set(lcdBuf, ' ', 8);
    lcdBuf[8] = '\0';
    ui_mem_cpy(lcdBuf, str);
    __t16 = num;
    lcdBuf[7] = '0' + (__t16 % 10);
    __t16 /= 10;
    if (__t16 > 0) {
        lcdBuf[6] = '0' + (__t16 % 10);
        __t16 /= 10;
        if (__t16 > 0) {
            lcdBuf[5] = '0' + (__t16 % 10);
            __t16 /= 10;
            if (__t16 > 0) {
                lcdBuf[4] = '0' + (__t16 % 10);
            }
        }
    }
    lcd_disp_str(lcdBuf);
    //lcd_disp_refresh();
}
void ui_disp_str_num_ex(uint8_t* str, uint16_t num) {
    //uint16_t t;
    lcd_clear_buffer();
    m_mem_set(lcdBuf, ' ', 8);
    lcdBuf[8] = '\0';
    ui_mem_cpy(lcdBuf, str);
    lcdBuf[5]='0';
    lcdBuf[6]='0';
    __t16 = num;
    lcdBuf[7] = '0' + (__t16 % 10);
    __t16 /= 10;
    if (__t16 > 0) {
        lcdBuf[6] = '0' + (__t16 % 10);
        __t16 /= 10;
        if (__t16 > 0) {
            lcdBuf[5] = '0' + (__t16 % 10);
            __t16 /= 10;
            if (__t16 > 0) {
                lcdBuf[4] = '0' + (__t16 % 10);
            }
        }
    }
    lcd_disp_str(lcdBuf);
    //lcd_disp_refresh();
}

/*
void ui_disp_logo(uint8_t dly) {
    ui_disp_str_dly((uint8_t*) " cs700", dly);
}
 * */
void ui_start_display(void) {
    uint8_t dly=9;
    lcd_clear_all();
        while((Event & flgEventMenuTimer1Irq)==0);
        Event &= ~flgEventMenuTimer1Irq;    
    lcd_disp_str((uint8_t*) " -hell0-");
        while((Event & flgEventMenuTimer1Irq)==0);
        Event &= ~flgEventMenuTimer1Irq;    
   // delay_1s();
    while(dly>0){
        ui_disp_str_num((uint8_t*)" cs700", dly);
        //delay_1s();
        while((Event & flgEventMenuTimer1Irq)==0);
        Event &= ~flgEventMenuTimer1Irq;
        dly--;
    }
}
//display floq
void ui_disp_flow_error(void)
{
    lcd_clear_buffer();
    lcd_disp_str((uint8_t*)"  error ");
}
void ui_disp_flow(void) {
    uint32_t flw = flow;
    uint8_t decnum;
    if(flowError){
        ui_disp_flow_error();
        return;
    } 
    m_mem_set(lcdBuf, '0', sizeof (lcdBuf));
    lcdBuf[8] = '\0';  
    
    if(mainSystemData.imdFlowUnit==0){
        decnum=mainSystemData.imdFlowResolution;        
    }else{
        decnum=2;
        flw=flw*60;
        flw=flw/100;            
    }
    ui_uint32_2_str((uint8_t*) (lcdBuf + sizeof (lcdBuf) - 2), flw, cfg_LCD_DIG_NUM);
    ui_head_hide(lcdBuf, cfg_LCD_DIG_NUM - 1 - decnum);
    lcd_clear_buffer();
    lcd_disp_dp(decnum, true);
    lcd_disp_str(lcdBuf);        
}

void ui_disp_totale_v(void) {
    uint8_t decnum;
     if(flowError){
        ui_disp_flow_error();
        return;
    }           
    decnum = mainSystemData.volumeResolution;
    __t32=mainSystemData.totalConsumeVolume;
    if (decnum > 3)decnum = 3;
    __t32 += rtVolume_m3;
    m_mem_set(lcdBuf, '0', sizeof (lcdBuf));
    lcdBuf[8] = '\0';
    ui_uint32_2_str((uint8_t*) (lcdBuf + sizeof (lcdBuf) - 2), __t32, cfg_LCD_DIG_NUM);
    ui_head_hide(lcdBuf, cfg_LCD_DIG_NUM - 1 - decnum);
    lcd_clear_buffer();
    lcd_disp_dp(decnum, true);
    lcd_disp_str(lcdBuf);
}
/*

void ui_disp_balance_money(void) {
    int32_t om = rtBalanceMoney;
    uint16_t i;
    uint8_t decnum = mainSystemData.MoneyResolution;
    if (decnum > 3)decnum = 3;
    m_mem_set(lcdBuf, ' ', sizeof (lcdBuf));
    if (om >= 0) {
        ui_uint32_2_str((uint8_t*) (lcdBuf + sizeof (lcdBuf) - 2), om, cfg_LCD_DIG_NUM);
        ui_head_hide(lcdBuf, cfg_LCD_DIG_NUM - 1 - decnum);
    } else {
        ui_uint32_2_str((uint8_t*) (lcdBuf + sizeof (lcdBuf) - 2), 0 - om, cfg_LCD_DIG_NUM);
        ui_head_hide(lcdBuf, cfg_LCD_DIG_NUM - 1 - decnum);

        for (i = 0; i < cfg_LCD_DIG_NUM - 2 - decnum; i++){
            if (lcdBuf[i] != ' ') {
                break;
            }
            continue;
        }

        if (i == 0) {
            m_mem_cpy(lcdBuf, (uint8_t*) "-9999999");
        } else {
            lcdBuf[i - 1] = '-';
        }
    }
    lcd_clear_buffer();
    lcd_disp_dp(decnum, true);
    lcd_disp_str(lcdBuf);
}
 * */
/*
void ui_disp_balance_volume(void) {
    uint16_t i;
    uint8_t decnum = mainSystemData.volumeResolution;
    int32_t ov = mainSystemData.BalanceMoney;
    ov = ov * 1000;
    ov = ov / mainSystemData.price;
    ov -= rtVolume_m3;
    m_mem_set(lcdBuf, '0', sizeof (lcdBuf));
    lcdBuf[8] = '\0';
    if (ov >= 0) {
        ui_uint32_2_str((uint8_t*) (lcdBuf + sizeof (lcdBuf) - 2), ov, cfg_LCD_DIG_NUM - 1);
        ui_head_hide(lcdBuf, 4);
    } else {
        ui_uint32_2_str((uint8_t*) (lcdBuf + sizeof (lcdBuf) - 2), 0 - ov, cfg_LCD_DIG_NUM - 1);
        ui_head_hide(lcdBuf, cfg_LCD_DIG_NUM - 1 - decnum);
        for (i = 0; i < cfg_LCD_DIG_NUM - 1 - decnum; i++) {
            if (lcdBuf[i] != ' ') {
                break;
            }
            continue;
        }
        if (i <= 1) {
            m_mem_cpy(lcdBuf, (uint8_t*) "u-999999");
        } else {
            lcdBuf[i - 1] = '-';
        }
    }
    lcdBuf[0] = 'u';
    lcd_clear_buffer();
    lcd_disp_dp(decnum, true);
    lcd_disp_str(lcdBuf);
}
 * */
/*
void ui_disp_transfer_money(void){
    lcd_clear_buffer();
    if(mainSystemData.cardID ==0 ){
        lcd_disp_str((uint8_t*)"k0      ");
    }else{
        __t32=mainSystemData.transferMoney;
        __t32/=100;
        m_mem_set(lcdBuf, ' ', sizeof (lcdBuf));
        ui_uint32_2_str((uint8_t*) (lcdBuf + sizeof (lcdBuf) - 2), __t32, cfg_LCD_DIG_NUM-1);
        ui_head_hide(lcdBuf, cfg_LCD_DIG_NUM - 1 - 0);   
        lcdBuf[0] = 'k';
        lcdBuf[1] = 'j';
        //lcd_clear_buffer();
        //lcd_disp_dp(decnum, true);
        lcd_disp_str(lcdBuf);    
    }
}
 * */
/*
void ui_disp_balance_and_volume(void){
    //lcd_disp_str((uint8_t*)"00000000");
    m_mem_set(lcdBuf, '0', sizeof (lcdBuf));
    uint8_t ret=0x00;
    __q31=rtBalanceMoney;
    __q31/=100;
    if(__q31>=0){
        if(__q31>3000)__q31=3000;
        ui_uint32_2_str((uint8_t*) (lcdBuf + sizeof (lcdBuf) - 2), __q31, 4);
        ui_head_hide(lcdBuf+4, 3);
    }else{
        __q31=0-__q31;
        if(__q31>999)__q31=999;
        ui_uint32_2_str((uint8_t*) (lcdBuf + sizeof (lcdBuf) - 2), __q31, 4);
        ret=ui_head_hide(lcdBuf+4, 3);
        if(ret)
            lcdBuf[4+ret-1]='-';
        
    }
    //
    __q31=rtBalanceMoney;
    __q31=__q31/mainSystemData.price;
    //__q31=__q31/10;
     if(__q31>=0){
        if(__q31>9999)__q31=9999;
        ui_uint32_2_str(lcdBuf + 3, __q31, 4);
        ui_head_hide(lcdBuf, 3);
    }else{
        __q31=0-__q31;
        if(__q31>999)__q31=999;
        ui_uint32_2_str(lcdBuf + 3, __q31, 3);
        ret=ui_head_hide(lcdBuf, 3);
        if(ret)
        lcdBuf[0+ret-1]='-';
    }
    lcd_clear_buffer();
    lcd_disp_cln(true);
    lcd_disp_dp(4,true);
    //lcd_disp_dp(1,true);
    lcd_disp_str(lcdBuf);
}
*/
void ui_disp_clear_totale(void) {
    ui_disp_totale_v();
    led1_change();
    led2_off();
    led3_off();
}

 /*
#define ui_disp_clear_totale() do{ \
    ui_disp_totale_v(); \
    led1_change(); \
    led2_off(); \
    led3_off(); \
}while(0);
*/
/*
void ui_disp_clear_current(void) {

}*/
/*
#define ui_disp_clear_balance() do{ \
    ui_disp_balance_money(); \
    led2_change(); \
    led1_off(); \
    led3_off(); \
}while(0);
*/
/*
void ui_disp_clear_balance(void){
    //ui_disp_balance_money(); 
    ui_disp_balance_and_volume();
    led2_change(); 
    led1_off(); 
    led3_off(); 
}
*/
void ui_disp_set_line(void) {
    uint16_t t16=(*((uint16_t*)uiAdjbuf));
    m_mem_set(lcdBuf, ' ', sizeof (lcdBuf));
    ui_uint32_2_str((uint8_t*) (lcdBuf + sizeof (lcdBuf) - 2), t16, 4);
    lcdBuf[0]='l';
    lcd_clear_buffer();
    lcd_disp_dp(3, true);
    lcd_disp_str(lcdBuf);
    
    /*
    uint32_t flw = flow;
    uint8_t decnum = mainSystemData.imdFlowResolution;
    if (decnum > 3)decnum = 3;
    
    flw=flw * (*((uint16_t*)uiAdjbuf));
    flw=flw/mainSystemData.flowRatio;
    
    m_mem_set(lcdBuf, '0', sizeof (lcdBuf));
    lcdBuf[8] = '\0';

    ui_uint32_2_str((uint8_t*) (lcdBuf + sizeof (lcdBuf) - 2), flw, cfg_LCD_DIG_NUM - 1);
    ui_head_hide(lcdBuf, cfg_LCD_DIG_NUM - 1 - decnum);

		lcdBuf[0] = 'l';


    lcd_clear_buffer();
    lcd_disp_dp(decnum, true);
     */
    
    lcd_disp_str(lcdBuf);
    //show dp
}


void ui_disp_small_flow_how_care(void){
	//__t8=*((uint8_t*)(uiAdjbuf));
	ui_disp_str_num((uint8_t*)"   cu-", uiAdjbuf[0]);
}
/*
void ui_disp_flow_warning_sul0(void){
	
	switch(menu){
		case 0x80:ui_disp_str((uint8_t*)"d1-set");break;
		case 0x81:
			__t8=*((uint8_t*)(uiAdjbuf));
			ui_disp_str_num((uint8_t*)"d1-", __t8);break;
		case 0x82:
			__t16=*((uint16_t*)(uiAdjbuf));
			ui_disp_str_num((uint8_t*)"d1hh", __t16);break;
		case 0x83:
			__t16=*((uint16_t*)(uiAdjbuf));
			ui_disp_str_num((uint8_t*)"d1hd", __t16);break;
		case 0x84:
			__t16=*((uint16_t*)(uiAdjbuf));
			ui_disp_str_num((uint8_t*)"d1ll", __t16);break;
		case 0x85:
			__t16=*((uint16_t*)(uiAdjbuf));
			ui_disp_str_num((uint8_t*)"d1ld", __t16);break;
		default:		
			break;
	}	
}
void ui_disp_flow_warning_sul1(void){
	switch(menu){
		case 0x90:ui_disp_str((uint8_t*)"d2-set");break;
		case 0x91:
			__t8=*((uint8_t*)(uiAdjbuf));
			ui_disp_str_num((uint8_t*)"d2-", __t8);break;
		case 0x92:
			__t16=*((uint16_t*)(uiAdjbuf));
			ui_disp_str_num((uint8_t*)"d2hh", __t16);break;
		case 0x93:
			__t16=*((uint16_t*)(uiAdjbuf));
			ui_disp_str_num((uint8_t*)"d2hd", __t16);break;
		case 0x94:
			__t16=*((uint16_t*)(uiAdjbuf));
			ui_disp_str_num((uint8_t*)"d2ll", __t16);break;
		case 0x95:
			__t16=*((uint16_t*)(uiAdjbuf));
			ui_disp_str_num((uint8_t*)"d2ld", __t16);break;
		default:
			break;
	}		
}
void ui_disp_calib(void){

}*/	

void ui_disp_modbus_id(void){
	//__t8=*((uint8_t*)(uiAdjbuf));
	ui_disp_str_num((uint8_t*)"id-", uiAdjbuf[0]);
}
/*
void ui_disp_flow_warning_off_set(void){
	switch(menu){
	case 0xd0:ui_disp_str((uint8_t*)"fc-set");break;		
	case 0xd1:
		__t16=*((uint16_t*)(uiAdjbuf));
		ui_disp_str_num((uint8_t*)"bpr-", __t16);break;
	case 0xd2:
		__t16=*((uint16_t*)(uiAdjbuf));
		ui_disp_str_num((uint8_t*)"bcu-", __t16);break;		

	case 0xd3:
		__t16=*((uint16_t*)(uiAdjbuf));
		ui_disp_str_num((uint8_t*)"bts-", __t16);break;				

	case 0xd4:
		__t16=*((uint16_t*)(uiAdjbuf));
		ui_disp_str_num((uint8_t*)"btl-", __t16);break;	
	}
}
 * */
/*
void ui_disp_set_price(void){

    m_mem_set(lcdBuf,'0',sizeof(lcdBuf));
    __t16=*((uint16_t*)(uiAdjbuf));
    ui_uint32_2_str((uint8_t*) (lcdBuf + sizeof (lcdBuf) - 2), *((int16_t*)(uiAdjbuf)), 3);
    ui_head_hide(lcdBuf, 5); 
    lcdBuf[0]='d';
    lcdBuf[1]='j';
    lcdBuf[2]='-';
     lcd_clear_buffer();     
    lcd_disp_dp(2, true);
    lcd_disp_str(lcdBuf);   
}
void ui_disp_set_transferMoneyLimit(void){
	__t16=*((uint16_t*)(uiAdjbuf));
	ui_disp_str_num((uint8_t*)"sj-",__t16);	
}
void ui_disp_set_device_id(void){
	__t16=*((uint16_t*)(uiAdjbuf));
	ui_disp_str_num((uint8_t*)"did-",__t16);	    
}
 * */
/*
void ui_disp_set_balanceWarning(void){
	__q15=*((int16_t*)(uiAdjbuf));
	//__q15*=100;
	if(__q15>=0){
		ui_disp_str_num((uint8_t*)"d1   ",__q15);	
	}else{
		__q15=0-__q15;
		ui_disp_str_num((uint8_t*)"d1  -",__q15);	
	}

}
 * */
/*
void ui_disp_set_balanceOff(void){
	__q15=*((int16_t*)(uiAdjbuf));
	//__q15*=100;
	if(__q15>=0){
		ui_disp_str_num((uint8_t*)"d2   ",__q15);	
	}else{
		__q15=0-__q15;
		ui_disp_str_num((uint8_t*)"d2  -",__q15);	
	}

}
 */
/*
void ui_disp_clear_card_id(void){
	if(mainSystemData.cardID==0)
		lcd_disp_str((uint8_t*)"   c-id0");
	else
		lcd_disp_str((uint8_t*)"   c-id9");
}
 * */
void ui_disp_set_calib(void){
    uint8_t loc=calibLocation;
    m_mem_set(lcdBuf,'0',sizeof(lcdBuf));
    loc%=100;
    lcdBuf[0]=loc/10 + '0';
    lcdBuf[1]=loc%10 + '0';
     ui_uint32_2_str((uint8_t*) (lcdBuf + sizeof (lcdBuf) - 2), *((int16_t*)(uiAdjbuf)), 4);
     ui_head_hide(lcdBuf+2, 4); 
    lcd_clear_buffer();     
    lcd_disp_dp(1, true);
    lcd_disp_str(lcdBuf);
}
void ui_disp_set_imd_flow_uint(void){
    lcd_clear_buffer();
    //ui_disp_str_num((uint8_t*)"sl-",uiAdjbuf[0]);	
    if(uiAdjbuf[0]==0){
        lcd_disp_str((uint8_t*)"    sl-l");
    }else{
        lcd_disp_str((uint8_t*)"    sl-f");
    }
}
void ui_disp_menu(void) {
    Event &= ~flgEventMenuMainRefrash;
    switch (menu )
	{
        case 0: 	{ui_disp_flow();			led3_on();led1_off();led2_off();break;}
        case 1: 	{ui_disp_totale_v();		led1_on();led2_off();led3_off();break;}
        //case 2: 	{ui_disp_balance_and_volume();	led2_on();led1_off();led3_off();break;}
		case 0x10:	{
            password=(uint8_t)(*((uint16_t*)uiAdjbuf));
            ui_disp_str_num_ex((uint8_t*)"psd-", password);
            break;}
		//case 0x20:	{ui_disp_clear_balance();	break;	}
		case 0x30:	{ui_disp_clear_totale();	break;	}
        case 0x40:	{ui_disp_set_line();		break;	}
        case 0xe0:
            ui_disp_set_calib();
            led3_change(); 
            break;
        case 0x50:  {ui_disp_set_calib();break;}
        //case 0x60:  {ui_disp_set_device_id();break;}
		case 0x70:	{ui_disp_small_flow_how_care();break;}
		
		//case 0x80:	{ui_disp_set_price();break;}
		//case 0x90:	{ui_disp_set_transferMoneyLimit();break;}
		case 0xa0:	{ui_disp_modbus_id();			break;}
		
		//case 0xb0:{ui_disp_set_balanceWarning();break;};
		//case 0xb1:{ui_disp_set_balanceOff();break;}	
		
		//case 0xc0:	{ui_disp_recharge();			break;}
		//case 0xc0:	{ui_disp_transfer_money();			break;}
        //case 0xf0:  {ui_disp_set_line_reset();		break;}
		//case 0xd0:
		//case 0xd1:{ui_disp_clear_card_id();break;}
        case 0xf1:{ui_disp_set_imd_flow_uint();break;}
        //case 0xe0:{ui_disp_set_calib();break;}
        
//#if cfg_KEY_FUCTION_WHOLE==1   
/*     
		case 0x50:	
		case 0x51:	
		case 0x52:	
		case 0x53:	{ui_disp_nemu_set_unit();	break;}
		case 0x60:
		case 0x61:
		case 0x62:
		case 0x63:	{ui_disp_menu_set_dec_num();break;}
		
		case 0x80:	
		case 0x81:
		case 0x82:
		case 0x83:
		case 0x84:
		case 0x85:	{ui_disp_flow_warning_sul0();	break;}
		case 0x90:
		case 0x91:	
		case 0x92:
		case 0x93:
		case 0x94:
		case 0x95:	{ui_disp_flow_warning_sul1();	break;}
		
		case 0xb0:	{ui_disp_calib();				break;}
		
		case 0xd0:	{ui_disp_flow_warning_off_set();		break;}
		default:	break;
//#endif
*/
	}
}
/*
void ui_disp_rfid_err(void){
	lcd_clear_buffer();
	//lcd_disp_dp(decnum, true);
	lcd_disp_str(lcdBuf);
		switch(CardError){
            case NO_ERROR:              lcd_disp_str((uint8_t*)"success ");break;
            //case isNEW_CARD:lcd_disp_str((uint8_t*)"success");break;
            //isNEW_CARD
            case isNEW_CARD_INIT_OK:    lcd_disp_str((uint8_t*)"initia  ");break;
            case CARD_NO_BALANCE:       lcd_disp_str((uint8_t*)"noenough");break;
            case OPEN_ACCOUNT_OK:       lcd_disp_str((uint8_t*)"act-open");break;
            case OPEN_ACCOUNT_ERROR:    lcd_disp_str((uint8_t*)"act-oper");break;
            case UPDATE_DEC_OK:         lcd_disp_str((uint8_t*)"up data ");break;
            default :                   ui_disp_str_num((uint8_t*)" err-",CardError);
            break;
	}
}
*/