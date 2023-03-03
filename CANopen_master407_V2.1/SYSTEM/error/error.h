#ifndef __ERROR_H
#define __ERROR_H	 

#include "main.h"


struct ServoError
{
	u8	BigFork_Servo_Error; //���������
	u8	SmallFork_Servo_Error; //С��������
	u8	BackPack_Servo_Error; //��¨�������
};


struct SteptError
{
	u8	BigFork_LeftExtend_Error;																						/* ���������� */
	u8	BigFork_LeftShrink_Error;																						/* ������մ��� */
	u8	BigFork_RightExtend_Error;																					/* ���������� */
	u8	BigFork_RighShrink_Error;																						/* ������մ��� */
	u8	BigFork_ReturnHome_Error;																						/* ����ԭ���� */
	u8  BigFork_Detect_object_Right_Error;
	u8  BigFork_Detect_object_Left_Error;
	
	u8	SmallFork_LeftExtend_Error;																					/* С��������� */
	u8	SmallFork_LeftShrink_Error;																					/* С�����մ��� */
//	u8	SmallFork_RightExtend_Error;																			/* С��������� */
//	u8	SmallFork_RighShrink_Error;																				/* С�����մ��� */
	u8	SmallFork_ReturnHome_Error;																					/* С����ԭ���� */
	u8  SmallFork_Detect_object_Error;
	
	u8	Rotate_LeftOut_Error;																						/* ��ת������� */
	u8	Rotate_LeftBack_Error;																						/* ��ת��ش��� */
	u8	Rotate_RightOut_Error;																						/* ��ת�ҳ����� */
	u8	Rotate_RightBack_Error;																						/* ��ת�һش��� */
	u8	Rotate_RightBehindBack_Error;																			/* ��ת�Һ�ش��� */
	u8	Rotate_LeftBehindBack_Error;																			/* ��ת���ش��� */
	u8	Rotate_ReturnHome_Error;																						/* ��ת��ԭ���� */	
};
extern struct SteptError SteptControlError;
extern struct ServoError ServoContorlError;

void InitError(void);
void ScanServoError(void);
void ScanStept(void);
u16 LiftStatus(void);
void ClearSmallForkExtendError(void);
void ClearBigForkExtendError(void);
void ClearRotateError(void);



































#endif																					/* __ERROR_H */
