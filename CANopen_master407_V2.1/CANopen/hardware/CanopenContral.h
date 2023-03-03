#ifndef __CANOPENCONTRAL_H
#define	__CANOPENCONTRAL_H
#include "main.h"

/*电机-------------motor 参数*/
#define BigFork     		1										/* 大叉 */
#define SmallFork   		2										/* 小叉 */



/* 工作模式 -----mode参数 */

#define velocityMode   						3																				/* 速度模式 */
#define positionMode   						1																				/* 位置模式 */
#define homeMode     	 						6																				/* 找原点模式 */

/* 位置浮动窗口 */
#define FloatingWindow						500																				/* 位置浮动窗口 */


/* 
	梯形速度
	大叉最大速度 0.33m/s
	小叉最大速度 0.36m/s
	
	300/7.8932 = 38.007  为大叉每秒圈数
	300/9.5139 = 31.532	为小叉每秒圈数
	
	100/7.8932 = 12.669  为大叉每秒圈数
	100/9.5139 = 10.511	为小叉每秒圈数
	
	300/8.0 = 37.5  为大叉每秒圈数
	300/8.8 = 34.09	为小叉每秒圈数
	
	100/8.0 = 12.5  为大叉每秒圈数
	100/8.8 = 11.36	为小叉每秒圈数
*/

#define LiftBigForkProfile_V_Long			((u32)((37.5 * 60 * 512 * 10000) / 1875))								/* 大叉升降梯形速度 */					/* 空载长距离速度 */
#define LiftSmallForkProfile_V_Long		((u32)((34.09 * 60 * 512 * 10000) / 1875))								/* 小叉升降梯形速度 */

//#define LiftBigForkProfile_V_Long			((u32)((38.007 * 60 * 512 * 10000) / 1875))								/* 大叉升降梯形速度 */					/* 空载长距离速度 */
//#define LiftSmallForkProfile_V_Long		((u32)((31.532 * 60 * 512 * 10000) / 1875))								/* 小叉升降梯形速度 */

//#define LiftBigForkProfile_V_Long			((u32)((12.669 * 60 * 512 * 10000) / 1875))								/* 大叉升降梯形速度 */					/* 空载长距离速度 */
//#define LiftSmallForkProfile_V_Long		((u32)((10.510 * 60 * 512 * 10000) / 1875))								/* 小叉升降梯形速度 */


#define LiftBigForkProfile_V_short			((u32)((12.5 * 60 * 512 * 10000) / 1875))								/* 大叉升降梯形速度 */				/* 负载短距离速度 */
#define LiftSmallForkProfile_V_short		((u32)((11.36 * 60 * 512 * 10000) / 1875))								/* 小叉升降梯形速度 */


//#define LiftBigForkProfile_V_short			((u32)((12.669 * 60 * 512 * 10000) / 1875))								/* 大叉升降梯形速度 */				/* 负载短距离速度 */
//#define LiftSmallForkProfile_V_short		((u32)((10.510 * 60 * 512 * 10000) / 1875))								/* 小叉升降梯形速度 */

//#define LiftBigForkProfile_V_short			((u32)((6.3 * 60 * 512 * 10000) / 1875))								/* 大叉升降梯形速度 */				/* 负载短距离速度 */
//#define LiftSmallForkProfile_V_short		((u32)((10.510 * 60 * 512 * 10000) / 1875))								/* 小叉升降梯形速度 */



/* 
		一圈对应的距离 单位mm 
		大叉电机转1圈，行走7.893251408mm
		小叉电机转1圈，行走9.51399903mm
*/
#define	LiftBigFork_1RadToMM						80000									/* 扩大10000倍 78932*/
#define	LiftSmallFork_1RadToMM					88000									/* 扩大10000倍 95139*/


/* 大小叉层间距 */
#define	Bin_Distance										245																/* 大小叉升降两层相隔间距 */
#define	BackPack_Distance								426																/* 背篓两层相隔间距 */


#define	BigFork_MaxPosition								1084												/* 大叉能够到达的最大位置*/	
#define	BigFork_MinPosition								(-735)												/* 大叉能够到达的最低位置 */
#define	BigFork_MinBackPosition						(-390)												/* 大叉能够到达的最低位置 */
#define	BigFork_Position_When_SmallFork_LowSpeed							(-190)		/* 小叉携带料箱高速运行到最低点时，大叉可到达的最低位置 */
#define	SmallFork_MaxPosition							1356												/* 小叉能够到达的最大位置 */	
#define	SmallFork_MinPosition							(-439)												/* 小叉能够到达的最低位置 */	
#define	SmallFork_MinBackPosition					(-390)												
#define	SmallFork_MinPosition_HighSpeed_HaveTote							(-234)		/* 小叉携带料箱高速运行时可到达的最低位置 */
#define SmallFork_MaxPositon_Collisiion_BigFork               1060      /* 大叉上升到最高位置时，小叉不碰到大叉的最高位置*/

#define SmallFork_InboundPosition        210	//230
#define BigFork_InboundPositin           450
#define SmallFork_OutboundPosition       230
#define BigFork_OutboundPosition         450

