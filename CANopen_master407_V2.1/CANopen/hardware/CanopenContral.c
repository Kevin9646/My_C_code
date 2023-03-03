#include "CanopenContral.h"



int32_t LiftBigFork_Floor[14] = {0};						/* 9���ϼ���λ��    10���¼���λ��  11: ����ջ���Сλ��*/
int32_t LiftSmallFork_Floor[15] = {0};
int32_t BackPack_Floor[6] = {0};								
int32_t BackPack_Floor_PutDown[6] = {0};
int32_t BackPack_BigFork_sync[6] = {0};
int32_t LiftBigForkCapture_Floor[9] = {0};
int32_t LiftSmallForkCapute_Floor[9] = {0};

u8 BigForkHome_Flag = 0;

/*
	ȡλ����
*/
UNS32 const bitset[32]={										
  0x1,0x2,0x4,0x8,
  0x10,0x20,0x40,0x80,
  0x100,0x200,0x400,0x800,
  0x1000,0x2000,0x4000,0x8000,
  0x10000,0x20000,0x40000,0x80000,
  0x100000,0x200000,0x400000,0x800000,
  0x1000000,0x2000000,0x4000000,0x8000000,
  0x10000000,0x20000000,0x40000000,0x80000000,
};

struct statusWord   bigFork_StatusWord,
										smallFork_StatusWord,
										AGVLeftStatusWord,
										AGVRightStatusWord;

struct errorWord 	  bigFork_ErrorWord,
									  smallFork_ErrorWord,
									  AGVLeft_ErrorWord,
									  AGVRight_ErrorWord;

/* 
	����ϵ�
	motor:
				BigFork
				SmallFork
				AGV��
				AGV��
*/
void PowerUpMotor(UNS8 motor)
{
	switch(motor)
	{
		case 1:														/* ��� */
			controlWord_6040[0] = 0x0F;

			break;
		case 2:														/* С�� */
			controlWord_6040[1] = 0x0F;

			break;	

	}
}

/* 
	����ϵ�
	motor:
				BigFork
				SmallFork
				AGV��
				AGV��
*/

void PowerDownMotor(UNS8 motor)
{
	switch(motor)
	{
		case 1:														/* ��� */
			controlWord_6040[0] = 0x06;

			break;
		case 2:														/* С�� */
			controlWord_6040[1] = 0x06;

			break;	


	}
}

/* 
	����ֹͣ
	motor:
				BigFork
				SmallFork
				AGV��
				AGV��
*/

void QuickStopMotor(UNS8 motor)
{
	switch(motor)
	{
		case 1:														/* ��� */
			controlWord_6040[0] = 0x0B;

			break;
		case 2:														/* С�� */
			controlWord_6040[1] = 0x0B;

			break;	

	}
}

/* 
	����ڲ�����
	motor:
				BigFork
				SmallFork
				AGV��
				AGV��
*/
void ClearInternalFault(UNS8 motor)
{
	switch(motor)
	{
		case 1:														/* ��� */
			controlWord_6040[0] = 0x86;

			break;
		case 2:														/* С�� */
			controlWord_6040[1] = 0x86;

			break;	
	}
}


/*
	����λ��ģʽ------�����ԭ��
	motor:
				BigFork
				SmallFork
				AGV��
				AGV��
*/

void AbsolutePositionMode(UNS8 motor)
{
	switch(motor)
	{
		case 1:														/* ��� */
			controlWord_6040[0] = 0x3F;
			break;
		case 2:														/* С�� */
			controlWord_6040[1] = 0x3F;
			break;	
	}
}

/*
	���λ��ģʽ------�������һ��ֹͣλ��
	motor:
				BigFork
				SmallFork
				AGV��
				AGV��
*/
void RelativePositionMode(UNS8 motor)
{
	switch(motor)
	{
		case 1:														/* ��� */
			controlWord_6040[0] = 0x5F;
			break;
		case 2:														/* С�� */
			controlWord_6040[1] = 0x5F;

			break;	

	}
}

/*
		ԭ��ģʽ
	motor:
				BigFork
				SmallFork
				AGV��
				AGV��
*/


void HomeMode(UNS8 motor)
{
	switch(motor)
	{
		case 1:														/* ��� */
			controlWord_6040[0] = 0x1F;

			break;
		case 2:														/* С�� */
			controlWord_6040[1] = 0x1F;

			break;	
	}
}

/*
	����ģʽ����
		motor:
				BigFork
				SmallFork
				AGV��
				AGV��
		mode:
				velocityMode
				positionMode
				homeMode
				
*/
void MotorWorkMode(UNS8 motor,UNS8 mode)
{
	switch(motor)
	{
		case 1:														/* ��� */
			modeOfOperation_6060[0] = mode;

			break;
		case 2:														/* С�� */
			modeOfOperation_6060[1] = mode;

			break;	
	}
}


/*
		Ŀ��λ��
		motor:
				BigFork
				SmallFork
				AGV��
				AGV��
		value:
			λ����ֵ
*/
void TargetPosition(UNS8 motor, UNS32 value)
{
	switch(motor)
	{
		case 1:														/* ��� */
			targetPosition_607A[0] = value;
			break;
		case 2:														/* С�� */
			targetPosition_607A[1] = value;

			break;	
	}
}

/*
		�����ٶ�
		motor:
				BigFork
				SmallFork
				AGV��
				AGV��
		position:
			λ����ֵ
*/
void ProfileVelocity(UNS8 motor, UNS32 value)
{
	switch(motor)
	{
		case 1:														/* ��� */
			profileVelocity_6081[0] = value;
			break;
		case 2:														/* С�� */
			profileVelocity_6081[1] = value;
			break;	
	}
}



