/*
 * uwb_msg.h
 *
 *  Created on: May 26, 2024
 *      Author: 24848
 */

#ifndef DEVICES_INC_UWB_MSG_H_
#define DEVICES_INC_UWB_MSG_H_


#include "uwb.h"

typedef enum{
	beacon_frame = 0,
	data_frame = 1,
	ack_frame = 2,
	mac_cmd_frame = 3,
	compound_frame = 5,
}Frame_Type_t;

typedef enum{
	//定位
	UWB_Ranging_Poll = 0,
	UWB_Ranging_Resp,
	UWB_Ranging_Final,
	UWB_Ranging_Ack,

	UWB_App_Data,

	UWB_Cmd_Req = 0x11,

}Frame_Function_t;  //功能


typedef enum{
	initiator = 0,
	responder = 1,
}UWB_Ranging_Role;

/**
 * 定义消息结构体
 */

/*
 * 802.15.4 帧头部
 *|     帧控制     | 序列号 |   目标PAN ID  |   目标地址  |  源地址    |  xx      payload  |  FCS   |
 *  2 + 1 +  2 +  2 + 2 = 9
 * */
//802.15.4前两字节为帧控制字段
/*
 * 帧类型    b2-0             001  数据帧   000    101
 * 安全使能   b3              0
 * 帧Pending   b4			0
 * 需要ACK  b5				0
 * PAN ID 压缩   b6			1     //1表示只有Dest PAN ID,0两者都有
 * 保留      b7-9			000
 * 目标地址模式   b10-11		10  目标地址是短地址
 * 帧版本         b12-13      00
 * 源地址模式      b14-15      10 源地址为短地址
 * */
//----> 两个字节的Frame control :  0x88(暂时高字节可以固定)
//发送，低字节在前

#define UWB_MAC_HEADER_LEN		9

#define ALL_MSG_COMMON_LEN 		9
#define ALL_MSG_ADD_LEN         2
#define ALL_MSG_DIST_PAN_IDX    3
#define ALL_MSG_DIST_IDX        5
#define ALL_MSG_SRC_IDX     	7

#define FRAME_CONTROL_HIGH		0x88
#define FRAME_CONTROL_LOW		0x41

#define FRAME_CONTROL			0x8841

#define	FRAME_TYPE_CUS			0x0380

/**
 * 设置帧类型字段
 * 设置需要ACK请求
 */
#define SET_FRAMECONTROL(type, ack)		(FRAME_CONTROL|((type)&0x07)|((ack)&0x0020))

//#define GET_FRAMETYPE(control)			(control&0x07)

#define BEACON_CONTROL					(FRAME_CONTROL)
#define POLL_CONTROL					(FRAME_CONTROL|(0x0001<<7))
#define RESP_CONTROL					(FRAME_CONTROL|(0x0001<<7))
#define FINAL_CONTROL					(FRAME_CONTROL|(0x0001<<7))    //不需要响应的


#define POLL_COMP_CONTROL				(FRAME_CONTROL|(0x0005<<7))
#define RESP_COMP_CONTROL				(FRAME_CONTROL|(0x0005<<7))
#define FINAL_COMP_CONTROL				(FRAME_CONTROL|(0x0005<<7))

#define MAC_CMD_CONTROL					(FRAME_CONTROL|(0x0003<<7))
#define ACK_FRAME_CONTROL				(FRAME_CONTROL|(0x0002<<7))
//
//#define MAC_CMD_CONTROL					(0x8843)
//#define ACK_FRAME_CONTROL				(0x8842)

#define GET_FRAMETYPE(control)			((control&FRAME_TYPE_CUS)>>7)
//#define GET_FRAMETYPE(control)			(control&0x0007)

//数据包载荷的字节大小
#define POLL_PAYLOAD_LEN		(0+1+2)	//功能码 + FCS
#define RESP_PAYLOAD_LEN		(0+1+2)
#define FINAL_PAYLOAD_LEN		(24+1+2)
#define ACK_PAYLOAD_LEN			(32+1+2) //距离(4)，方向角(2/4)，高度(2), 速度，偏航角yaw

#define PAIR_REQ_PAYLOAD_LEN	(1+2)

#define	JOIN_REQ_PAYLOAD_LEN	(1+1+2)

#define POLL_MSG_LEN			(UWB_MAC_HEADER_LEN + POLL_PAYLOAD_LEN)
#define RESP_MSG_LEN			(UWB_MAC_HEADER_LEN + RESP_PAYLOAD_LEN)
#define FINAL_MSG_LEN			(UWB_MAC_HEADER_LEN + FINAL_PAYLOAD_LEN)
#define ACK_MSG_LEN				(UWB_MAC_HEADER_LEN + ACK_PAYLOAD_LEN)

//以之作为blink消息，等待tag发现，然后去配对
#define BEACON_COM_LEN			(UWB_MAC_HEADER_LEN + 2) // here is my position

#define PAIR_REQ_MSG_LEN		(UWB_MAC_HEADER_LEN + JOIN_REQ_PAYLOAD_LEN)


#pragma pack(1)

//修改这个frame control 里面的保留字节吧，对应到这个数据包的类型
typedef struct{
	uint16_t control;
	uint8_t sequence;
	uint16_t pan_id;
	uint16_t dist;
	uint16_t src;
}UWB_Msg_Header_t;   //9

typedef struct{
	uint64	poll_tx_ts;
	uint64	resp_rx_ts;
	uint64	final_tx_ts;
}UWB_Ranging_Final_t;   //24

/**
 * @TODO ACK msg payload specification
 */
typedef struct{
	int64	tof_dtu;
	int64	D1;
	int64	D2;
	int64	D3;
}UWB_Ranging_ACK_t;   //32


typedef struct{
	uint8_t function;
	//This is going to be right you know
	union{
		UWB_Ranging_Final_t  final_payload;
	};
}UWB_Function1_t;

//数据帧
typedef struct{

	UWB_Msg_Header_t header;
	UWB_Function1_t payload1;

}UWB_Data_Frame_t;


typedef struct{
	uint8_t function;
}UWB_Mac_Payload_t;
//MAC命令帧 —— 用于时隙管理
typedef struct{
	UWB_Msg_Header_t header;
	UWB_Mac_Payload_t payload;
}UWB_Mac_Frame_t;   //2  or 1 is also needed here, I can do this you know, I am familiar with it now,

#define UWB_Blink_Frame_t	UWB_Msg_Header_t
#define UWB_Ack_Frame_t 	UWB_Msg_Header_t

typedef struct{
	uint8_t function;
	//127 -7 -2(FCS) - 1- 32 -1 = 84
	uint8_t data[84];
}UWB_Function2_t;

typedef struct{
	UWB_Msg_Header_t header;
	UWB_Function1_t payload1;
	UWB_Function2_t payload2;
}UWB_Comp_Frame_t;

#pragma pack()


#endif /* DEVICES_INC_UWB_MSG_H_ */
