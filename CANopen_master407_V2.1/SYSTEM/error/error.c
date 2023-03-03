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
	SteptControlError.Rotate_LeftOut_Error = 0;																						/* ��ת������� */
	SteptControlError.Rotate_LeftBack_Error = 0;																						/* ��ת��ش��� */
	SteptControlError.Rotate_RightOut_Error = 0;																						/* ��ת�ҳ����� */
	SteptControlError.Rotate_RightBack_Error = 0;																						/* ��ת�һش��� */
	SteptControlError.Rotate_RightBehindBack_Error = 0;																			/* ��ת�Һ�ش��� */
	SteptControlError.Rotate_LeftBehindBack_Error = 0;																			/* ��ת���ش��� */
	SteptControlError.Rotate_ReturnHome_Error = 0;	
}

void ClearBigForkExtendError(void)
{
	SteptControlError.BigFork_LeftExtend_Error = 0;																						/* ���������� */
	SteptControlError.BigFork_LeftShrink_Error = 0;																						/* ������մ��� */
	SteptControlError.BigFork_RightExtend_Error = 0;																					/* ���������� */
	SteptControlError.BigFork_RighShrink_Error = 0;																						/* ������մ��� */
	SteptControlError.BigFork_ReturnHome_Error = 0;																						/* ����ԭ���� */
	SteptControlError.BigFork_Detect_object_Left_Error = 0;
	SteptControlError.BigFork_Detect_object_Right_Error = 0;
}

void ClearSmallForkExtendError(void)
{
	SteptControlError.SmallFork_LeftExtend_Error = 0;																					/* С��������� */
	SteptControlError.SmallFork_LeftShrink_Error = 0;																					/* С�����մ��� */
	SteptControlError.SmallFork_ReturnHome_Error = 0;																					/* С����ԭ���� */
	SteptControlError.SmallFork_Detect_object_Error = 0;
}


/* �����������ɨ�� */
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
		status = 3; // �����������
	}
	else if(ServoContorlError.BackPack_Servo_Error == 0xff)
	{
		status = 4; //��¨��λ����
	}
	else if(ServoContorlError.SmallFork_Servo_Error == 0xff)
	{
		status = 5; //С��ץ������
	}
	else if(ServoContorlError.BigFork_Servo_Error == 0xff)
	{
		status = 6; //���ץ������
	}
	else if ((SteptControlError.BigFork_LeftExtend_Error == 0xff) || (SteptControlError.BigFork_LeftShrink_Error == 0xff) || (SteptControlError.BigFork_RighShrink_Error == 0xff) || (SteptControlError.BigFork_RightExtend_Error == 0xff))
	{
		status = 7;  //�����������
	}
	else if ((SteptControlError.SmallFork_LeftExtend_Error == 0xff) || (SteptControlError.SmallFork_LeftShrink_Error == 0xff))
	{
		status = 8;  //С����������
	}
	else if ((SteptControlError.Rotate_LeftOut_Error == 0xff) || (SteptControlError.Rotate_RightOut_Error == 0xff) || (SteptControlError.Rotate_LeftBack_Error == 0xff) || (SteptControlError.Rotate_RightBack_Error == 0xff) || (SteptControlError.Rotate_LeftBehindBack_Error == 0xff) || (SteptControlError.Rotate_RightBehindBack_Error == 0xff))
	{
		status = 9;  //С����ת
	}
	else if ((SteptControlError.BigFork_Detect_object_Right_Error == 0xff) ||(SteptControlError.BigFork_Detect_object_Left_Error == 0xff))
	{
		status = 10; // ������������ϰ���
	}
	else if (SteptControlError.SmallFork_Detect_object_Error == 0xff)
	{
		status = 11; //С�����������ϰ���
	}
	else if (RobotFinishFlag.AllReturnHome_Finish == 0)
	{
		status = 1; // ��Ҫ��ԭ
	}
	else if((RobotFinishFlag.AllReturnHome_Finish == 0xff) && (RobotState.ControlState == 0xff))
	{
		if(RobotStepSeparate())
		{
			status = 20; //�������Ѿ�������ȡ���������ڲ������ڶ�����
		}
		else
		{
			status = 2; //��������������
		}
	}
	else if((RobotFinishFlag.AllReturnHome_Finish == 0xff) && (RobotState.ControlState == 0))
	{
		status = 0; //������������
	}
	else
	{
		status = 99; //�����˴���δ֪״̬
	}
	return status;
}



