u8 HomeMove_Lift(u8 motor)																				/* ������ԭ�� */
{
	if(motor == BigFork)
	{
		if(RobotFinishFlag.ReturnHome_State_finish[3] == 0xff)
		{
			RobotFinishFlag.ReturnHome_State_finish[3] = 0;							/* �����ԭ��ɱ�־ */
		}
		
		
		RobotState.ReturnHome_State[3] = 0xff;											/* �������ԭ״̬ */
		modeOfOperation_6060[0] = 0x06;
		controlWord_6040[0] = 0x0f;
		HomeTIM_BigFork(0) = -1;
	}
	if(motor == SmallFork)
	{
		if(RobotFinishFlag.ReturnHome_State_finish[4] == 0xff)
		{
			RobotFinishFlag.ReturnHome_State_finish[4] = 0;							/* �����ԭ��ɱ�־ */
		}
		RobotState.ReturnHome_State[4] = 0xff;												/* ����С���ԭ״̬ */
		modeOfOperation_6060[1] = 0x06;
		controlWord_6040[1] = 0x0f;
		HomeTIM_SmallFork(0) = -1;
	}
	
	return 0;
}


void Scanf_Lift_message(void)
{
	if(RobotState.ReturnHome_State[3] == 0xff)				/* ���ԭ��ģʽ */
	{
		if(((Sensor_Din[0]&0x01) == 0x00) && (BigForkHome_Flag == 0)) // ����ԭ�ķ�ʽ��Ҫ���⴦���жϡ�
		{
			BigForkHome_Flag = 1;
		}
		if((Sensor_Din[0] == 0x01) && (BigForkHome_Flag ==1))													/* ���ԭ�㴥���źŲɼ� */
		{
			if((positionActualValue_6063[0] >= -FloatingWindow) && (positionActualValue_6063[0] <= FloatingWindow))
			{
				RobotFinishFlag.ReturnHome_State_finish[3] = 0xff;			/* ��ԭ��ɱ�� */
				RobotState.ReturnHome_State[3] = 0;											/* �����ԭ״̬��־ */
				BigForkHome_Flag = 0;
			}
		}
	}	
	if(RobotState.ReturnHome_State[4] == 0xff)				/* С��ԭ��ģʽ */
	{
		if(Sensor_Din[1] == 0x01)															/* С��ԭ�㴥���źŲɼ� */
		{
			if((positionActualValue_6063[1] >= -FloatingWindow) && (positionActualValue_6063[1] <= FloatingWindow))
			{
				RobotFinishFlag.ReturnHome_State_finish[4] = 0xff;			/* ��ԭ��ɱ�� */
				RobotState.ReturnHome_State[4] = 0;											/* �����ԭ״̬��־ */
//				controlWord_6040[1] = 0x0f;
			}
		}
	}
}

void LiftBigFork_Move(int32_t targetPosition, u32 ProfileV)					/* ����˶���ָ��λ�� */
{

	if(RobotFinishFlag.LiftBigFork_PositionArrive == 0xff)						/* ����ǰ��������־ */
	{
		RobotFinishFlag.LiftBigFork_PositionArrive = 0;
	}
		RobotState.LiftBigFork_Move_State = 0xff;
	
	modeOfOperation_6060[0] = 0x01;
	targetPosition_607A[0] = targetPosition;
	
	profileVelocity_6081[0] = ProfileV;
	controlWord_6040[0] = 0x0f;
	BigForkTIM(0) = -1;
}


void LiftSmallFork_Move(int32_t targetPosition, u32 ProfileV)				/* С���˶���ָ��λ�� */
{
	
	if(RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff)					/* ����ǰ��������־ */
	{
		RobotFinishFlag.LiftSmallFork_PositionArrive = 0;
	}
	RobotState.LiftSmallFork_move_State = 0xff;
	
	
	modeOfOperation_6060[1] = 0x01;
	targetPosition_607A[1] = targetPosition;

	profileVelocity_6081[1] = ProfileV;
	controlWord_6040[1] = 0x0f;
	SmallForkTIM(0) = -1;
}


u8 LiftMoveToBinCaptureImage(u8 Floor)
{
	if(Floor == 50)
	{
		if(positionActualValue_6063[0] < LiftBigForkCapture_Floor[8])
		{
			LiftBigFork_Move(LiftBigForkCapture_Floor[8],LiftBigForkProfile_V_Long);
		}
		else
		{
			RobotFinishFlag.LiftBigFork_PositionArrive = 0xff;
		}
		LiftSmallFork_Move(LiftSmallForkCapute_Floor[8],LiftSmallForkProfile_V_Long);
	}
	else
	{
		LiftBigFork_Move(LiftBigForkCapture_Floor[Floor-1],LiftBigForkProfile_V_Long);
		LiftSmallFork_Move(LiftSmallForkCapute_Floor[Floor-1],LiftSmallForkProfile_V_Long);
	}
	return 0;
}


/* ���С��ͬʱ�˶���ָ��ȡ����λ�� */
u8 LiftMoveToBin(u8 Floor)
{
	if (Floor == 1)
	{
		LiftBigFork_Move(LiftBigFork_Floor[10],LiftBigForkProfile_V_Long);						/* ����˶���������  ����˶��������������һ��λ�� */
		LiftSmallFork_Move(LiftSmallFork_Floor[10],LiftSmallForkProfile_V_Long);	
	}
	else if (Floor == 8)
	{
		LiftBigFork_Move(LiftBigFork_Floor[8],LiftBigForkProfile_V_Long);						/* ����˶�����ߵ�*/
		LiftSmallFork_Move(LiftSmallFork_Floor[12],LiftSmallForkProfile_V_Long);	// С����Ҫ�ȵ�������ײ��λ�ã�
	}
	else if (Floor == 50) //����ⲿ��ֻ��ҪС�湤��
	{
		if(positionActualValue_6063[0] < (LiftBigFork_Floor[12]+ LiftBigForkADD + InboundOutboundADD_Big))
		{
			LiftBigFork_Move(LiftBigFork_Floor[12]+ LiftBigForkADD + InboundOutboundADD_Big,LiftBigForkProfile_V_Long);
		}
		else
		{
			RobotFinishFlag.LiftBigFork_PositionArrive = 0xff;
		}
		LiftSmallFork_Move(LiftSmallFork_Floor[13],LiftSmallForkProfile_V_Long);
	}
	else
	{
		LiftBigFork_Move(LiftBigFork_Floor[Floor],LiftBigForkProfile_V_Long);						/* ����˶���������  ����˶��������������һ��λ�� */
		LiftSmallFork_Move(LiftSmallFork_Floor[Floor-1],LiftSmallForkProfile_V_Long);	
	}
	return 0;
}



