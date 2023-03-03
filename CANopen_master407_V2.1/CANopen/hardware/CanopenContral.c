#include "CanopenContral.h"



int32_t LiftBigFork_Floor[14] = {0};						/* 9：上极限位置    10：下极限位置  11: 大叉收回最小位置*/
int32_t LiftSmallFork_Floor[15] = {0};
int32_t BackPack_Floor[6] = {0};								
int32_t BackPack_Floor_PutDown[6] = {0};
int32_t BackPack_BigFork_sync[6] = {0};
int32_t LiftBigForkCapture_Floor[9] = {0};
int32_t LiftSmallForkCapute_Floor[9] = {0};

u8 BigForkHome_Flag = 0;

/*
	取位数组
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
	电机上电
	motor:
				BigFork
				SmallFork
				AGV左
				AGV右
*/
void PowerUpMotor(UNS8 motor)
{
	switch(motor)
	{
		case 1:														/* 大叉 */
			controlWord_6040[0] = 0x0F;

			break;
		case 2:														/* 小叉 */
			controlWord_6040[1] = 0x0F;

			break;	

	}
}

/* 
	电机断电
	motor:
				BigFork
				SmallFork
				AGV左
				AGV右
*/

void PowerDownMotor(UNS8 motor)
{
	switch(motor)
	{
		case 1:														/* 大叉 */
			controlWord_6040[0] = 0x06;

			break;
		case 2:														/* 小叉 */
			controlWord_6040[1] = 0x06;

			break;	


	}
}

/* 
	快速停止
	motor:
				BigFork
				SmallFork
				AGV左
				AGV右
*/

void QuickStopMotor(UNS8 motor)
{
	switch(motor)
	{
		case 1:														/* 大叉 */
			controlWord_6040[0] = 0x0B;

			break;
		case 2:														/* 小叉 */
			controlWord_6040[1] = 0x0B;

			break;	

	}
}

/* 
	清楚内部故障
	motor:
				BigFork
				SmallFork
				AGV左
				AGV右
*/
void ClearInternalFault(UNS8 motor)
{
	switch(motor)
	{
		case 1:														/* 大叉 */
			controlWord_6040[0] = 0x86;

			break;
		case 2:														/* 小叉 */
			controlWord_6040[1] = 0x86;

			break;	
	}
}


/*
	绝对位置模式------相对于原点
	motor:
				BigFork
				SmallFork
				AGV左
				AGV右
*/

void AbsolutePositionMode(UNS8 motor)
{
	switch(motor)
	{
		case 1:														/* 大叉 */
			controlWord_6040[0] = 0x3F;
			break;
		case 2:														/* 小叉 */
			controlWord_6040[1] = 0x3F;
			break;	
	}
}

/*
	相对位置模式------相对于上一次停止位置
	motor:
				BigFork
				SmallFork
				AGV左
				AGV右
*/
void RelativePositionMode(UNS8 motor)
{
	switch(motor)
	{
		case 1:														/* 大叉 */
			controlWord_6040[0] = 0x5F;
			break;
		case 2:														/* 小叉 */
			controlWord_6040[1] = 0x5F;

			break;	

	}
}

/*
		原点模式
	motor:
				BigFork
				SmallFork
				AGV左
				AGV右
*/


void HomeMode(UNS8 motor)
{
	switch(motor)
	{
		case 1:														/* 大叉 */
			controlWord_6040[0] = 0x1F;

			break;
		case 2:														/* 小叉 */
			controlWord_6040[1] = 0x1F;

			break;	
	}
}

/*
	工作模式配置
		motor:
				BigFork
				SmallFork
				AGV左
				AGV右
		mode:
				velocityMode
				positionMode
				homeMode
				
*/
void MotorWorkMode(UNS8 motor,UNS8 mode)
{
	switch(motor)
	{
		case 1:														/* 大叉 */
			modeOfOperation_6060[0] = mode;

			break;
		case 2:														/* 小叉 */
			modeOfOperation_6060[1] = mode;

			break;	
	}
}


