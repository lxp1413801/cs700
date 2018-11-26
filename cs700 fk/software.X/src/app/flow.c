#include "../includes/includes.h"
//__bank(5)

volatile uint32_t fl_t32;
volatile uint16_t adcSamplingBuffer[ADC_SAMPLING_BUFFER_SIZE] = {0};
volatile int32_t rtBalanceMoney = 0;
volatile uint16_t flow   = 0; //unit:L/min
volatile uint16_t rtVolume_m3 = 0; //(ms*L/min)/1000*60=ms*L/(1000*ms*60)=L
volatile uint16_t rtVolume_L = 0;
volatile uint16_t adcSamplingTimer = 0;
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
em_offReason offReason = OFF_REASON_NONE;
void in_app_vavle_off(void){
    Event |= flgEventSaveRtflow;
    if(deviceStatus.bits.acPowerDown)return;
    valve_off();
} 
void in_app_vavle_on(void){
    Event |= flgEventSaveRtflow;
    if(deviceStatus.bits.acPowerDown)return;
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
    if (adcValue <= Main_flowCalibrationTable.flowCalibrationArr[0].adcValue)return 0;
    for (i = 0; i < FLOW_CALIBRATION_POINT_NUM-1; i++) {
        if (adcValue >= Main_flowCalibrationTable.flowCalibrationArr[i].adcValue &&
                adcValue < Main_flowCalibrationTable.flowCalibrationArr[i + 1].adcValue)break;
        if(Main_flowCalibrationTable.flowCalibrationArr[i].adcValue == 
                Main_flowCalibrationTable.flowCalibrationArr[i + 1].adcValue){
                //return (i-1);
                break;
        }
    }
    return i;
}


uint16_t app_flow_adc_2_flow(uint8_t index) {

    uint16_t fl1 = 0, fl2 = 0;
    uint16_t ad1 = 0, ad2 = 0;
    if (index == 0)return 0;
    if (index < FLOW_CALIBRATION_POINT_NUM - 1) {
        fl1 = Main_flowCalibrationTable.flowCalibrationArr[index].rtflowValue;
        fl2 = Main_flowCalibrationTable.flowCalibrationArr[index + 1].rtflowValue;
        ad1 = Main_flowCalibrationTable.flowCalibrationArr[index].adcValue;
        ad2 = Main_flowCalibrationTable.flowCalibrationArr[index + 1].adcValue;
        if(ad2 == ad1)return (Main_flowCalibrationTable.maxFlow *2 );
        fl_t32 = fl1 + (fl2 - fl1)*(adcAverageValue - ad1) / (ad2 - ad1);
    } 
	fl_t32 = fl_t32 * mainSystemData.flowRatio;
    fl_t32 /= 1000;  
    __t32=Main_flowCalibrationTable.maxFlow;
   __t32 =__t32 *  mainSystemData.smallFlowNotCare ;
   __t32 = __t32 /100;
   if(fl_t32<__t32)return 0;
    return (uint16_t)fl_t32;
}

void app_flow_rt_volume_cal(void) {

    //fl_t16 = adcSamplingBuffer[ADC_SAMPLING_BUFFER_SIZE - 1];
   // fl_t32 = app_flow_flow_cal(fl_t16);
    __irq_disable();
    fl_t16 = adcSamplingTimer;
    __irq_enable();
    fl_t32=flow;
    fl_t32 = fl_t32*fl_t16;
    fl_t32 /= 1000; //s*l/min;
    rtVolume_L += (uint16_t)(fl_t32 / 60);
    //fl_t32 = rtVolume_L;
    rtVolume_m3 += (rtVolume_L / 1000);
    rtVolume_L %= 1000;
    if(rtVolume_m3>cfg_FLOW_SAVE_THRESHOLD){
        Event |=flgEventSaveRtflow;
    }
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
        in_app_vavle_off();
    } else {
        deviceStatus.bits.noEnough=0;

    }
}
void flow_sampling_process(void) {
    uint8_t i;
    Event &= ~flgEvenAdcSamplling;
    adcvalue = adc_mcp3241_read_convert_16(&i);
    __nop();
    if ((i & 0x80) == 0) {
        __nop();
        //adcAverageValue = app_flow_put_adc_value_to_buffer(adcvalue);
        adcAverageValue=adcvalue;
        __nop();
    } else {
       adcvalue=0; //
    }
    i = app_flow_lookup(adcvalue);
    flow = app_flow_adc_2_flow(i);
    app_flow_rt_volume_cal();
    app_flow_rt_balance_money_cal();
    //app_flow_flow_waring(0);
    //app_flow_flow_waring(1);
    app_flow_money_warning();
    app_flow_money_off();
	
    adc_mcp3241_start_convert_16(ADC_RESULT_BITS_16, ADC_GAIN_1);
    
}

