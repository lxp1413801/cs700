#ifndef __flow_h__
#define __flow_h__

#ifdef __cplusplus
extern "C" {
#endif
#define ADC_SAMPLING_BUFFER_SIZE 20
    extern bool flowError;
    extern volatile uint16_t adcSamplingTimer;
    extern volatile uint16_t adcSamplingBuffer[ADC_SAMPLING_BUFFER_SIZE];
    extern volatile uint8_t smallFlowNotCare;
    //extern volatile uint16_t flowRatio;
    extern volatile uint16_t adcAverageValue;
    extern volatile uint16_t flow;
    
    extern volatile uint32_t rtVolume_m3  ; //(ms*L/min)/1000*60=ms*L/(1000*ms*60)=L
    extern volatile uint32_t rtVolume_L  ;
   // extern volatile int32_t rtBalanceMoney  ;
    //extern void in_app_vavle_on(void);
    extern uint16_t app_flow_put_adc_value_to_buffer(uint16_t t16);

    extern uint32_t app_flow_flow_cal(uint16_t adcvalue);
    extern void app_flow_flow_waring(uint8_t sol);
    extern void app_flow_rt_volume_cal(void);

    extern void flow_sampling_process(void);
    extern void app_flow_rt_volume_cal_ex(void);

#define app_flow_all_calculation() do{ \
        flow=app_flow_flow_cal(adcAverageValue); \
        app_flow_rt_volume_cal(); \
        app_flow_rt_balance_money_cal(); \
        app_flow_flow_waring(0); \
        app_flow_flow_waring(1); \
        app_flow_money_warning(); \
        app_flow_money_off(); \
    }while(0);

#ifdef __cplusplus
}
#endif
#endif
//file end
