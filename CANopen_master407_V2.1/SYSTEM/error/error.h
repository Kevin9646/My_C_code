#ifndef __ERROR_H
#define __ERROR_H	 

#include "main.h"


struct ServoError
{
	u8	BigFork_Servo_Error; //大叉舵机故障
	u8	SmallFork_Servo_Error; //小叉舵机故障
	u8	BackPack_Servo_Error; //背篓舵机故障
};


struct SteptError
{
	u8	BigFork_LeftExtend_Error;																						/* 大叉左伸错误 */
	u8	BigFork_LeftShrink_Error;																						/* 大叉左收错误 */
	u8	BigFork_RightExtend_Error;																					/* 大叉右伸错误 */
	u8	BigFork_RighShrink_Error;																						/* 大叉右收错误 */
	u8	BigFork_ReturnHome_Error;																						/* 大叉回原错误 */
	u8  BigFork_Detect_object_Right_Error;
	u8  BigFork_Detect_object_Left_Error;
	
	u8	SmallFork_LeftExtend_Error;																					/* 小叉左伸错误 */
	u8	SmallFork_LeftShrink_Error;																					/* 小叉左收错误 */
//	u8	SmallFork_RightExtend_Error;																			/* 小叉右伸错误 */
//	u8	SmallFork_RighShrink_Error;																				/* 小叉右收错误 */
	u8	SmallFork_ReturnHome_Error;																					/* 小大叉回原错误 */
	u8  SmallFork_Detect_object_Error;
	
	u8	Rotate_LeftOut_Error;																						/* 旋转左出错误 */
	u8	Rotate_LeftBack_Error;																						/* 旋转左回错误 */
	u8	Rotate_RightOut_Error;																						/* 旋转右出错误 */
	u8	Rotate_RightBack_Error;																						/* 旋转右回错误 */
	u8	Rotate_RightBehindBack_Error;																			/* 旋转右后回错误 */
	u8	Rotate_LeftBehindBack_Error;																			/* 旋转左后回错误 */
	u8	Rotate_ReturnHome_Error;																						/* 旋转回原错误 */	
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
