#include "error.h"

struct SteptError SteptControlError;
struct ServoError ServoContorlError;


void InitError(void)
{
	memset(&SteptControlError,0,sizeof(SteptControlError));
	memset(&ServoContorlError,0,sizeof(ServoContorlError));
}

void ClearRotateError(void)
{
	SteptControlError.Rotate_LeftOut_Error = 0;																						/* 旋转左出错误 */
	SteptControlError.Rotate_LeftBack_Error = 0;																						/* 旋转左回错误 */
	SteptControlError.Rotate_RightOut_Error = 0;																						/* 旋转右出错误 */
	SteptControlError.Rotate_RightBack_Error = 0;																						/* 旋转右回错误 */
	SteptControlError.Rotate_RightBehindBack_Error = 0;																			/* 旋转右后回错误 */
	SteptControlError.Rotate_LeftBehindBack_Error = 0;																			/* 旋转左后回错误 */
	SteptControlError.Rotate_ReturnHome_Error = 0;	
}

void ClearBigForkExtendError(void)
{
	SteptControlError.BigFork_LeftExtend_Error = 0;																						/* 大叉左伸错误 */
	SteptControlError.BigFork_LeftShrink_Error = 0;																						/* 大叉左收错误 */
	SteptControlError.BigFork_RightExtend_Error = 0;																					/* 大叉右伸错误 */
	SteptControlError.BigFork_RighShrink_Error = 0;																						/* 大叉右收错误 */
	SteptControlError.BigFork_ReturnHome_Error = 0;																						/* 大叉回原错误 */
	SteptControlError.BigFork_Detect_object_Left_Error = 0;
	SteptControlError.BigFork_Detect_object_Right_Error = 0;
}

void ClearSmallForkExtendError(void)
{
	SteptControlError.SmallFork_LeftExtend_Error = 0;																					/* 小叉左伸错误 */
	SteptControlError.SmallFork_LeftShrink_Error = 0;																					/* 小叉左收错误 */
	SteptControlError.SmallFork_ReturnHome_Error = 0;																					/* 小大叉回原错误 */
	SteptControlError.SmallFork_Detect_object_Error = 0;
}


/* 步进电机错误扫描 */
void ScanStept(void)
{
	if ((SteptControlError.BigFork_LeftExtend_Error == 0xff) || (SteptControlError.BigFork_LeftShrink_Error == 0xff) || (SteptControlError.BigFork_RighShrink_Error == 0xff) 
		|| (SteptControlError.BigFork_RightExtend_Error == 0xff) || (SteptControlError.SmallFork_LeftExtend_Error == 0xff) || (SteptControlError.SmallFork_LeftShrink_Error == 0xff)
		|| (SteptControlError.Rotate_LeftOut_Error == 0xff) || (SteptControlError.Rotate_RightOut_Error == 0xff) || (SteptControlError.Rotate_LeftBack_Error == 0xff)
		|| (SteptControlError.Rotate_RightBack_Error == 0xff) || (SteptControlError.Rotate_LeftBehindBack_Error == 0xff) || (SteptControlError.Rotate_RightBehindBack_Error == 0xff)
		|| (SteptControlError.BigFork_Detect_object_Right_Error == 0xff) ||(SteptControlError.BigFork_Detect_object_Left_Error == 0xff) || (SteptControlError.SmallFork_Detect_object_Error == 0xff)
	)
	{
		memset(&RobotState,0,sizeof(RobotState)); 
		memset(&RobotFinishFlag,0,sizeof(RobotFinishFlag));
	}
}

void ScanServoError(void)
{
	if((ServoContorlError.BackPack_Servo_Error == 0xff) || (ServoContorlError.SmallFork_Servo_Error == 0xff) || (ServoContorlError.BigFork_Servo_Error == 0xff))
	{
		memset(&RobotState,0,sizeof(RobotState)); 
		memset(&RobotFinishFlag,0,sizeof(RobotFinishFlag));
	}
}

bool RobotStepSeparate(void)
{
	if((RobotState.TakeStept6 == 0xff) || (RobotState.TakeStept7 == 0xff) || (RobotState.TakeStept8 == 0xff) || (RobotState.TakeStept9 == 0xff) 
		|| (RobotState.TakeStept10 == 0xff) || (RobotState.PutStept9 == 0xff))
	{
		return true;
	}
	else
	{
		return false;
	}
}


u16 LiftStatus(void)
{
	u16 status = 0;
	if(RobotState.motorerror == 0xff)
	{
		status = 3; // 升降电机故障
	}
	else if(ServoContorlError.BackPack_Servo_Error == 0xff)
	{
		status = 4; //背篓限位故障
	}
	else if(ServoContorlError.SmallFork_Servo_Error == 0xff)
	{
		status = 5; //小叉抓钩故障
	}
	else if(ServoContorlError.BigFork_Servo_Error == 0xff)
	{
		status = 6; //大叉抓钩故障
	}
	else if ((SteptControlError.BigFork_LeftExtend_Error == 0xff) || (SteptControlError.BigFork_LeftShrink_Error == 0xff) || (SteptControlError.BigFork_RighShrink_Error == 0xff) || (SteptControlError.BigFork_RightExtend_Error == 0xff))
	{
		status = 7;  //大叉伸缩故障
	}
	else if ((SteptControlError.SmallFork_LeftExtend_Error == 0xff) || (SteptControlError.SmallFork_LeftShrink_Error == 0xff))
	{
		status = 8;  //小叉伸缩故障
	}
	else if ((SteptControlError.Rotate_LeftOut_Error == 0xff) || (SteptControlError.Rotate_RightOut_Error == 0xff) || (SteptControlError.Rotate_LeftBack_Error == 0xff) || (SteptControlError.Rotate_RightBack_Error == 0xff) || (SteptControlError.Rotate_LeftBehindBack_Error == 0xff) || (SteptControlError.Rotate_RightBehindBack_Error == 0xff))
	{
		status = 9;  //小叉旋转
	}
	else if ((SteptControlError.BigFork_Detect_object_Right_Error == 0xff) ||(SteptControlError.BigFork_Detect_object_Left_Error == 0xff))
	{
		status = 10; // 大叉伸缩碰到障碍物
	}
	else if (SteptControlError.SmallFork_Detect_object_Error == 0xff)
	{
		status = 11; //小叉伸缩碰到障碍物
	}
	else if (RobotFinishFlag.AllReturnHome_Finish == 0)
	{
		status = 1; // 需要回原
	}
	else if((RobotFinishFlag.AllReturnHome_Finish == 0xff) && (RobotState.ControlState == 0xff))
	{
		if(RobotStepSeparate())
		{
			status = 20; //机器人已经将料箱取到机器人内部，用于动作拆开
		}
		else
		{
			status = 2; //机器人正在运行
		}
	}
	else if((RobotFinishFlag.AllReturnHome_Finish == 0xff) && (RobotState.ControlState == 0))
	{
		status = 0; //升降机构空闲
	}
	else
	{
		status = 99; //机器人处于未知状态
	}
	return status;
}



