/* ��С�浽ָ��λ��ж������浽Ŀ������λ�ã�С�浽 */
u8 LiftMoveToBin_PutBin(u8 Floor)
{
	if(Floor == 1)
	{
		LiftBigFork_Move(LiftBigFork_Floor[Floor] + LiftBigForkADD + AGVHigh_Big,LiftBigForkProfile_V_Long);						/* �������Ҳ�����˶���һ��λ�� */
		LiftSmallFork_Move(LiftSmallFork_Floor[Floor - 1] + LiftSmallForkADD + AGVHigh_Small,LiftSmallForkProfile_V_Long);					/* С������˶���һ��λ�� */
	}
	else if (Floor == 8 || Floor == 7)
	{
		LiftBigFork_Move(LiftBigFork_Floor[8],LiftBigForkProfile_V_Long);						/* ����˶�����ߵ�*/
		LiftSmallFork_Move(LiftSmallFork_Floor[12],LiftSmallForkProfile_V_Long);	 /*���ٵ�������ײ��λ��*/
	}
	else if (Floor == 50)
	{
		if(positionActualValue_6063[0] < (LiftBigFork_Floor[13] + LiftBigForkADD + InboundOutboundADD_Big))
		{
			LiftBigFork_Move(LiftBigFork_Floor[13] + LiftBigForkADD + InboundOutboundADD_Big,LiftBigForkProfile_V_Long);
		}
		else
		{
			RobotFinishFlag.LiftBigFork_PositionArrive = 0xff;
		}
		LiftSmallFork_Move(LiftSmallFork_Floor[14]+ LiftSmallForkADD + InboundOutboundADD_Small,LiftSmallForkProfile_V_Long);	
	}
	else
	{
		LiftBigFork_Move(LiftBigFork_Floor[Floor] + LiftBigForkADD,LiftBigForkProfile_V_Long);						/* ����˶���������  ����˶��������������һ��λ�� */
		LiftSmallFork_Move(LiftSmallFork_Floor[Floor-1] + LiftSmallForkADD,LiftSmallForkProfile_V_Long);	
	}
	return 0;
}


/* ��С�浽ָ��λ��ж������浽Ŀ������λ�ã�С�浽 */
u8 LiftMoveToBin_PutBin_backup(u8 Floor)
{
	if((Floor == 1) || (Floor == 2))
	{
		LiftBigFork_Move(LiftBigFork_Floor[11],LiftBigForkProfile_V_Long);						/* �������Ҳ�����˶���һ��λ�� */
		LiftSmallFork_Move(LiftSmallFork_Floor[11],LiftSmallForkProfile_V_Long);					/* С������˶���һ��λ�� */
	}
	else if (Floor == 8)
	{
		LiftBigFork_Move(LiftBigFork_Floor[8],LiftBigForkProfile_V_Long);						/* ����˶�����ߵ�*/
		LiftSmallFork_Move(LiftSmallFork_Floor[12],LiftSmallForkProfile_V_Long);	 /*���ٵ�������ײ��λ��*/
	}
	else if (Floor == 50)
	{
		LiftBigFork_Move(LiftBigFork_Floor[13],LiftBigForkProfile_V_Long);	
		LiftSmallFork_Move(LiftSmallFork_Floor[14],LiftSmallForkProfile_V_Long);	
	}
	else
	{
		LiftBigFork_Move(LiftBigFork_Floor[Floor-1] ,LiftBigForkProfile_V_Long);						/* ����˶���������  ����˶��������������һ��λ�� */
		LiftSmallFork_Move(LiftSmallFork_Floor[Floor-2] ,LiftSmallForkProfile_V_Long);	
	}
	return 0;
}



/* ��С���˶�����¨ָ��λ��-------ȡ�����õ���¨ */
u8 LiftMoveToBackPack(u8 Floor)
{
	if(positionActualValue_6063[0] < BackPack_BigFork_sync[Floor -1])
	{
		LiftBigFork_Move(BackPack_BigFork_sync[Floor -1],LiftBigForkProfile_V_Long);						/* ��浽��һ�㱳¨λ�� */
	}
	else
	{
		RobotFinishFlag.LiftBigFork_PositionArrive = 0xff;
	}
	if((Floor ==5) || (Floor == 6))    /*��5��6��С���ײ����棬�����ܴ���Ҫ���⴦��*/
	{
		LiftSmallFork_Move(LiftSmallFork_Floor[12],LiftSmallForkProfile_V_Long);			/* С��߶����е�����ײ�����ĸ߶� */
	}
	else
	{
		LiftSmallFork_Move(BackPack_Floor[Floor-1],LiftSmallForkProfile_V_Long);			/* С�浽��¨λ�� */
	}
	return 0;
}