/*
		目标位置
		motor:
				BigFork
				SmallFork
				AGV左
				AGV右
		value:
			位置数值
*/
void TargetPosition(UNS8 motor, UNS32 value)
{
	switch(motor)
	{
		case 1:														/* 大叉 */
			targetPosition_607A[0] = value;
			break;
		case 2:														/* 小叉 */
			targetPosition_607A[1] = value;

			break;	
	}
}

/*
		梯形速度
		motor:
				BigFork
				SmallFork
				AGV左
				AGV右
		position:
			位置数值
*/
void ProfileVelocity(UNS8 motor, UNS32 value)
{
	switch(motor)
	{
		case 1:														/* 大叉 */
			profileVelocity_6081[0] = value;
			break;
		case 2:														/* 小叉 */
			profileVelocity_6081[1] = value;
			break;	
	}
}



u8 HomeMove_Lift(u8 motor)																				/* 升降找原点 */
{
	if(motor == BigFork)
	{
		if(RobotFinishFlag.ReturnHome_State_finish[3] == 0xff)
		{
			RobotFinishFlag.ReturnHome_State_finish[3] = 0;							/* 清除回原完成标志 */
		}
		
		
		RobotState.ReturnHome_State[3] = 0xff;											/* 进入大叉回原状态 */
		modeOfOperation_6060[0] = 0x06;
		controlWord_6040[0] = 0x0f;
		HomeTIM_BigFork(0) = -1;
	}
	if(motor == SmallFork)
	{
		if(RobotFinishFlag.ReturnHome_State_finish[4] == 0xff)
		{
			RobotFinishFlag.ReturnHome_State_finish[4] = 0;							/* 清除回原完成标志 */
		}
		RobotState.ReturnHome_State[4] = 0xff;												/* 进入小叉回原状态 */
		modeOfOperation_6060[1] = 0x06;
		controlWord_6040[1] = 0x0f;
		HomeTIM_SmallFork(0) = -1;
	}
	
	return 0;
}


void Scanf_Lift_message(void)
{
	if(RobotState.ReturnHome_State[3] == 0xff)				/* 大叉原点模式 */
	{
		if(((Sensor_Din[0]&0x01) == 0x00) && (BigForkHome_Flag == 0)) // 大叉回原的方式需要特殊处理判断。
		{
			BigForkHome_Flag = 1;
		}
		if((Sensor_Din[0] == 0x01) && (BigForkHome_Flag ==1))													/* 大叉原点触发信号采集 */
		{
			if((positionActualValue_6063[0] >= -FloatingWindow) && (positionActualValue_6063[0] <= FloatingWindow))
			{
				RobotFinishFlag.ReturnHome_State_finish[3] = 0xff;			/* 回原完成标记 */
				RobotState.ReturnHome_State[3] = 0;											/* 清除回原状态标志 */
				BigForkHome_Flag = 0;
			}
		}
	}	
	if(RobotState.ReturnHome_State[4] == 0xff)				/* 小叉原点模式 */
	{
		if(Sensor_Din[1] == 0x01)															/* 小叉原点触发信号采集 */
		{
			if((positionActualValue_6063[1] >= -FloatingWindow) && (positionActualValue_6063[1] <= FloatingWindow))
			{
				RobotFinishFlag.ReturnHome_State_finish[4] = 0xff;			/* 回原完成标记 */
				RobotState.ReturnHome_State[4] = 0;											/* 清除回原状态标志 */
//				controlWord_6040[1] = 0x0f;
			}
		}
	}
}

