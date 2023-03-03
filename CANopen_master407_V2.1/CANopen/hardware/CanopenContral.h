#ifndef __CANOPENCONTRAL_H
#define	__CANOPENCONTRAL_H
#include "main.h"

/*���-------------motor ����*/
#define BigFork     		1										/* ��� */
#define SmallFork   		2										/* С�� */



/* ����ģʽ -----mode���� */

#define velocityMode   						3																				/* �ٶ�ģʽ */
#define positionMode   						1																				/* λ��ģʽ */
#define homeMode     	 						6																				/* ��ԭ��ģʽ */

/* λ�ø������� */
#define FloatingWindow						500																				/* λ�ø������� */


/* 
	�����ٶ�
	�������ٶ� 0.33m/s
	С������ٶ� 0.36m/s
	
	300/7.8932 = 38.007  Ϊ���ÿ��Ȧ��
	300/9.5139 = 31.532	ΪС��ÿ��Ȧ��
	
	100/7.8932 = 12.669  Ϊ���ÿ��Ȧ��
	100/9.5139 = 10.511	ΪС��ÿ��Ȧ��
	
	300/8.0 = 37.5  Ϊ���ÿ��Ȧ��
	300/8.8 = 34.09	ΪС��ÿ��Ȧ��
	
	100/8.0 = 12.5  Ϊ���ÿ��Ȧ��
	100/8.8 = 11.36	ΪС��ÿ��Ȧ��
*/

#define LiftBigForkProfile_V_Long			((u32)((37.5 * 60 * 512 * 10000) / 1875))								/* ������������ٶ� */					/* ���س������ٶ� */
#define LiftSmallForkProfile_V_Long		((u32)((34.09 * 60 * 512 * 10000) / 1875))								/* С�����������ٶ� */

//#define LiftBigForkProfile_V_Long			((u32)((38.007 * 60 * 512 * 10000) / 1875))								/* ������������ٶ� */					/* ���س������ٶ� */
//#define LiftSmallForkProfile_V_Long		((u32)((31.532 * 60 * 512 * 10000) / 1875))								/* С�����������ٶ� */

//#define LiftBigForkProfile_V_Long			((u32)((12.669 * 60 * 512 * 10000) / 1875))								/* ������������ٶ� */					/* ���س������ٶ� */
//#define LiftSmallForkProfile_V_Long		((u32)((10.510 * 60 * 512 * 10000) / 1875))								/* С�����������ٶ� */


#define LiftBigForkProfile_V_short			((u32)((12.5 * 60 * 512 * 10000) / 1875))								/* ������������ٶ� */				/* ���ض̾����ٶ� */
#define LiftSmallForkProfile_V_short		((u32)((11.36 * 60 * 512 * 10000) / 1875))								/* С�����������ٶ� */


//#define LiftBigForkProfile_V_short			((u32)((12.669 * 60 * 512 * 10000) / 1875))								/* ������������ٶ� */				/* ���ض̾����ٶ� */
//#define LiftSmallForkProfile_V_short		((u32)((10.510 * 60 * 512 * 10000) / 1875))								/* С�����������ٶ� */

//#define LiftBigForkProfile_V_short			((u32)((6.3 * 60 * 512 * 10000) / 1875))								/* ������������ٶ� */				/* ���ض̾����ٶ� */
//#define LiftSmallForkProfile_V_short		((u32)((10.510 * 60 * 512 * 10000) / 1875))								/* С�����������ٶ� */



/* 
		һȦ��Ӧ�ľ��� ��λmm 
		�����ת1Ȧ������7.893251408mm
		С����ת1Ȧ������9.51399903mm
*/
#define	LiftBigFork_1RadToMM						80000									/* ����10000�� 78932*/
#define	LiftSmallFork_1RadToMM					88000									/* ����10000�� 95139*/


/* ��С����� */
#define	Bin_Distance										245																/* ��С���������������� */
#define	BackPack_Distance								426																/* ��¨���������� */


#define	BigFork_MaxPosition								1084												/* ����ܹ���������λ��*/	
#define	BigFork_MinPosition								(-735)												/* ����ܹ���������λ�� */
#define	BigFork_MinBackPosition						(-390)												/* ����ܹ���������λ�� */
#define	BigFork_Position_When_SmallFork_LowSpeed							(-190)		/* С��Я������������е���͵�ʱ�����ɵ�������λ�� */
#define	SmallFork_MaxPosition							1356												/* С���ܹ���������λ�� */	
#define	SmallFork_MinPosition							(-439)												/* С���ܹ���������λ�� */	
#define	SmallFork_MinBackPosition					(-390)												
#define	SmallFork_MinPosition_HighSpeed_HaveTote							(-234)		/* С��Я�������������ʱ�ɵ�������λ�� */
#define SmallFork_MaxPositon_Collisiion_BigFork               1060      /* ������������λ��ʱ��С�治�����������λ��*/

