#ifndef LOBOTSERVOCONTROLLER_H_
#define LOBOTSERVOCONTROLLER_H_

#include "main.h"

/* 大叉舵机ID */

#define ID1 1
#define ID2 2
#define ID3 3
#define ID4 4
#define ID5 5
#define ID6 6



/* 小叉舵机ID */

#define ID7 7
#define ID8 8
#define ID9 9
#define ID10 10
#define ID11 11
#define ID12 12


/* 背篓舵机ID */

#define ID13 13
#define ID14 14
#define ID15 15
#define ID16 16
#define ID17 17
#define ID18 18
#define ID19 19
#define ID20 20


#define LOBOT_SERVO_FRAME_HEADER         0x55
#define LOBOT_SERVO_MOVE_TIME_WRITE      1
#define LOBOT_SERVO_MOVE_TIME_READ       2
#define LOBOT_SERVO_MOVE_TIME_WAIT_WRITE 7
#define LOBOT_SERVO_MOVE_TIME_WAIT_READ  8
#define LOBOT_SERVO_MOVE_START           11
#define LOBOT_SERVO_MOVE_STOP            12
#define LOBOT_SERVO_ID_WRITE             13
#define LOBOT_SERVO_ID_READ              14
#define LOBOT_SERVO_ANGLE_OFFSET_ADJUST  17
#define LOBOT_SERVO_ANGLE_OFFSET_WRITE   18
#define LOBOT_SERVO_ANGLE_OFFSET_READ    19
#define LOBOT_SERVO_ANGLE_LIMIT_WRITE    20
#define LOBOT_SERVO_ANGLE_LIMIT_READ     21
#define LOBOT_SERVO_VIN_LIMIT_WRITE      22
#define LOBOT_SERVO_VIN_LIMIT_READ       23
#define LOBOT_SERVO_TEMP_MAX_LIMIT_WRITE 24
#define LOBOT_SERVO_TEMP_MAX_LIMIT_READ  25
#define LOBOT_SERVO_TEMP_READ            26
#define LOBOT_SERVO_VIN_READ             27
#define LOBOT_SERVO_POS_READ             28
#define LOBOT_SERVO_OR_MOTOR_MODE_WRITE  29
#define LOBOT_SERVO_OR_MOTOR_MODE_READ   30
#define LOBOT_SERVO_LOAD_OR_UNLOAD_WRITE 31
#define LOBOT_SERVO_LOAD_OR_UNLOAD_READ  32
#define LOBOT_SERVO_LED_CTRL_WRITE       33
#define LOBOT_SERVO_LED_CTRL_READ        34
#define LOBOT_SERVO_LED_ERROR_WRITE      35
#define LOBOT_SERVO_LED_ERROR_READ       36

#define LOBOT_DEBUG 1


#define	ID1_OUT								0x800
#define	ID1_BACK							0x2f7
#define	ID2_OUT								0x800
#define	ID2_BACK							0xd09
#define	ID3_OUT								0x800
#define	ID3_BACK							0xd09
#define	ID4_OUT								0x800
#define	ID4_BACK							0x2f7
#define	ID5_OUT								0xd5e
#define	ID5_BACK							0x10ff
#define	ID6_OUT								0x8e2   //4ec
#define	ID6_BACK							0x5a9  //1b3


#define	ID7_OUT								0x800  
#define	ID7_BACK							0xd09
#define	ID10_OUT							0x800  
#define	ID10_BACK							0xd09

#define	ID8_OUT								0x800  
#define	ID8_BACK							0x2f7
#define	ID9_OUT								0x800  
#define	ID9_BACK							0x2f7  

#define	ID11_OUT							0xa0b
#define	ID11_BACK							0x547
#define	ID12_OUT							0x202
#define	ID12_BACK							0x697


#define	ID13_OUT							0x800
#define	ID13_BACK							0x213
#define	ID14_OUT							0x800
#define	ID14_BACK							0xded
#define	ID15_OUT							0x800
#define	ID15_BACK							0xded
#define	ID16_OUT							0x800
#define	ID16_BACK							0x213
#define	ID17_OUT							0x800
#define	ID17_BACK							0x213
#define	ID18_OUT							0x800
#define	ID18_BACK							0x213
#define	ID19_OUT							0x800
#define	ID19_BACK							0xded
#define	ID20_OUT							0x800
#define	ID20_BACK							0xded


#define	FloatWindow_SG				80


struct BigForkServoState	
{	
	u8 ID1_state;
	u8 ID2_state;
	u8 ID3_state;
	u8 ID4_state;
	u8 ID5_state;
	u8 ID6_state;
};

struct SmallForkServoState	
{	
	u8 ID7_state;
	u8 ID8_state;
	u8 ID9_state;
	u8 ID10_state;
	u8 ID11_state;
	u8 ID12_state;
};

struct BackPackServoState	
{	
	u8 ID13_state;
	u8 ID14_state;
	u8 ID15_state;
	u8 ID16_state;
	u8 ID17_state;
	u8 ID18_state;
	u8 ID19_state;
	u8 ID20_state;
};

extern struct BigForkServoState big_fork_servo_state;
extern struct SmallForkServoState small_fork_servo_state;
extern struct BackPackServoState back_pack_servo_state;

extern uint8_t LobotRxBuf[16];

uint8_t LobotCheckSum(uint8_t buf[]);
void LobotSerialServoSetID(uint8_t oldID, uint8_t newID);
void LobotSerialServoMove(uint8_t id, int16_t position, uint16_t time);
void LobotSerialServoUnload(uint8_t id);
void LobotSerialServoLoad(uint8_t id);
int LobotSerialServoReadPosition(uint8_t id);
int LobotSerialMsgHandle(void);

void Scan_LobotSerialServo(void);	/* 轮询扫描舵机标志 */	
void Close_LobotSerialServo(void);



void ServoMove_Bigfork12_L_Out(void);
void ServoMove_Bigfork12_L_Back(void);
void ServoMove_Bigfork34_R_Out(void);
void ServoMove_Bigfork34_R_Back(void);
void ServoMove_Smallfork56_Out(void);
void ServoMove_Smallfork56_Back(void);
void ServoMove_BackPack_Back(void);
void ServoMove_BackPack_Out(void);
void ScanServoFinishFlag(void);
bool MakeServoFinished(int servoid, int position);

void LobotSerialServoSetID_new(uint8_t oldID, uint8_t newID);																/* 新舵机ID */
uint8_t LobotCheckSum_new(uint8_t *buf);			/* 新舵机校验和 */
void LobotSerialServoReadID_new(void);
void LobotSerialServoMoveConfig(uint8_t id, int16_t position, uint16_t time);													/* 异步写运动配置指令 */
void	LobotSerialServoMove_new(void);
int LobotSerialServoReadPosition_new(uint8_t id);

int LobotSerialMsgHandle_new(uint8_t id);
#endif