/* 大叉小叉取货卸货时抬升距离 */
#define AGVHigh_Small															((int32_t)((float)(15/8.8)*10000))
#define AGVHigh_Big																((int32_t)((float)(120/8.0)*10000))	
#define LiftBigForkADD														((int32_t)((float)(160/8.0)*10000))
#define LiftSmallForkADD													((int32_t)((float)(160/8.8)*10000))
#define LiftSmallForkBackPackADD									((int32_t)((float)(78/8.8)*10000))  //未用到暂时
#define LiftBigForkDec														((int32_t)((float)(120/8.0)*10000))
#define LiftSmallForkDec													((int32_t)((float)(85/8.8)*10000))
#define LiftSmallForkBackPackDec									((int32_t)((float)(95/8.8)*10000))
#define LiftSmallForkMoveDetal                    ((int32_t)((float)(20/8.8)*10000))
#define LiftBigforkrkMoveDetal                    ((int32_t)((float)(20/8.0)*10000))
#define InboundOutboundADD_Small                  ((int32_t)((float)(5/8.8)*10000))  // 避让扫码枪的高度
#define InboundOutboundADD_Big                    ((int32_t)((float)(5/8.0)*10000))  //避让扫码枪的高度

/* 
	状态字结构体
*/
struct statusWord
{
	UNS8	readyPowerUp;											/* 0  准备上电 */
	UNS8	pwoerOn;													/* 1  已上电 */
	UNS8	enable;														/* 2  使能 */
	UNS8	fault;														/* 3  故障 */
	UNS8	prohibitionOutputVoltage;					/* 4  禁止输出电压 */
	UNS8	quickStop;												/* 5  快速停止 */
	UNS8	PowerOnProhibited;								/* 6  上电禁止 */
	UNS8	warn;															/* 7  警告 */
	UNS8	internalReservation;							/* 8  内部保留 */
	UNS8	remoteControl;										/* 9  远程控制 */
	UNS8	targetPositionReached;						/* 10 目标位置到达 */
	UNS8	internalLimitActivation;					/* 11 内部限位激活 */
	UNS8	impulseResponse;									/* 12 脉冲响应 */	
	UNS8	followingErrorOrHomeError;				/* 13 跟随误差或原点错误 */
	UNS8	motorExcitationfound;							/* 14 找到电机励磁 */
	UNS8	homeFound;												/* 15 找到原点 */
};

/* 错误字结构体 */
struct errorWord
{
	UNS8	internalError;										/* 0  内部错误报警 */
	UNS8	encoderABZConnection;							/* 1  编码器 ABZ 连接报警 */
	UNS8	encoderUVWConnection;							/* 2  编码器 UVW 连接报警 */
	UNS8	encoderCount;											/* 3  编码器计数报警 */
	UNS8	driverHighTemperature;						/* 4  驱动器高温报警 */
	UNS8	driverHighVoltage;								/* 5  驱动器高压报警 */
	UNS8	driverLowVoltage;									/* 6  驱动器低压报警 */
	UNS8	driverOvercurrent;								/* 7  驱动器过流报警 */
	UNS8	absorptionResistance;							/* 8  吸收电阻报警 */
	UNS8	ExcessivePositionError;						/* 9  位置误差过大报警 */
	UNS8	logicLowVoltage;									/* 10 逻辑低压报警 */
	UNS8	MotorOrDriveIIT;									/* 11 电机或驱动器 iit 报警 */
	UNS8	pulseFrequencyTooHigh;						/* 12 脉冲频率过高报警 */
	UNS8	motorHighTemperature;							/* 13 电机高温报警 */
	UNS8	motorExcitation;									/* 14 电机励磁报警 */
	UNS8	memory;														/* 15 存储器报警 */
};






void PowerUpMotor(UNS8 motor);
void PowerDownMotor(UNS8 motor);
void QuickStopMotor(UNS8 motor);
void ClearInternalFault(UNS8 motor);
void AbsolutePositionMode(UNS8 motor);
void RelativePositionMode(UNS8 motor);
void HomeMode(UNS8 motor);
void MotorWorkMode(UNS8 motor,UNS8 mode);
void TargetPosition(UNS8 motor, UNS32 value);
void ProfileVelocity(UNS8 motor, UNS32 value);
void canopenDataInit(void);
void Lift_BigFork_ClearStateFun(void);
void Lift_SmallFork_ClearStateFun(void);
void ScanForkFinishFLag(void);
u8 LiftMoveToBackPack_Dec(u8 Floor);
u8 LiftMoveToBin_PutBin(u8 Floor);
u8 LiftMoveToBinCaptureImage(u8 Floor);


u8 HomeMove_Lift(u8 motor);													/* 升降找原点 */
void LiftBigFork_Move(int32_t targetPosition,u32 ProfileV);					/* 大叉运动到指定位置 */
void LiftSmallFork_Move(int32_t targetPosition, u32 ProfileV);				/* 小叉运动到指定位置 */

void Scanf_Lift_message(void);															/* 扫描升降信号 */

extern u8 LiftMoveToBin(u8 Floor);													/* 大叉小叉同时运动 */
u8 LiftMoveToBackPack(u8 Floor);

extern UNS32 const bitset[32];
extern struct statusWord    bigFork_StatusWord,
														smallFork_StatusWord,
														AGVLeftStatusWord,
														AGVRightStatusWord;
extern struct errorWord 	  bigFork_ErrorWord,
														smallFork_ErrorWord,
														AGVLeft_ErrorWord,
														AGVRight_ErrorWord;


extern int32_t LiftBigFork_Floor[14];
extern int32_t LiftSmallFork_Floor[15];
extern int32_t BackPack_Floor[6];
extern int32_t BackPack_Floor_PutDown[6];
extern int32_t BackPack_BigFork_sync[6];
extern int32_t LiftBigForkCapture_Floor[9];
extern int32_t LiftSmallForkCapute_Floor[9];


#endif					/* __CANOPENCONTRAL_H */


