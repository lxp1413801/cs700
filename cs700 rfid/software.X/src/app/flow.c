#include "../includes/includes.h"
//__bank(5)
bool flowError=false;
volatile uint32_t fl_t32;
volatile uint16_t adcSamplingBuffer[ADC_SAMPLING_BUFFER_SIZE] = {0};
volatile int32_t rtBalanceMoney = 0;
volatile uint16_t flow   = 0; //unit:L/min
volatile uint32_t rtVolume_m3 = 0; //(ms*L/min)/1000*60=ms*L/(1000*ms*60)=L
volatile uint32_t rtVolume_L = 0;
volatile uint16_t adcSamplingTimer = 0;
volatile uint16_t adcSamplingTimerOneTime=0;
volatile uint16_t adcAverageValue = 0;
volatile uint16_t fl_t16;
volatile uint16_t adcvalue = 0;
volatile uint8_t fl_t8;
volatile uint8_t smallFlowNotCare=0x00;
//volatile uint16_t flowRatio = 0x00;
st_warningStatus warningStatus = {0};
//st_lockStatus lockStatus = {0};
#ifndef  cfg_FLOW_SAVE_THRESHOLD
#define cfg_FLOW_SAVE_THRESHOLD 10000
#endif
volatile em_offReason opReason = OFF_REASON_NONE;

void in_app_vavle_off(opReason_t op)
{
        //if()
	if(cardReadingFlag)return ;
     if(valveStatus==VALVE_CLOSED || valveStatus==VALVE_OPENING || valveStatus==VALVE_CLOSEING)return;
    //if(valveStatus!=VALVE_CLOSED && valveStatus!=VALVE_CLOSEING){
        
        if(deviceStatus.bits.acPowerDown)return;
        //Event |= flgEventSaveRtflow;
        opReason=op;
        valve_off();
   // }
    if(valveStatus==VALVE_CLOSED || valveStatus==VALVE_CLOSEING)return;
    if(deviceStatus.bits.acPowerDown)return;
    opReason=op;
    valve_off();        
} 

void in_app_vavle_on(opReason_t op)
{
	if(cardReadingFlag)return ;
    if(op==ON_REASON_HA && opReason==OFF_REASON_MAN)return;
    if(valveStatus==VALVE_OPENED || valveStatus==VALVE_OPENING || valveStatus==VALVE_CLOSEING)return;
    if(deviceStatus.bits.acPowerDown)return;
    //Event |= flgEventSaveRtflow;
    opReason=op;
    valve_on();
} 
uint16_t app_flow_put_adc_value_to_buffer(uint16_t x) {
    /*
    fl_t32 = 0;
    fl_t16 = 0;
    for (fl_t8 = 0; fl_t8 < ADC_SAMPLING_BUFFER_SIZE - 1; fl_t8++) {
        fl_t16 = adcSamplingBuffer[fl_t8 + 1];
        adcSamplingBuffer[fl_t8] = fl_t16;
        fl_t32 = fl_t32 + fl_t16;
    }
    adcSamplingBuffer[fl_t8] = x;
    fl_t32 += x;
    fl_t32 /= ADC_SAMPLING_BUFFER_SIZE;
    return (uint16_t) fl_t32;
     */
    return x;
}


uint8_t app_flow_lookup(uint16_t adcValue) {
    uint8_t i;
    if (adcValue < Main_flowCalibrationTable.flowCalibrationArr[0].adcValue)return 0;
    for (i = 0; i < FLOW_CALIBRATION_POINT_NUM-1; i++) {
        if (adcValue >= Main_flowCalibrationTable.flowCalibrationArr[i].adcValue &&
                adcValue < Main_flowCalibrationTable.flowCalibrationArr[i + 1].adcValue)break;
        if(Main_flowCalibrationTable.flowCalibrationArr[i].adcValue >= 
                Main_flowCalibrationTable.flowCalibrationArr[i + 1].adcValue){
            //return (i-1);
            break;
        }
    }
    return i;
}