/* ��С���˶�����¨ָ��λ��-----ȡ��¨��������õ���λ*/
u8 LiftMoveToBackPack_Dec(u8 Floor)
{
	if(positionActualValue_6063[0] < BackPack_BigFork_sync[Floor -1])
	{
		LiftBigFork_Move(BackPack_BigFork_sync[Floor -1],LiftBigForkProfile_V_Long);						/* ��浽��һ�㱳¨λ�� */
	}
	else
	{
		RobotFinishFlag.LiftBigFork_PositionArrive = 0xff;
	}
	if (Floor == 3)      /* С�����˸��� ��Ҫ����4��λ�ý���ȡ�� */
	{
		LiftSmallFork_Move(BackPack_Floor_PutDown[Floor], LiftSmallForkProfile_V_Long);			
	}
	else if((Floor ==5) || (Floor == 6)) /* С���������ײ����棬�����ȸ��ٵ�����ײ����λ�� */
	{
		LiftSmallFork_Move(LiftSmallFork_Floor[12],LiftSmallForkProfile_V_Long);			/* С��߶����е�����ײ�����ĸ߶� */
	}
	else
	{
		LiftSmallFork_Move(BackPack_Floor_PutDown[Floor-1],LiftSmallForkProfile_V_Long);			/* С�浽��¨λ�� */
	}
	return 0;
}



void canopenDataInit(void)
{
	/* ��ʼ��CANOPEN���� */
	unsigned char nodeID = 0x00;                   //�ڵ�ID
  setNodeId(&Master_Data, nodeID);
  setState(&Master_Data, Initialisation);				//�ڵ��ʼ��
	setState(&Master_Data, Pre_operational);	
	setState(&Master_Data, Operational);
	
//	stopSYNC(&Master_Data);

	delay_ms(10);

	
//	/* ʹ�ܴӻ� */
	masterSendNMTstateChange(&Master_Data, 1, NMT_Start_Node);
	delay_ms(10);
	masterSendNMTstateChange(&Master_Data, 2, NMT_Start_Node);
	delay_ms(10);
	
	
	
	 controlWord_6040[0] = 0x86;
	 controlWord_6040[1] = 0x86;

	 modeOfOperation_Init(0) = -1;	
	
//	modeOfOperation_6060[0] = 0x01;
//	modeOfOperation_6060[1] = 0x01;
//	
//	controlWord_6040[0]  = 0x0f;
//	controlWord_6040[1]  = 0x0f;	
//	delay_ms(10);
//	startSYNC(&Master_Data);
}

/* �������״̬��� */
void Lift_BigFork_ClearStateFun(void)
{
	if(Sensor_Din[0] == 0x02)
	{
		if(BigLiftLimit_Up == 0) //�������
		{
			if((positionActualValue_6063[0] >= (targetPosition_607A[0]- 8*FloatingWindow)) && (positionActualValue_6063[0] < (targetPosition_607A[0] + 8*FloatingWindow)))  // �ϼ��޷Ŵ�λ����
			{
				RobotFinishFlag.LiftBigFork_PositionArrive = 0xff;
				RobotState.LiftBigFork_Move_State = 0;
			}
		}
	}
	else
	{
		if((positionActualValue_6063[0] >= targetPosition_607A[0]-FloatingWindow) && (positionActualValue_6063[0] < targetPosition_607A[0] + FloatingWindow))
		{
			RobotFinishFlag.LiftBigFork_PositionArrive = 0xff;
			RobotState.LiftBigFork_Move_State = 0;
		}
	}
}

/* С������״̬��� */
void Lift_SmallFork_ClearStateFun(void)
{
	if(Sensor_Din[1] == 0x04)
	{
		if(SmallLiftLimit_Down == 0) //�������
		{
			if((positionActualValue_6063[1] >= (targetPosition_607A[1]- 8*FloatingWindow)) && (positionActualValue_6063[1] < (targetPosition_607A[1] + 8*FloatingWindow)))  // �ϼ��޷Ŵ�λ����
			{
				RobotFinishFlag.LiftSmallFork_PositionArrive = 0xff;
				RobotState.LiftSmallFork_move_State = 0;
			}
		}
	}
	else
	{
		if((positionActualValue_6063[1] >= targetPosition_607A[1]-FloatingWindow) && (positionActualValue_6063[1] < targetPosition_607A[1] + FloatingWindow))
		{
			RobotFinishFlag.LiftSmallFork_PositionArrive = 0xff;
			RobotState.LiftSmallFork_move_State = 0;
		}
	}
}


void ScanForkFinishFLag(void)
{
	if(RobotState.LiftBigFork_Move_State == 0xff )
	{
		Lift_BigFork_ClearStateFun();
	}	

	if(RobotState.LiftSmallFork_move_State == 0xff )
	{
		Lift_SmallFork_ClearStateFun();
	}	
}





