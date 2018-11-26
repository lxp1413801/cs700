#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__
#include "modbus.h"
#include "crc16.h"
#ifdef __cplusplus
extern "C"{
#endif

/*
序号	块地址	名称	长度B	取值范围	说明
1		协议版本	1	1~255	版本控制
2		卡类型	1	AA/EE	标识协议为业务数据/配置数据。
业务卡：AA，工具卡：EE
3		公司编号	2	0~65535	标识卡片客户公司的公司编号
4		卡片id	4		用户公司为卡片分配的身份id
		设备id	4		用户公司为对应设备分配的身份id
5		密钥号	1	0~255	标识数据加密的密钥编号；
0：初始密钥 
254：超级密钥 
255：不加密 
其他：升级后的密钥号；
如密钥号不为255则指令区数据须采用XXTea加密。
6		保留	3	随机数	
7		分区信息	16		存储各分区的占用情况
8		公司标识	14		公司的标识符（可以不使用）
9		CRC	2	CRC校验	CRC校验
*/


#ifdef __cplusplus
}
#endif
#endif