void LiftBigFork_Move(int32_t targetPosition, u32 ProfileV)					/* 大叉运动到指定位置 */
{

	if(RobotFinishFlag.LiftBigFork_PositionArrive == 0xff)						/* 启动前清除到达标志 */
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


void LiftSmallFork_Move(int32_t targetPosition, u32 ProfileV)				/* 小叉运动到指定位置 */
{
	
	if(RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff)					/* 启动前清除到达标志 */
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


/* 大叉小叉同时运动到指定取料箱位置 */
u8 LiftMoveToBin(u8 Floor)
{
	if (Floor == 1)
	{
		LiftBigFork_Move(LiftBigFork_Floor[10],LiftBigForkProfile_V_Long);						/* 大叉运动到第三层  大叉运动到需求料箱的上一个位置 */
		LiftSmallFork_Move(LiftSmallFork_Floor[10],LiftSmallForkProfile_V_Long);	
	}
	else if (Floor == 8)
	{
		LiftBigFork_Move(LiftBigFork_Floor[8],LiftBigForkProfile_V_Long);						/* 大叉运动到最高点*/
		LiftSmallFork_Move(LiftSmallFork_Floor[12],LiftSmallForkProfile_V_Long);	// 小叉需要先到避免碰撞的位置；
	}
	else if (Floor == 50) //出入库部分只需要小叉工作
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
		LiftBigFork_Move(LiftBigFork_Floor[Floor],LiftBigForkProfile_V_Long);						/* 大叉运动到第三层  大叉运动到需求料箱的上一个位置 */
		LiftSmallFork_Move(LiftSmallFork_Floor[Floor-1],LiftSmallForkProfile_V_Long);	
	}
	return 0;
}



/* 大小叉到指定位置卸货，大叉到目标料箱位置，小叉到 */
u8 LiftMoveToBin_PutBin(u8 Floor)
{
	if(Floor == 1)
	{
		LiftBigFork_Move(LiftBigFork_Floor[Floor] + LiftBigForkADD + AGVHigh_Big,LiftBigForkProfile_V_Long);						/* 大叉运行也高速运动到一定位置 */
		LiftSmallFork_Move(LiftSmallFork_Floor[Floor - 1] + LiftSmallForkADD + AGVHigh_Small,LiftSmallForkProfile_V_Long);					/* 小叉高速运动到一定位置 */
	}
	else if (Floor == 8 || Floor == 7)
	{
		LiftBigFork_Move(LiftBigFork_Floor[8],LiftBigForkProfile_V_Long);						/* 大叉运动到最高点*/
		LiftSmallFork_Move(LiftSmallFork_Floor[12],LiftSmallForkProfile_V_Long);	 /*高速到避免碰撞的位置*/
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
		LiftBigFork_Move(LiftBigFork_Floor[Floor] + LiftBigForkADD,LiftBigForkProfile_V_Long);						/* 大叉运动到第三层  大叉运动到需求料箱的上一个位置 */
		LiftSmallFork_Move(LiftSmallFork_Floor[Floor-1] + LiftSmallForkADD,LiftSmallForkProfile_V_Long);	
	}
	return 0;
}


/* 大小叉到指定位置卸货，大叉到目标料箱位置，小叉到 */
u8 LiftMoveToBin_PutBin_backup(u8 Floor)
{
	if((Floor == 1) || (Floor == 2))
	{
		LiftBigFork_Move(LiftBigFork_Floor[11],LiftBigForkProfile_V_Long);						/* 大叉运行也高速运动到一定位置 */
		LiftSmallFork_Move(LiftSmallFork_Floor[11],LiftSmallForkProfile_V_Long);					/* 小叉高速运动到一定位置 */
	}
	else if (Floor == 8)
	{
		LiftBigFork_Move(LiftBigFork_Floor[8],LiftBigForkProfile_V_Long);						/* 大叉运动到最高点*/
		LiftSmallFork_Move(LiftSmallFork_Floor[12],LiftSmallForkProfile_V_Long);	 /*高速到避免碰撞的位置*/
	}
	else if (Floor == 50)
	{
		LiftBigFork_Move(LiftBigFork_Floor[13],LiftBigForkProfile_V_Long);	
		LiftSmallFork_Move(LiftSmallFork_Floor[14],LiftSmallForkProfile_V_Long);	
	}
	else
	{
		LiftBigFork_Move(LiftBigFork_Floor[Floor-1] ,LiftBigForkProfile_V_Long);						/* 大叉运动到第三层  大叉运动到需求料箱的上一个位置 */
		LiftSmallFork_Move(LiftSmallFork_Floor[Floor-2] ,LiftSmallForkProfile_V_Long);	
	}
	return 0;
}



/* 大小叉运动到背篓指定位置-------取箱后放置到背篓 */
u8 LiftMoveToBackPack(u8 Floor)
{
	if(positionActualValue_6063[0] < BackPack_BigFork_sync[Floor -1])
	{
		LiftBigFork_Move(BackPack_BigFork_sync[Floor -1],LiftBigForkProfile_V_Long);						/* 大叉到上一层背篓位置 */
	}
	else
	{
		RobotFinishFlag.LiftBigFork_PositionArrive = 0xff;
	}
	if((Floor ==5) || (Floor == 6))    /*第5、6层小叉会撞到大叉，声音很大，需要特殊处理*/
	{
		LiftSmallFork_Move(LiftSmallFork_Floor[12],LiftSmallForkProfile_V_Long);			/* 小叉高度运行到不会撞到大叉的高度 */
	}
	else
	{
		LiftSmallFork_Move(BackPack_Floor[Floor-1],LiftSmallForkProfile_V_Long);			/* 小叉到背篓位置 */
	}
	return 0;
}

/* 大小叉运动到背篓指定位置-----取背篓的料箱放置到库位*/
u8 LiftMoveToBackPack_Dec(u8 Floor)
{
	if(positionActualValue_6063[0] < BackPack_BigFork_sync[Floor -1])
	{
		LiftBigFork_Move(BackPack_BigFork_sync[Floor -1],LiftBigForkProfile_V_Long);						/* 大叉到上一层背篓位置 */
	}
	else
	{
		RobotFinishFlag.LiftBigFork_PositionArrive = 0xff;
	}
	if (Floor == 3)      /* 小叉与横杆干涉 需要到第4层位置进行取箱 */
	{
		LiftSmallFork_Move(BackPack_Floor_PutDown[Floor], LiftSmallForkProfile_V_Long);			
	}
	else if((Floor ==5) || (Floor == 6)) /* 小叉上升后会撞到大叉，所以先高速到不会撞到的位置 */
	{
		LiftSmallFork_Move(LiftSmallFork_Floor[12],LiftSmallForkProfile_V_Long);			/* 小叉高度运行到不会撞到大叉的高度 */
	}
	else
	{
		LiftSmallFork_Move(BackPack_Floor_PutDown[Floor-1],LiftSmallForkProfile_V_Long);			/* 小叉到背篓位置 */
	}
	return 0;
}



void canopenDataInit(void)
{
	/* 初始化CANOPEN主机 */
	unsigned char nodeID = 0x00;                   //节点ID
  setNodeId(&Master_Data, nodeID);
  setState(&Master_Data, Initialisation);				//节点初始化
	setState(&Master_Data, Pre_operational);	
	setState(&Master_Data, Operational);
	
//	stopSYNC(&Master_Data);

	delay_ms(10);

	
//	/* 使能从机 */
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

/* 大叉升降状态清除 */
void Lift_BigFork_ClearStateFun(void)
{
	if(Sensor_Din[0] == 0x02)
	{
		if(BigLiftLimit_Up == 0) //防抖检测
		{
			if((positionActualValue_6063[0] >= (targetPosition_607A[0]- 8*FloatingWindow)) && (positionActualValue_6063[0] < (targetPosition_607A[0] + 8*FloatingWindow)))  // 上极限放大到位区间
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

/* 小叉升降状态清除 */
void Lift_SmallFork_ClearStateFun(void)
{
	if(Sensor_Din[1] == 0x04)
	{
		if(SmallLiftLimit_Down == 0) //防抖检测
		{
			if((positionActualValue_6063[1] >= (targetPosition_607A[1]- 8*FloatingWindow)) && (positionActualValue_6063[1] < (targetPosition_607A[1] + 8*FloatingWindow)))  // 上极限放大到位区间
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
	if((RobotFinishFlag.AllReturnHome_Finish == 0xff) && (RobotState.ControlState == 0xff))			/* 已回原点，并且收到启动大小叉抓箱指令 */
	{
		if(LiftAllAction.TakePut == 0xff)							/* 取料箱状态 */
		{	
			/* -----------------------------第1步start--------------------------------------------- */
			if(RobotState.TakeStept1 == 0xff)
			{
				if((RobotFinishFlag.LiftBigFork_PositionArrive == 0xff) && (RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff))
				{
					RobotState.TakeStept1 = 0;
					RobotState.TakeStept2 = 0xff;
					RobotFinishFlag.LiftBigFork_PositionArrive = 0;
					RobotFinishFlag.LiftSmallFork_PositionArrive = 0;					
					
					/* 增加启动步进标志位，标志可以启动下一步的第一个步骤 */
					RobotState.NextSteptStart = 0xff;
				}
			}
			
			
/* ------------------------------------------------------第2步start----------------------------------------------------------------- */
			if(RobotState.TakeStept2 == 0xff)
			{
				if(RobotState.GrabDir == 0xff)																													/* 取左边料箱 */
				{
					if(RobotState.NextSteptStart == 0xff)
					{
						Rotate_Left_Out();																			/* 小叉旋转 左旋出	*/
						BigFork_Extend_Left();																	/* 大叉向左伸出 */
						RobotState.NextSteptStart = 0;
					}
	
					if(RobotFinishFlag.Rotate_out_left_finish == 0xff)
					{
						RobotFinishFlag.Rotate_out_left_finish = 0;
						SmallFork_Extend_Left();															/* 小叉向左伸出 */
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
						/* 增加AGV底盘高度 */
						LiftBigFork_Move(positionActualValue_6063[0] + LiftBigForkADD + AGVHigh_Big,LiftBigForkProfile_V_short);		/* 大叉抬升 */
						
						RobotFinishFlag.BigForkSGear_LeftOut12_finish = 0;
						RobotFinishFlag.SmallForkSGear_Out56_finish = 0;
					}					
				}	
				
				if(RobotState.GrabDir == 0)
				{
					if(RobotState.NextSteptStart == 0xff)
					{
						Rotate_Right_Out();																			/* 小叉旋转左旋出	*/
						BigFork_Extend_Right();																	/* 大叉向左伸出 */
						RobotState.NextSteptStart = 0;
					}
					if(RobotFinishFlag.Rotate_out_right_finish == 0xff)
					{
						SmallFork_Extend_Left();															/* 小叉向左伸出 */
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
							/* 增加AGV底盘高度 */
							LiftBigFork_Move(positionActualValue_6063[0] + LiftBigForkADD + AGVHigh_Big,LiftBigForkProfile_V_short);		/* 大叉抬升 */						
							RobotFinishFlag.BigForkSGear_rightOut34_finish = 0;
							RobotFinishFlag.SmallForkSGear_Out56_finish = 0;
					}
				}					
	
				if(RobotFinishFlag.LiftBigFork_PositionArrive == 0xff)						/* 如果大叉位置到达，开始启动小叉 */
				{
					
					LiftSmallFork_Move(positionActualValue_6063[1] + LiftSmallForkADD + AGVHigh_Small,LiftSmallForkProfile_V_short);		/* 小叉抬升 */
					RobotFinishFlag.LiftBigFork_PositionArrive = 0;											/* 清除大叉位置到达标志 */
				}
				
				if(RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff)
				{
					RobotFinishFlag.LiftSmallFork_PositionArrive = 0;
					RobotState.TakeStept2 = 0;
					RobotState.TakeStept3 = 0xff;
					
					/* 增加启动步进标志位，标志可以启动下一步的第一个步骤 */
					RobotState.NextSteptStart = 0xff;
					
				}					
			}	
			
			
		/*-----------------------------------------第3步start----------------------------------------------------------------*/
			if(RobotState.TakeStept3 == 0xff)																	/* 第3步				*/
			{				
				if(RobotState.GrabDir == 0xff)																				/* 取左边料箱 */
				{
					if(RobotState.NextSteptStart == 0xff)
					{
						RobotState.NextSteptStart = 0;
						SmallFork_Shrink_Left();																					/* 小叉由左收缩 */
					}
					if(RobotFinishFlag.SmallFork_Shrink_finish == 0xff)									/* 小叉收缩完成 */
					{
						RobotFinishFlag.SmallFork_Shrink_finish = 0;
						Rotate_Left_Back();																								/* 旋转左回 */
					}	
					
					if(RobotFinishFlag.Rotate_back_left_finish == 0xff)									/* 旋转返回完成 */
					{
						RobotFinishFlag.Rotate_back_left_finish = 0;
						
						LiftSmallFork_Move(LiftSmallFork_Floor[9],LiftSmallForkProfile_V_short);				/* 如果抓取的料箱为第1层，小叉下行到最低端 */
						LiftBigFork_Move(LiftBigFork_Floor[LiftAllAction.Bin_Floor - 1],LiftBigForkProfile_V_short);		/* 大叉下放料箱 */
					}		
				}
				
				if(RobotState.GrabDir == 0)																						/* 取右边料箱 */
				{
					if(RobotState.NextSteptStart == 0xff)
					{
						RobotState.NextSteptStart = 0;
						SmallFork_Shrink_Left();																					/* 小叉由左收缩 小叉只有一遍，名字为Left */
					}
					
					if(RobotFinishFlag.SmallFork_Shrink_finish == 0xff)
					{
						RobotFinishFlag.SmallFork_Shrink_finish = 0;
						Rotate_Right_Back();																							/* 旋转右回 */
					}	
					if(RobotFinishFlag.Rotate_back_right_finish == 0xff)																/* 判断右回是否完成 */
					{
						RobotFinishFlag.Rotate_back_right_finish = 0;					
						LiftSmallFork_Move(LiftSmallFork_Floor[9],LiftSmallForkProfile_V_short);				/* 如果抓取的料箱为第1层，小叉下行到最低端 */
						LiftBigFork_Move(LiftBigFork_Floor[LiftAllAction.Bin_Floor - 1],LiftBigForkProfile_V_short);		/* 大叉下放料箱 */
					}						
				}				
							
				if((RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff) && (RobotFinishFlag.LiftBigFork_PositionArrive == 0xff))						/* 判断小叉是否下行，给大叉下行让出位置 */
				{			
					
					if(RobotState.GrabDir == 0xff)
					{
						ServoMove_Bigfork12_L_Back();																				/* 大叉左边舵机收回 */
					}
					if(RobotState.GrabDir == 0)
					{
						ServoMove_Bigfork34_R_Back();
					}	
					
					RobotFinishFlag.LiftBigFork_PositionArrive = 0;												/* 清除完成标记 */
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
				
			/* --------------------------------第4步Start--------------------------------------------- */
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
				if(RobotFinishFlag.SmallFork_Extend_finish == 0xff)				/* 小叉伸出完成 */
				{
					RobotFinishFlag.SmallFork_Extend_finish = 0;
					
					RobotState.NextSteptStart = 0xff;
					RobotState.TakeStept4 = 0;
					RobotState.TakeStept5 = 0xff;
				}	
			}
/* ------------------------------------------第5步start---------------------------------------------------------------- */
					
			if(RobotState.TakeStept5 == 0xff)
			{				
				if(RobotState.NextSteptStart == 0xff)
				{
					RobotState.NextSteptStart = 0;
					LiftSmallFork_Move(positionActualValue_6063[1] - LiftSmallForkBackPackDec,LiftSmallForkProfile_V_short);																	/* 下放料箱 */
				}						
				
				if(RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff)
				{
					
					ServoMove_Smallfork56_Back();																	/* 收回舵机 */
					
					RobotFinishFlag.LiftSmallFork_PositionArrive = 0;
				}
				
				
				if(RobotFinishFlag.SmallForkSGear_Back56_finish == 0xff)				/* 小叉舵机收回完成 */
				{
					
					SmallFork_Shrink_Left();
					RobotFinishFlag.SmallForkSGear_Back56_finish = 0;
					
				}		
				
				if(RobotFinishFlag.SmallFork_Shrink_finish == 0xff)										/* 小叉收回 */
				{
					RobotFinishFlag.SmallFork_Shrink_finish = 0;
					RobotState.TakeStept5 = 0;															
					RobotState.ControlState = 0;																				/* 清除操作模式 */
					memset(&LiftAllAction,0,sizeof(LiftAllAction));
					memset(&RobotFinishFlag,0,sizeof(RobotFinishFlag));
					memset(&RobotState,0,sizeof(RobotState));
				}
			}	
			
		}
		
		
		
/*--------------------------------------卸货部分---------------------------------------------------*/
		if(LiftAllAction.TakePut == 0)													/* 卸货 */
		{		
/* -------------------------------卸箱第1步Start------------------------------------------------- */
			if(RobotState.PutStept1 == 0xff)
			{
				if((RobotFinishFlag.LiftBigFork_PositionArrive == 0xff) && (RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff))
				{
					RobotState.PutStept1 = 0;
					RobotState.PutStept2 = 0xff;
					RobotFinishFlag.LiftBigFork_PositionArrive = 0;
					RobotFinishFlag.LiftSmallFork_PositionArrive = 0;
					
					
					/* 增加启动步进标志位，标志可以启动下一步的第一个步骤 */
					RobotState.NextSteptStart = 0xff;
				}
			}			
			
/*-------------------------------------第2步Start--------------------------------------------------------------*/
				
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
					LiftSmallFork_Move(BackPack_Floor[LiftAllAction.BackPackFloor - 1],LiftSmallForkProfile_V_short);				/* 到达背篓抬起位置 */
				}
				
				if(RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff)
				{
					RobotFinishFlag.LiftSmallFork_PositionArrive = 0;
					SmallFork_Shrink_Left();																			/* 拿到料箱，收回小叉 */
				}
				
				if(RobotFinishFlag.SmallFork_Shrink_finish == 0xff)
				{
					RobotFinishFlag.SmallFork_Shrink_finish = 0;
					RobotState.PutStept2 = 0;
					RobotState.PutStept3 = 0xff;
					RobotState.NextSteptStart = 0xff;
				}
			}	
			
			
/*------------------------------------第3步Start-----------------------------------------------------*/			
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
			
/*---------------------------第4步Start--------------------------------------------*/
			if(RobotState.PutStept4 == 0xff)
			{
				if(RobotState.NextSteptStart == 0xff)	
				{
					RobotState.NextSteptStart = 0;

					LiftBigFork_Move(LiftBigFork_Floor[0],LiftBigForkProfile_V_short);							/* 大叉慢速运动到第一层 */
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
				
				if(RobotState.GrabDir == 0xff)																							/* 舵机左 */
				{
					if(RobotFinishFlag.BigForkSGear_LeftOut12_finish == 0xff) 
					{				
						RobotFinishFlag.BigForkSGear_LeftOut12_finish = 0;
						RobotState.PutStept4 = 0;
						RobotState.PutStept5 = 0xff;
						RobotState.NextSteptStart = 0xff;											
					}
					
				}	
				if(RobotState.GrabDir == 0)																									/* 舵机完成右 */
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
/* --------------------------第5步开始----------------------------------------- */				
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

/*-------------------------------第6步start------------------------------------------------------------*/						
			
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
					SmallFork_Shrink_Left();											/* 小叉收回 */
				}					
				
				if(RobotFinishFlag.SmallFork_Shrink_finish == 0xff)
				{
					RobotFinishFlag.SmallFork_Shrink_finish = 0;
					
//					if(RobotState.GrabDir == 0xff)														/* 在最底层无法旋转 */
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
			
/* *-------------------------------第7步--------------------------------------------------------- */			
			if(RobotState.PutStept7 == 0xff)
			{
				if(RobotState.NextSteptStart == 0xff)
				{
					RobotState.NextSteptStart = 0;
					LiftBigFork_Move(LiftBigFork_Floor[LiftAllAction.Bin_Floor] + LiftBigForkADD,LiftBigForkProfile_V_short);
					/* 大叉下降 */	
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
			
/* ---------------------------------------第8步--------------------------------------------------------------------- */
			
			if(RobotState.PutStept8 == 0xff)
			{
				if(RobotState.NextSteptStart == 0xff)
				{
					/* 大小叉抬升，大叉收回，小叉旋转 */
					
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




