void	ScanFirstFloorControl(void)
{
	if((RobotFinishFlag.AllReturnHome_Finish == 0xff) && (RobotState.ControlState == 0xff))			/* �ѻ�ԭ�㣬�����յ�������С��ץ��ָ�� */
	{
		if(LiftAllAction.TakePut == 0xff)							/* ȡ����״̬ */
		{	
			/* -----------------------------��1��start--------------------------------------------- */
			if(RobotState.TakeStept1 == 0xff)
			{
				if((RobotFinishFlag.LiftBigFork_PositionArrive == 0xff) && (RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff))
				{
					RobotState.TakeStept1 = 0;
					RobotState.TakeStept2 = 0xff;
					RobotFinishFlag.LiftBigFork_PositionArrive = 0;
					RobotFinishFlag.LiftSmallFork_PositionArrive = 0;					
					
					/* ��������������־λ����־����������һ���ĵ�һ������ */
					RobotState.NextSteptStart = 0xff;
				}
			}
			
			
/* ------------------------------------------------------��2��start----------------------------------------------------------------- */
			if(RobotState.TakeStept2 == 0xff)
			{
				if(RobotState.GrabDir == 0xff)																													/* ȡ������� */
				{
					if(RobotState.NextSteptStart == 0xff)
					{
						Rotate_Left_Out();																			/* С����ת ������	*/
						BigFork_Extend_Left();																	/* ���������� */
						RobotState.NextSteptStart = 0;
					}
	
					if(RobotFinishFlag.Rotate_out_left_finish == 0xff)
					{
						RobotFinishFlag.Rotate_out_left_finish = 0;
						SmallFork_Extend_Left();															/* С��������� */
					}				
					
					if(RobotFinishFlag.BigFork_Extend_left_finish == 0xff)
					{
						ServoMove_Bigfork12_L_Out();
						RobotFinishFlag.BigFork_Extend_left_finish = 0;
					}
					
					if(RobotFinishFlag.SmallFork_Extend_finish == 0xff)
					{
						ServoMove_Smallfork56_Out();
						RobotFinishFlag.SmallFork_Extend_finish = 0;
					}
					
					
					if((RobotFinishFlag.BigForkSGear_LeftOut12_finish == 0xff) &&(RobotFinishFlag.SmallForkSGear_Out56_finish == 0xff))
					{
						/* ����AGV���̸߶� */
						LiftBigFork_Move(positionActualValue_6063[0] + LiftBigForkADD + AGVHigh_Big,LiftBigForkProfile_V_short);		/* ���̧�� */
						
						RobotFinishFlag.BigForkSGear_LeftOut12_finish = 0;
						RobotFinishFlag.SmallForkSGear_Out56_finish = 0;
					}					
				}	
				
				if(RobotState.GrabDir == 0)
				{
					if(RobotState.NextSteptStart == 0xff)
					{
						Rotate_Right_Out();																			/* С����ת������	*/
						BigFork_Extend_Right();																	/* ���������� */
						RobotState.NextSteptStart = 0;
					}
					if(RobotFinishFlag.Rotate_out_right_finish == 0xff)
					{
						SmallFork_Extend_Left();															/* С��������� */
					}
									
					
					if(RobotFinishFlag.BigFork_Extend_right_finish == 0xff)
					{
						ServoMove_Bigfork34_R_Out();
						RobotFinishFlag.BigFork_Extend_right_finish = 0;
					}
					
					if(RobotFinishFlag.SmallFork_Extend_finish == 0xff)
					{
						ServoMove_Smallfork56_Out();
						RobotFinishFlag.SmallFork_Extend_finish = 0;
					}				
					
					if((RobotFinishFlag.SmallForkSGear_Out56_finish == 0xff) && (RobotFinishFlag.BigForkSGear_rightOut34_finish == 0xff))
					{
							/* ����AGV���̸߶� */
							LiftBigFork_Move(positionActualValue_6063[0] + LiftBigForkADD + AGVHigh_Big,LiftBigForkProfile_V_short);		/* ���̧�� */						
							RobotFinishFlag.BigForkSGear_rightOut34_finish = 0;
							RobotFinishFlag.SmallForkSGear_Out56_finish = 0;
					}
				}					
	
				if(RobotFinishFlag.LiftBigFork_PositionArrive == 0xff)						/* ������λ�õ����ʼ����С�� */
				{
					
					LiftSmallFork_Move(positionActualValue_6063[1] + LiftSmallForkADD + AGVHigh_Small,LiftSmallForkProfile_V_short);		/* С��̧�� */
					RobotFinishFlag.LiftBigFork_PositionArrive = 0;											/* ������λ�õ����־ */
				}
				
				if(RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff)
				{
					RobotFinishFlag.LiftSmallFork_PositionArrive = 0;
					RobotState.TakeStept2 = 0;
					RobotState.TakeStept3 = 0xff;
					
					/* ��������������־λ����־����������һ���ĵ�һ������ */
					RobotState.NextSteptStart = 0xff;
					
				}					
			}	
			
			
		/*-----------------------------------------��3��start----------------------------------------------------------------*/
			if(RobotState.TakeStept3 == 0xff)																	/* ��3��				*/
			{				
				if(RobotState.GrabDir == 0xff)																				/* ȡ������� */
				{
					if(RobotState.NextSteptStart == 0xff)
					{
						RobotState.NextSteptStart = 0;
						SmallFork_Shrink_Left();																					/* С���������� */
					}
					if(RobotFinishFlag.SmallFork_Shrink_finish == 0xff)									/* С��������� */
					{
						RobotFinishFlag.SmallFork_Shrink_finish = 0;
						Rotate_Left_Back();																								/* ��ת��� */
					}	
					
					if(RobotFinishFlag.Rotate_back_left_finish == 0xff)									/* ��ת������� */
					{
						RobotFinishFlag.Rotate_back_left_finish = 0;
						
						LiftSmallFork_Move(LiftSmallFork_Floor[9],LiftSmallForkProfile_V_short);				/* ���ץȡ������Ϊ��1�㣬С�����е���Ͷ� */
						LiftBigFork_Move(LiftBigFork_Floor[LiftAllAction.Bin_Floor - 1],LiftBigForkProfile_V_short);		/* ����·����� */
					}		
				}
				
				if(RobotState.GrabDir == 0)																						/* ȡ�ұ����� */
				{
					if(RobotState.NextSteptStart == 0xff)
					{
						RobotState.NextSteptStart = 0;
						SmallFork_Shrink_Left();																					/* С���������� С��ֻ��һ�飬����ΪLeft */
					}
					
					if(RobotFinishFlag.SmallFork_Shrink_finish == 0xff)
					{
						RobotFinishFlag.SmallFork_Shrink_finish = 0;
						Rotate_Right_Back();																							/* ��ת�һ� */
					}	
					if(RobotFinishFlag.Rotate_back_right_finish == 0xff)																/* �ж��һ��Ƿ���� */
					{
						RobotFinishFlag.Rotate_back_right_finish = 0;					
						LiftSmallFork_Move(LiftSmallFork_Floor[9],LiftSmallForkProfile_V_short);				/* ���ץȡ������Ϊ��1�㣬С�����е���Ͷ� */
						LiftBigFork_Move(LiftBigFork_Floor[LiftAllAction.Bin_Floor - 1],LiftBigForkProfile_V_short);		/* ����·����� */
					}						
				}				
							
				if((RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff) && (RobotFinishFlag.LiftBigFork_PositionArrive == 0xff))						/* �ж�С���Ƿ����У�����������ó�λ�� */
				{			
					
					if(RobotState.GrabDir == 0xff)
					{
						ServoMove_Bigfork12_L_Back();																				/* �����߶���ջ� */
					}
					if(RobotState.GrabDir == 0)
					{
						ServoMove_Bigfork34_R_Back();
					}	
					
					RobotFinishFlag.LiftBigFork_PositionArrive = 0;												/* �����ɱ�� */
					RobotFinishFlag.LiftSmallFork_PositionArrive = 0;
				}
			
				
				if(RobotState.GrabDir == 0xff)
				{			
					if(RobotFinishFlag.BigForkSGear_LeftBack12_finish == 0xff )
					{
						RobotFinishFlag.BigForkSGear_LeftBack12_finish = 0;					
						BigFork_Shrink_Left();
					}	
					
					if(RobotFinishFlag.BigFork_Shrink_left_finish == 0xff)
					{
						RobotFinishFlag.BigFork_Shrink_left_finish = 0;
						RobotState.NextSteptStart = 0xff;
						RobotState.TakeStept3 = 0;
						RobotState.TakeStept4 = 0xff;
					}
				}					
				if(RobotState.GrabDir == 0)
				{			
					if(RobotFinishFlag.BigForkSGear_rightBack34_finish == 0xff )
					{
						RobotFinishFlag.BigForkSGear_rightBack34_finish = 0;
						BigFork_Shrink_Right();
					}

					if(RobotFinishFlag.BigFork_Shrink_right_finish == 0xff)
					{
						RobotFinishFlag.BigFork_Shrink_right_finish = 0;
						RobotState.NextSteptStart = 0xff;
						RobotState.TakeStept3 = 0;
						RobotState.TakeStept4 = 0xff;
					}
				}
			}
				
			/* --------------------------------��4��Start--------------------------------------------- */
			if(RobotState.TakeStept4 == 0xff)
			{
				if(RobotState.NextSteptStart == 0xff)
				{
					RobotState.NextSteptStart = 0;
					LiftMoveToBackPack(LiftAllAction.BackPackFloor);
				}
				
				if((RobotFinishFlag.LiftBigFork_PositionArrive == 0xff) && (RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff))
				{
					RobotFinishFlag.LiftBigFork_PositionArrive = 0;
					RobotFinishFlag.LiftSmallFork_PositionArrive = 0;
					SmallFork_Extend_Left();
				}	
				if(RobotFinishFlag.SmallFork_Extend_finish == 0xff)				/* С�������� */
				{
					RobotFinishFlag.SmallFork_Extend_finish = 0;
					
					RobotState.NextSteptStart = 0xff;
					RobotState.TakeStept4 = 0;
					RobotState.TakeStept5 = 0xff;
				}	
			}
/* ------------------------------------------��5��start---------------------------------------------------------------- */
					
			if(RobotState.TakeStept5 == 0xff)
			{				
				if(RobotState.NextSteptStart == 0xff)
				{
					RobotState.NextSteptStart = 0;
					LiftSmallFork_Move(positionActualValue_6063[1] - LiftSmallForkBackPackDec,LiftSmallForkProfile_V_short);																	/* �·����� */
				}						
				
				if(RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff)
				{
					
					ServoMove_Smallfork56_Back();																	/* �ջض�� */
					
					RobotFinishFlag.LiftSmallFork_PositionArrive = 0;
				}
				
				
				if(RobotFinishFlag.SmallForkSGear_Back56_finish == 0xff)				/* С�����ջ���� */
				{
					
					SmallFork_Shrink_Left();
					RobotFinishFlag.SmallForkSGear_Back56_finish = 0;
					
				}		
				
				if(RobotFinishFlag.SmallFork_Shrink_finish == 0xff)										/* С���ջ� */
				{
					RobotFinishFlag.SmallFork_Shrink_finish = 0;
					RobotState.TakeStept5 = 0;															
					RobotState.ControlState = 0;																				/* �������ģʽ */
					memset(&LiftAllAction,0,sizeof(LiftAllAction));
					memset(&RobotFinishFlag,0,sizeof(RobotFinishFlag));
					memset(&RobotState,0,sizeof(RobotState));
				}
			}	
			
		}
		
		
		
/*--------------------------------------ж������---------------------------------------------------*/
		if(LiftAllAction.TakePut == 0)													/* ж�� */
		{		
/* -------------------------------ж���1��Start------------------------------------------------- */
			if(RobotState.PutStept1 == 0xff)
			{
				if((RobotFinishFlag.LiftBigFork_PositionArrive == 0xff) && (RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff))
				{
					RobotState.PutStept1 = 0;
					RobotState.PutStept2 = 0xff;
					RobotFinishFlag.LiftBigFork_PositionArrive = 0;
					RobotFinishFlag.LiftSmallFork_PositionArrive = 0;
					
					
					/* ��������������־λ����־����������һ���ĵ�һ������ */
					RobotState.NextSteptStart = 0xff;
				}
			}			
			
/*-------------------------------------��2��Start--------------------------------------------------------------*/
				
			if(RobotState.PutStept2 == 0xff)
			{
				if(RobotState.NextSteptStart == 0xff)
				{
					RobotState.NextSteptStart = 0;
					SmallFork_Extend_Left();		
				}
				
				if(RobotFinishFlag.SmallFork_Extend_finish == 0xff)
				{
					RobotFinishFlag.SmallFork_Extend_finish = 0;
					ServoMove_Smallfork56_Out();
				}
				
				if(RobotFinishFlag.SmallForkSGear_Out56_finish == 0xff)
				{
					RobotFinishFlag.SmallForkSGear_Out56_finish = 0;
					LiftSmallFork_Move(BackPack_Floor[LiftAllAction.BackPackFloor - 1],LiftSmallForkProfile_V_short);				/* ���ﱳ¨̧��λ�� */
				}
				
				if(RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff)
				{
					RobotFinishFlag.LiftSmallFork_PositionArrive = 0;
					SmallFork_Shrink_Left();																			/* �õ����䣬�ջ�С�� */
				}
				
				if(RobotFinishFlag.SmallFork_Shrink_finish == 0xff)
				{
					RobotFinishFlag.SmallFork_Shrink_finish = 0;
					RobotState.PutStept2 = 0;
					RobotState.PutStept3 = 0xff;
					RobotState.NextSteptStart = 0xff;
				}
			}	
			
			
/*------------------------------------��3��Start-----------------------------------------------------*/			
			if(RobotState.PutStept3 == 0xff)
			{
				if(RobotState.NextSteptStart == 0xff)
				{
					RobotState.NextSteptStart = 0;
					LiftMoveToBin_PutBin(LiftAllAction.Bin_Floor);
				}
				
				if((RobotFinishFlag.LiftBigFork_PositionArrive == 0xff) && (RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff))
				{
					RobotFinishFlag.LiftBigFork_PositionArrive = 0;
					RobotFinishFlag.LiftSmallFork_PositionArrive = 0;
					if(RobotState.GrabDir == 0xff)
					{
						BigFork_Extend_Left();
//						Rotate_Left_Out();
					}
					if(RobotState.GrabDir == 0)
					{
						BigFork_Extend_Right();
//						Rotate_Right_Out();
					}
				}
				
				if(RobotState.GrabDir == 0xff)
				{
					if(RobotFinishFlag.BigFork_Extend_left_finish == 0xff)
					{
						RobotFinishFlag.BigFork_Extend_left_finish = 0;
						RobotState.PutStept3 = 0;
						RobotState.PutStept4 = 0xff;
						RobotState.NextSteptStart = 0xff;							
					}				
				}
				
				if(RobotState.GrabDir == 0)
				{		
					if(RobotFinishFlag.BigFork_Extend_right_finish == 0xff)
					{
						RobotFinishFlag.BigFork_Extend_right_finish = 0;
						RobotState.PutStept3 = 0;
						RobotState.PutStept4 = 0xff;
						RobotState.NextSteptStart = 0xff;
					}													
				}			
			}			
			
/*---------------------------��4��Start--------------------------------------------*/
			if(RobotState.PutStept4 == 0xff)
			{
				if(RobotState.NextSteptStart == 0xff)	
				{
					RobotState.NextSteptStart = 0;

					LiftBigFork_Move(LiftBigFork_Floor[0],LiftBigForkProfile_V_short);							/* ��������˶�����һ�� */
				}
				
				if(RobotFinishFlag.LiftBigFork_PositionArrive == 0xff)
				{					
					RobotFinishFlag.LiftBigFork_PositionArrive = 0;
					
					if(RobotState.GrabDir == 0xff)
					{
						ServoMove_Bigfork12_L_Out();		
					}
					if(RobotState.GrabDir == 0)
					{
						ServoMove_Bigfork34_R_Out();	
					}																																							
				}
				
				if(RobotState.GrabDir == 0xff)																							/* ����� */
				{
					if(RobotFinishFlag.BigForkSGear_LeftOut12_finish == 0xff) 
					{				
						RobotFinishFlag.BigForkSGear_LeftOut12_finish = 0;
						RobotState.PutStept4 = 0;
						RobotState.PutStept5 = 0xff;
						RobotState.NextSteptStart = 0xff;											
					}
					
				}	
				if(RobotState.GrabDir == 0)																									/* �������� */
				{		
					
					if(RobotFinishFlag.BigForkSGear_rightOut34_finish == 0xff)
					{
						RobotFinishFlag.BigForkSGear_rightOut34_finish = 0;
						
						RobotState.PutStept4 = 0;
						RobotState.PutStept5 = 0xff;
						RobotState.NextSteptStart = 0xff;
												
					}							
				}					
			}	
/* --------------------------��5����ʼ----------------------------------------- */				
			if(RobotState.PutStept5 == 0xff)
			{
				if(RobotState.NextSteptStart == 0xff)
				{
					RobotState.NextSteptStart = 0;
					LiftBigFork_Move(LiftBigFork_Floor[LiftAllAction.Bin_Floor] + LiftBigForkADD + AGVHigh_Big,LiftBigForkProfile_V_short);
				}
				
				if(RobotFinishFlag.LiftBigFork_PositionArrive == 0xff)
				{
					RobotFinishFlag.LiftBigFork_PositionArrive = 0;
					LiftSmallFork_Move(LiftSmallFork_Floor[0] + LiftBigForkADD + AGVHigh_Small,LiftSmallForkProfile_V_short);
				}		
				
				if(RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff)
				{
					RobotFinishFlag.LiftSmallFork_PositionArrive = 0;				
					
					if(RobotState.GrabDir == 0xff)
					{
						Rotate_Left_Out();
					}
					if(RobotState.GrabDir == 0)
					{
						Rotate_Right_Out();
					}		
				}
				
				if(RobotState.GrabDir == 0xff)
				{
					if(RobotFinishFlag.Rotate_out_left_finish == 0xff)
					{
						RobotFinishFlag.Rotate_out_left_finish = 0;
						SmallFork_Extend_Left();
						
					}	
				}
				if(RobotState.GrabDir == 0)
				{
					if(RobotFinishFlag.Rotate_out_right_finish == 0xff)
					{
						RobotFinishFlag.Rotate_out_right_finish = 0;						
						SmallFork_Extend_Left();
						
					}
				}
				
				if(RobotFinishFlag.SmallFork_Extend_finish == 0xff)
				{
					
					RobotFinishFlag.SmallFork_Extend_finish = 0;
					RobotState.PutStept5 = 0;
					RobotState.PutStept6 = 0xff;
					RobotState.NextSteptStart = 0xff;
				}

		
			}

/*-------------------------------��6��start------------------------------------------------------------*/						
			
			if(RobotState.PutStept6 == 0xff)
			{
				if(RobotState.NextSteptStart == 0xff)
				{
					RobotState.NextSteptStart = 0;
					LiftSmallFork_Move(LiftSmallFork_Floor[0],LiftSmallForkProfile_V_short);
				}
				
				if(RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff)
				{
					ServoMove_Smallfork56_Back();
					RobotFinishFlag.LiftSmallFork_PositionArrive = 0;
				}
				
				if(RobotFinishFlag.SmallForkSGear_Back56_finish == 0xff)
				{
					RobotFinishFlag.SmallForkSGear_Back56_finish = 0;					
					SmallFork_Shrink_Left();											/* С���ջ� */
				}					
				
				if(RobotFinishFlag.SmallFork_Shrink_finish == 0xff)
				{
					RobotFinishFlag.SmallFork_Shrink_finish = 0;
					
//					if(RobotState.GrabDir == 0xff)														/* ����ײ��޷���ת */
//					{
//						Rotate_Left_Back();
//					}
//					if(RobotState.GrabDir == 0)
//					{
//						Rotate_Right_Back();
//					}	
						RobotState.PutStept6 = 0;
						RobotState.PutStept7 = 0xff;
						RobotState.NextSteptStart = 0xff;

					
				}
				
//				if(RobotState.GrabDir == 0xff)
//				{
//					if(RobotFinishFlag.Rotate_back_left_finish == 0xff)
//					{
//						RobotFinishFlag.Rotate_back_left_finish = 0;
//						RobotState.PutStept6 = 0;
//						RobotState.PutStept7 = 0xff;
//						RobotState.NextSteptStart = 0xff;
//					}	
//				}
//				if(RobotState.GrabDir == 0)
//				{
//					if(RobotFinishFlag.Rotate_back_right_finish == 0xff)
//					{
//						RobotFinishFlag.Rotate_back_right_finish = 0;						
//						RobotState.PutStept6 = 0;
//						RobotState.PutStept7 = 0xff;
//						RobotState.NextSteptStart = 0xff;						
//					}
//				}	
			}
			
/* *-------------------------------��7��--------------------------------------------------------- */			
			if(RobotState.PutStept7 == 0xff)
			{
				if(RobotState.NextSteptStart == 0xff)
				{
					RobotState.NextSteptStart = 0;
					LiftBigFork_Move(LiftBigFork_Floor[LiftAllAction.Bin_Floor] + LiftBigForkADD,LiftBigForkProfile_V_short);
					/* ����½� */	
				}
				
				if(RobotFinishFlag.LiftBigFork_PositionArrive == 0xff)
				{
					RobotFinishFlag.LiftBigFork_PositionArrive = 0;
					
					if(RobotState.GrabDir == 0xff)
					{
						ServoMove_Bigfork12_L_Back();		
					}
					if(RobotState.GrabDir == 0)
					{
						ServoMove_Bigfork34_R_Back();	
					}	
				}
				if(RobotState.GrabDir == 0xff)
				{
					if(RobotFinishFlag.BigForkSGear_LeftOut12_finish == 0xff)
					{
						RobotFinishFlag.BigForkSGear_LeftOut12_finish = 0;
						RobotState.PutStept7 = 0;
						RobotState.PutStept8 = 0xff;
						RobotState.NextSteptStart = 0xff;		
						
					}						
				}
				if(RobotState.GrabDir == 0)
				{
					if(RobotFinishFlag.BigForkSGear_rightOut34_finish == 0xff)
					{
						RobotFinishFlag.BigForkSGear_rightOut34_finish = 0;
						RobotState.PutStept7 = 0;
						RobotState.PutStept8 = 0xff;
						RobotState.NextSteptStart = 0xff;							
					}
				}	
			}
			
/* ---------------------------------------��8��--------------------------------------------------------------------- */
			
			if(RobotState.PutStept8 == 0xff)
			{
				if(RobotState.NextSteptStart == 0xff)
				{
					/* ��С��̧��������ջأ�С����ת */
					
					LiftMoveToBin(3);
					RobotState.NextSteptStart = 0;
				}
				
				if((RobotFinishFlag.LiftBigFork_PositionArrive == 0xff) && (RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff))
				{
					
					if(RobotState.GrabDir == 0xff)
					{
						Rotate_Left_Back();
						BigFork_Shrink_Left();
					}
					if(RobotState.GrabDir == 0)
					{
						BigFork_Shrink_Right();
						Rotate_Right_Back();
					}						
				}
				
				if(RobotState.GrabDir == 0xff)
				{
					if((RobotFinishFlag.Rotate_back_left_finish == 0xff) && (RobotFinishFlag.BigFork_Shrink_left_finish == 0xff))
					{
						RobotFinishFlag.Rotate_back_left_finish = 0;
						RobotFinishFlag.BigFork_Shrink_left_finish = 0;
						RobotState.PutStept7 = 0;
						RobotState.PutStept8 = 0xff;
						RobotState.NextSteptStart = 0xff;		
					}						
				}
				if(RobotState.GrabDir == 0)
				{
					if((RobotFinishFlag.Rotate_back_right_finish == 0xff) && (RobotFinishFlag.BigFork_Shrink_right_finish == 0xff))
					{
						RobotFinishFlag.BigForkSGear_rightOut34_finish = 0;
						RobotFinishFlag.BigFork_Shrink_right_finish = 0;
						RobotState.PutStept7 = 0;
						RobotState.PutStept8 = 0xff;
						RobotState.NextSteptStart = 0xff;							
					}
				}
			}	
		}

	}

}




