#define SmallFork_InboundPosition        210	//230
#define BigFork_InboundPositin           450
#define SmallFork_OutboundPosition       230
#define BigFork_OutboundPosition         450

/* ���С��ȡ��ж��ʱ̧������ */
#define AGVHigh_Small															((int32_t)((float)(15/8.8)*10000))
#define AGVHigh_Big																((int32_t)((float)(120/8.0)*10000))	
#define LiftBigForkADD														((int32_t)((float)(160/8.0)*10000))
#define LiftSmallForkADD													((int32_t)((float)(160/8.8)*10000))
#define LiftSmallForkBackPackADD									((int32_t)((float)(78/8.8)*10000))  //δ�õ���ʱ
#define LiftBigForkDec														((int32_t)((float)(120/8.0)*10000))
#define LiftSmallForkDec													((int32_t)((float)(85/8.8)*10000))
#define LiftSmallForkBackPackDec									((int32_t)((float)(95/8.8)*10000))
#define LiftSmallForkMoveDetal                    ((int32_t)((float)(20/8.8)*10000))
#define LiftBigforkrkMoveDetal                    ((int32_t)((float)(20/8.0)*10000))
#define InboundOutboundADD_Small                  ((int32_t)((float)(5/8.8)*10000))  // ����ɨ��ǹ�ĸ߶�
#define InboundOutboundADD_Big                    ((int32_t)((float)(5/8.0)*10000))  //����ɨ��ǹ�ĸ߶�

/* 
	״̬�ֽṹ��
*/
struct statusWord
{
	UNS8	readyPowerUp;											/* 0  ׼���ϵ� */
	UNS8	pwoerOn;													/* 1  ���ϵ� */
	UNS8	enable;														/* 2  ʹ�� */
	UNS8	fault;														/* 3  ���� */
	UNS8	prohibitionOutputVoltage;					/* 4  ��ֹ�����ѹ */
	UNS8	quickStop;												/* 5  ����ֹͣ */
	UNS8	PowerOnProhibited;								/* 6  �ϵ��ֹ */
	UNS8	warn;															/* 7  ���� */
	UNS8	internalReservation;							/* 8  �ڲ����� */
	UNS8	remoteControl;										/* 9  Զ�̿��� */
	UNS8	targetPositionReached;						/* 10 Ŀ��λ�õ��� */
	UNS8	internalLimitActivation;					/* 11 �ڲ���λ���� */
	UNS8	impulseResponse;									/* 12 ������Ӧ */	
	UNS8	followingErrorOrHomeError;				/* 13 ��������ԭ����� */
	UNS8	motorExcitationfound;							/* 14 �ҵ�������� */
	UNS8	homeFound;												/* 15 �ҵ�ԭ�� */
};

/* �����ֽṹ�� */
struct errorWord
{
	UNS8	internalError;										/* 0  �ڲ����󱨾� */
	UNS8	encoderABZConnection;							/* 1  ������ ABZ ���ӱ��� */
	UNS8	encoderUVWConnection;							/* 2  ������ UVW ���ӱ��� */
	UNS8	encoderCount;											/* 3  �������������� */
	UNS8	driverHighTemperature;						/* 4  ���������±��� */
	UNS8	driverHighVoltage;								/* 5  ��������ѹ���� */
	UNS8	driverLowVoltage;									/* 6  ��������ѹ���� */
	UNS8	driverOvercurrent;								/* 7  �������������� */
	UNS8	absorptionResistance;							/* 8  ���յ��豨�� */
	UNS8	ExcessivePositionError;						/* 9  λ�������󱨾� */
	UNS8	logicLowVoltage;									/* 10 �߼���ѹ���� */
	UNS8	MotorOrDriveIIT;									/* 11 ����������� iit ���� */
	UNS8	pulseFrequencyTooHigh;						/* 12 ����Ƶ�ʹ��߱��� */
	UNS8	motorHighTemperature;							/* 13 ������±��� */
	UNS8	motorExcitation;									/* 14 ������ű��� */
	UNS8	memory;														/* 15 �洢������ */
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


u8 HomeMove_Lift(u8 motor);													/* ������ԭ�� */
void LiftBigFork_Move(int32_t targetPosition,u32 ProfileV);					/* ����˶���ָ��λ�� */
void LiftSmallFork_Move(int32_t targetPosition, u32 ProfileV);				/* С���˶���ָ��λ�� */

void Scanf_Lift_message(void);															/* ɨ�������ź� */

extern u8 LiftMoveToBin(u8 Floor);													/* ���С��ͬʱ�˶� */
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