uint16_t app_flow_adc_2_flow(uint8_t index) {

    uint16_t flw1 = 0, flw2 = 0;
    uint16_t ad1 = 0, ad2 = 0;
    //if (index == 0)return 0;
	//add 20170920
	if(valveStatus==VALVE_CLOSED){
		flowError=false;
		return 0;
	}
	//add end
    fl_t32=1;
    if (index < FLOW_CALIBRATION_POINT_NUM - 1) {
		ad1 = Main_flowCalibrationTable.flowCalibrationArr[index].adcValue;
		ad2 = Main_flowCalibrationTable.flowCalibrationArr[index + 1].adcValue;
		flw1 = Main_flowCalibrationTable.flowCalibrationArr[index].rtflowValue;
		flw2 = Main_flowCalibrationTable.flowCalibrationArr[index + 1].rtflowValue;       
		if(ad2 == ad1){
			//return (Main_flowCalibrationTable.maxFlow *2 );
			//fl_t32 = 2;
            if(index==0)return 0;
			ad1 = Main_flowCalibrationTable.flowCalibrationArr[index-1].adcValue;
			ad2 = Main_flowCalibrationTable.flowCalibrationArr[index].adcValue;
			flw1 = Main_flowCalibrationTable.flowCalibrationArr[index-1].rtflowValue;
			flw2 = Main_flowCalibrationTable.flowCalibrationArr[index].rtflowValue;            
        }
        if(adcAverageValue < ad1)return 0;

        //fl_t32 = fl1 + (fl2 - fl1)*(adcAverageValue - ad1) / (ad2 - ad1);
        if(flw2<flw1)flw2=flw1;
        fl_t32 = fl_t32*(flw2 - flw1)*(adcAverageValue - ad1);
        if(ad2 > ad1){
            fl_t32 = fl_t32 /(ad2 - ad1);
            fl_t32 += flw1;                  
        }else{
            fl_t32= flw1;   
        }
    } 
	fl_t32 = fl_t32 * mainSystemData.flowRatio;
	fl_t32 /= 1000;  
	__t32=Main_flowCalibrationTable.maxFlow;
	__t32 =__t32 *  mainSystemData.smallFlowNotCare ;
	__t32 = __t32 /100;
	if(fl_t32<__t32)return 0;
    //add by lxp
    __t32=Main_flowCalibrationTable.maxFlow;
	#if 0
    __t32=__t32*11;
    __t32=__t32/10;
	#else
	if(fl_t32>=__t32)fl_t32=__t32;
	#endif
    if(fl_t32>=__t32){
        flowError=true;
    }else{
        flowError=false;
    }
	return (uint16_t)fl_t32;
}

void app_flow_rt_volume_cal(void) {

    //fl_t16 = adcSamplingBuffer[ADC_SAMPLING_BUFFER_SIZE - 1];
   // fl_t32 = app_flow_flow_cal(fl_t16);
    __irq_disable();
    fl_t16 = adcSamplingTimer;
    adcSamplingTimer=0;
    __irq_enable();
    fl_t32=flow;
    fl_t32 = fl_t32*fl_t16;
    fl_t32 /= 1000; //s*l/min;
    rtVolume_L += (uint16_t)(fl_t32 / 60);
    //fl_t32 = rtVolume_L;
    rtVolume_m3 += (rtVolume_L / 1000);
    rtVolume_L %= 1000;
    /*
    if(rtVolume_m3>cfg_FLOW_SAVE_THRESHOLD){
        Event |=flgEventSaveRtflow;
    }*/
}
void app_flow_rt_volume_cal_ex(void) {
    
    fl_t32=flow;
    //fl_t32=36000;
    fl_t32 = fl_t32 * (uint32_t)calVolumeTimeSec;
    fl_t32>>=1;
    //calVolumeTimeSec=0;
    //rtVolume_L +=fl_t32;
    rtVolume_L=rtVolume_L+fl_t32;
    rtVolume_m3=rtVolume_m3+(rtVolume_L / 60000ul);
   // rtVolume_m3 += (rtVolume_L / 60000);
    rtVolume_L = rtVolume_L % 60000ul;
    //rtVolume_m3+=10;
     calVolumeTimeSec=0;
    
}
void app_flow_rt_balance_money_cal(void) {
    //uint32_t t32;
    __q31 = mainSystemData.price;
    //fl_t32 = (fl_t32 * rtVolume_m3 / 1000);
    __q31 = (__q31 * rtVolume_m3 / 10);
    rtBalanceMoney = mainSystemData.BalanceMoney - __q31;
}

