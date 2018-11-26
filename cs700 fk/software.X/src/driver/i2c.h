#ifndef __IIC_H__
#define __IIC_H__
#ifdef __cplusplus
extern "C"{
#endif
enum
{
	IIC_NACK=1,
	IIC_ACK=0,
};	
#define SLV_ADDR 0xa0
#define iic_sda_mode_out() do{IO_RD5_SetDigitalOutput();}while(0);
#define iic_sda_mode_in() do{IO_RD5_SetDigitalInput();}while(0);
#define	iic_sda_hight() do{IO_RD5_SetHigh();}while(0);
#define	iic_sda_low() do{IO_RD5_SetLow();}while(0);
#define iic_sda_get() IO_RD5_GetValue()

#define	iic_scl_hight() do{IO_RD6_SetHigh();}while(0);
#define	iic_scl_low() do{IO_RD6_SetLow();}while(0);

#define iic_scl_mode_out() do{IO_RD6_SetDigitalOutput();}while(0);
extern void iic_start(void);
extern void iic_stop(void);
extern uint8_t iic_send_byte(uint8_t x);
extern uint8_t iic_read_byte(uint8_t addr);
extern uint8_t iic_waite_ack(void);
extern uint8_t iic_received_byte_if_ack(uint8_t ack);
extern void iic_read_n_byte(uint8_t addr,uint8_t* buf,uint16_t len);
extern void iic_write_byte(uint8_t addr,uint8_t x);
extern void iic_write_n_byte(uint8_t addr,uint8_t* buf,uint16_t len);

#ifdef __cplusplus
}
#endif
#endif