void app_flow_money_warning(void) {
    __q31=(int32_t)(mainSystemData.balanceWarning);
    if (rtBalanceMoney < __q31) {
        warningStatus.bits.moneyLowWarning = 1;
    } else {
        warningStatus.bits.moneyLowWarning = 0;
    }
}

void app_flow_money_off(void) {
    __q31=(int32_t)(mainSystemData.balanceOff);
    if (rtBalanceMoney < __q31) {
        warningStatus.bits.moneyLowWarning = 1;
        //lockStatus.bits.overdraftLock = 1;
        deviceStatus.bits.noEnough=1;
        in_app_vavle_off(OFF_REASON_LA);
    } else {
        deviceStatus.bits.noEnough=0;

    }
}
uint16_t app_flow_get_adcAverageValue(void)
{
    /*
    uint8_t i;
    uint32_t ret=0;
    uint16_t max=0;
    uint16_t        min=0x8000;
    for(i=0;i<ADC_SAMPLING_BUFFER_SIZE;i++){
        if(max<adcSamplingBuffer[i])max=adcSamplingBuffer[i];
        if(min>adcSamplingBuffer[i])min=adcSamplingBuffer[i];
        ret+=adcSamplingBuffer[i];
    }
    ret=ret-max;
    ret=ret-min;
    ret=ret/(ADC_SAMPLING_BUFFER_SIZE-2);
    return (uint16_t)ret;
     */
    uint8_t maxIndex,minIndex;
    uint8_t i,j;
    uint32_t ret=0;
    uint16_t max=0,min;
    for(j=0;j<8;j++){
        ret=0;
        max=0;
        min=0x8000;
        maxIndex=0;
        minIndex=0;
        for(i=0;i<ADC_SAMPLING_BUFFER_SIZE;i++){
            if(max<adcSamplingBuffer[i]){
                max=adcSamplingBuffer[i];
                maxIndex=i;
            }
            if(min>adcSamplingBuffer[i]){
                min=adcSamplingBuffer[i];
                minIndex=i;
            }
            ret+=adcSamplingBuffer[i];
        }
        ret=ret-max;
        ret=ret-min;
        ret=ret/(ADC_SAMPLING_BUFFER_SIZE-2);
        adcSamplingBuffer[maxIndex]=(uint16_t)ret;
        adcSamplingBuffer[minIndex]=(uint16_t)ret;
    }
    return (uint16_t)ret;    
}
    uint8_t adcSamplingTimes=0;
void flow_sampling_process(void) {
    
    uint8_t i;
    Event &= ~flgEvenAdcSamplling;

    adcvalue = adc_mcp3241_read_convert_16(&i);
    __nop();
    if ((i & 0x80) == 0) {
        if(adcSamplingTimes<=ADC_SAMPLING_BUFFER_SIZE){
            adcvalue=adcvalue & 0x1fff;
            adcvalue<<=4;
            adcSamplingBuffer[adcSamplingTimes]=adcvalue;
            adcSamplingTimes++;
        }
    } 
    if(adcSamplingTimes>=ADC_SAMPLING_BUFFER_SIZE){
        adcAverageValue=app_flow_get_adcAverageValue();
        adcSamplingTimes=0;
        i = app_flow_lookup(adcAverageValue);
        flow = app_flow_adc_2_flow(i);
        //flow=20000;
        if( Event & flgEvenCalVoleume){
         Event &= ~flgEvenCalVoleume;

            app_flow_rt_volume_cal_ex();
            app_flow_rt_balance_money_cal();
        }

        app_flow_money_warning();
        app_flow_money_off();
	}
    //adc_mcp3241_start_convert_16(ADC_RESULT_BITS_14, ADC_GAIN_1);//ADC_RESULT_BITS_12
    adc_mcp3241_start_convert_16(ADC_RESULT_BITS_12, ADC_GAIN_1);
}

