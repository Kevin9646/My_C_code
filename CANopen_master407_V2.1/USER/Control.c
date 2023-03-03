#include "Control.h"

/* �����������ýṹ�� */
struct LiftControl_Info LeftBigFork_Control,LeftSmallFork_Control;
struct SteptControl_Info steptBigFork_Info,steptSmallFork_Info,steptRotate_Info;
struct SteptControl_Info steptBigFork_Config[3],steptSmallFork_Config[2],steptRotate_Config[2];
struct FromFlashConfig FlashConfig;
struct Robot_Move_State	RobotState;
struct Robot_Finish_Flag RobotFinishFlag;
struct FromAGVCommand LiftAllAction;
struct FromAGVReceive AGVReceive;

u8 workStart_flag = 0;


/* �����������ɱ��� */
u8 lifecountB = 0;
u8 lifecountS = 0;

/* ��������������ɱ��� */
u8 stepStartB = 0;
u8 stepStartS = 0;
u8 stepStartR = 0;

/* ������Ʊ�־λ */
u8 Lobot2 = 0;
u8 dir_lobnot = 0;

u8 lift_fork_extra_action_flag = 0;
u8 lift_small_fork_extra_action_flag = 0;
u8 lift_fork_action_flag = 0;
u8 lift_small_fork_special_flag = 0;
u8 only_small_fork_work_flag = 0;
u8 backpack_servo_status = 0;


//�����غ���
unsigned int read_BigFork_StatusWord(int lineAddr)
{
	return statusWord_6041[0];
}
unsigned int read_SmallFork_StatusWord(int lineAddr)
{
	return statusWord_6041[1];
}
unsigned int read_BigFork_ErrorWord1(int lineAddr)
{
	return errorWord1[0];
}

unsigned int read_SmallFork_ErrorWord1(int lineAddr)
{
	return errorWord1[1];
}

unsigned int read_BigFork_ErrorWord2(int lineAddr)
{
	return errorWord2[0];
}

unsigned int read_SmallFork_ErrorWord2(int lineAddr)
{
	return errorWord2[1];
}


unsigned int Read_Flash_Config_rotate_home(int lineAddr)
{
	u16	Config = 0;
	Config = ReadConfig_Flash_16Bit(ADDR_ConfigData,ADDR_ConfigData + 2);
	return Config;
}

unsigned int Read_Flash_Config_rotate_left(int lineAddr)
{
	u16	Config = 0;
	Config = ReadConfig_Flash_16Bit(ADDR_ConfigData + 2,ADDR_ConfigData + 4);
	return Config;
}

unsigned int Read_Flash_Config_rotate_right(int lineAddr)
{
	u16	Config = 0;
	Config = ReadConfig_Flash_16Bit(ADDR_ConfigData + 4,ADDR_ConfigData + 6);
	return Config;
}

unsigned int Read_Flash_Config_download_flag(int lineAddr)
{
	u16	Config = 0;
	Config = ReadConfig_Flash_16Bit(ADDR_CodeWriteFlag,ADDR_CodeWriteFlag + 2);
	return Config;
}



unsigned int Read_SteeringGear_Position1(int lineAddr)																					/* ��ȡ���λ�� */
{

	return DD1;
}

unsigned int Read_SteeringGear_Position2(int lineAddr)																					/* ��ȡ���λ�� */
{

	return DD2;
}

unsigned int Read_SteeringGear_Position3(int lineAddr)																					/* ��ȡ���λ�� */
{

	return DD3;
}

unsigned int Read_SteeringGear_Position4(int lineAddr)																					/* ��ȡ���λ�� */
{

	return DD4;
}

unsigned int Read_SteeringGear_Position5(int lineAddr)																					/* ��ȡ���λ�� */
{

	return DD5;
}

unsigned int Read_SteeringGear_Position6(int lineAddr)																					/* ��ȡ���λ�� */
{

	return DD6;
}

unsigned int Read_LiftWorking_Status(int lineAddr)																					/* ��ȡ��������״̬ */
{
	return LiftStatus();
}

unsigned int Read_rotate_angle(int lineAddr)
{
	return angle;
}

unsigned int Read_BackPack_Servo_status(int lineAddr)
{
	return backpack_servo_status;
}

unsigned int Read_BigFork_floor(int lineAddr)
{
	for(int i=1;i<7;i++)
	{
		if(((LiftBigForkCapture_Floor[i]-FloatingWindow) <= positionActualValue_6063[0])  && (positionActualValue_6063[0] < (LiftBigForkCapture_Floor[i+1]- FloatingWindow)))
		{
			return i+1;
		}
	}
	return 8;
}

unsigned int Read_SmallFork_floor(int lineAddr)
{
	for(int i=1;i<7;i++)
	{
		if(((LiftSmallForkCapute_Floor[i]-FloatingWindow) <= positionActualValue_6063[1]) && (positionActualValue_6063[1] < (LiftSmallForkCapute_Floor[i+1] - FloatingWindow)))
		{
			return i+1;
		}
	}
	return 8;
}

/* ����IAP������������*/
void Jump_To_IAP (unsigned char addrH,unsigned char addrL,unsigned int val)
{
	Jump_To_IAP_Program();
}

/* ���������ûص����� */
void Write_Bigfork_ParaConfig(unsigned char addrH,unsigned char addrL,unsigned int val)
{
	switch(addrL)
	{
		case 0x00:
		{
			LeftBigFork_Control.targetPosition = ((UNS32)val) * 10000;
			break;
		}
		case 0x01:
		{
			LeftBigFork_Control.profileV = (((UNS32)(val)) * 512 * 10000) / 1875;
			break;
		}
		case 0x02:
		{
			lifecountB = val;
		}
	}
	if(lifecountB == 0x3f)
	{
		LeftBigFork_Control.motor = BigFork;
		LiftMove(LeftBigFork_Control);					/* ������� */
		lifecountB = 0;
	}
}

void Write_Bigfork_MoveToFloor(unsigned char addrH,unsigned char addrL,unsigned int val)					/* val ��ֵΪ0-7 */
{
	u8 i  = 0;
	i = (u8)val;
	LiftBigFork_Move(LiftBigFork_Floor[i],LiftBigForkProfile_V_Long);
}

void Write_Bigfork_MoveToFloorADD(unsigned char addrH,unsigned char addrL,unsigned int val)					/* val ��ֵΪ0-7 */
{
	u8 i  = 0;
	i = (u8)val;
	LiftBigFork_Move(LiftBigFork_Floor[i] + LiftBigForkADD,LiftBigForkProfile_V_short);
}


/* С��������ûص����� */
void Write_Smallfork_ParaConfig(unsigned char addrH,unsigned char addrL,unsigned int val)
{
	switch(addrL)
	{
		case 0x00:
		{
			LeftSmallFork_Control.targetPosition = ((UNS32)val) * 10000;
			break;
		}
		case 0x01:
		{
			LeftSmallFork_Control.profileV = (((UNS32)(val)) * 512 * 10000) / 1875;
			break;
		}
		case 0x02:
		{
			lifecountS = val;
		}
	}
	if(lifecountS == 0x3f)
	{
		LeftSmallFork_Control.motor = SmallFork;
		LiftMove(LeftSmallFork_Control);					/* С������ */
		lifecountS = 0;
	}
}

void Write_Smallfork_MoveToFloor(unsigned char addrH,unsigned char addrL,unsigned int val)					/* val ��ֵΪ0-7 */
{
	u8 i  = 0;
	i = (u8)val;
	LiftSmallFork_Move(LiftSmallFork_Floor[i],LiftSmallForkProfile_V_short);
}

void Write_Smallfork_MoveToFloorADD(unsigned char addrH,unsigned char addrL,unsigned int val)					/* val ��ֵΪ0-7  */
{
	u8 i  = 0;
	i = (u8)val;
	LiftSmallFork_Move(LiftSmallFork_Floor[i] + LiftSmallForkADD,LiftSmallForkProfile_V_short);
}

void Write_Smallfork_MoveToBackpack(unsigned char addrH,unsigned char addrL,unsigned int val)					/* val ��ֵΪ0-7  */
{
	u8 i  = 0;
	i = (u8)val;
	LiftSmallFork_Move(BackPack_Floor[i],LiftSmallForkProfile_V_Long);
}


void Write_Smallfork_MoveToBackpackDec(unsigned char addrH,unsigned char addrL,unsigned int val)					/* val ��ֵΪ0-7  */
{
	u8 i  = 0;
	i = (u8)val;
	LiftSmallFork_Move(BackPack_Floor_PutDown[i],LiftSmallForkProfile_V_short);
}


/* ԭ�������ص����� */
void Write_HomeMode_move(unsigned char addrH,unsigned char addrL,unsigned int val)
{
	HomeMove_Lift(((u8)val));
}

void Write_SteptMotorB_move(unsigned char addrH,unsigned char addrL,unsigned int val)				/* ������ */
{
	switch(addrL)
	{
		case 0x00:
		{
			
			steptBigFork_Info.pulse = (val * spr * 8.67)/100;													/* �Ӽ��ٻ���ϵ��Ϊ10 */
			break;
		}
		case 0x01:
		{
			steptBigFork_Info.accV = val;
			break;
		}
		case 0x02:
		{
			steptBigFork_Info.decV = val;
			break;
		}
		case 0x03:
		{
			steptBigFork_Info.speed = val;
			break;
		}
		case 0x04:
		{
			steptBigFork_Info.dir = val;
			break;
		}
		case 0x05:
		{
			stepStartB = val;
			break;
		}
	}
	if(stepStartB == 0x3f)
	{
//		StepMotor_PWM(BigFork_Num);
		steptBigFork_Info.startUp = 0xff;
		stepStartB = 0;
	}
}


void Write_SteptMotor_BigControl(unsigned char addrH,unsigned char addrL,unsigned int val)
{
	RobotFinishFlag.AllReturnHome_Finish = 0; //��������ÿ�������˶�����Ҫ������ԭȷ�������Ŀɿ�����
	switch(addrL)
	{
		case 0x06:
		{
			SteptBigFork_ReturnHome();																									/* ����ԭ */
			break;
		}
		case 0x07:
		{
			BigFork_Extend_Left();																										  /* ������� */
			break;
		}
		case 0x08:
		{
			BigFork_Shrink_Left();																											/* ������� */
			break;
		}
		case 0x09:
		{
			BigFork_Extend_Right();																											/* ������� */
			break;
		}
		case 0x0a:
		{
			BigFork_Shrink_Right();																											/* ������� */
			break;
		}
	}
}

void Write_SteptMotorS_move(unsigned char addrH,unsigned char addrL,unsigned int val)
{
	switch(addrL)
	{
		case 0x00:
		{
 			steptSmallFork_Info.pulse = (val * spr * 10)/100;											/* �Ӽ��ٻ���ϵ��Ϊ10 */
			break;
		}
		case 0x01:
		{
			steptSmallFork_Info.accV = val;
			break;
		}
		case 0x02:
		{
			steptSmallFork_Info.decV = val;
			break;
		}
		case 0x03:
		{
			steptSmallFork_Info.speed = val;
			break;
		}
		case 0x04:
		{
			steptSmallFork_Info.dir = val;
			break;
		}
		case 0x05:
		{
			stepStartS = val;
			break;
		}
	}
	if(stepStartS == 0x3f)
	{
//		StepMotor_PWM(SmallFork_Num);
		steptSmallFork_Info.startUp = 0xff;
		stepStartS = 0;
	}
}

void Write_SteptMotor_SmallControl(unsigned char addrH,unsigned char addrL,unsigned int val)
{
	RobotFinishFlag.AllReturnHome_Finish = 0; //��������ÿ�������˶�����Ҫ������ԭȷ�������Ŀɿ�����
	switch(addrL)
	{
		case 0x06:
		{
			SteptSmallFork_ReturnHome();																								/* С���ԭ */
			break;
		}
		case 0x07:
		{
			SmallFork_Extend_Left();																										  /* ������� */
			break;
		}
		case 0x08:
		{
			SmallFork_Shrink_Left();																											/* ������� */
			break;
		}
	}
}

void Write_SteptMotorR_move(unsigned char addrH,unsigned char addrL,unsigned int val)
{
	switch(addrL)
	{
		case 0x00:
		{
			steptRotate_Info.pulse = (val * spr )/100;													
			break;
		}
		case 0x01:
		{
			steptRotate_Info.accV = val;
			break;
		}
		case 0x02:
		{
			steptRotate_Info.decV = val;
			break;
		}
		case 0x03:
		{
			steptRotate_Info.speed = val;
			break;
		}
		case 0x04:
		{
			steptRotate_Info.dir = val;
			break;
		}
		case 0x05:
		{
			stepStartR = val;
			break;
		}
	}
	if(stepStartR == 0x3f)
	{
//		StepMotor_PWM(Rotate_Num);
		steptRotate_Info.startUp = 0xff;
		stepStartR = 0;
	}	
}

void Write_SteptMotor_RotateControl(unsigned char addrH,unsigned char addrL,unsigned int val)
{
	RobotFinishFlag.AllReturnHome_Finish = 0; //��������ÿ�������˶�����Ҫ������ԭȷ�������Ŀɿ�����
	switch(addrL)
	{
		case 0x06:
		{
			Rotate_ReturnHome();																									/* ��ת��ԭ */
			break;
		}
		case 0x07:
		{
			Rotate_Left_Out();																										  /* ��ת��� */
			break;
		}
		case 0x08:
		{
			Rotate_Left_Back();																											/* ��ת��� */
			break;
		}
		case 0x09:
		{
			Rotate_Right_Out();																											/* ��ת�ҳ� */
			break;
		}
		case 0x0a:
		{
			Rotate_Right_Back();																										/* ��ת�һ� */
			break;
		}
		case 0x0b:
		{
			Rotate_Behind_Out_left();																									  /* ��ת����� */
			break;
		}
		case 0x0c:
		{
			Rotate_Behind_Back_left();																											/* ��ת��غ� */
			break;
		}
		case 0x0d:
		{
			Rotate_Behind_Out_right();																											/* ��ת�ҳ���*/
			break;
		}
		case 0x0e:
		{
			Rotate_Behind_Back_right();																									/* ��ת�һغ�*/
			break;
		}
	}
}

void Write_LobotSerialServoMove1(unsigned char addrH,unsigned char addrL,unsigned int val)
{
	dir_lobnot = val;
	if((dir_lobnot !=7) && (dir_lobnot != 8))  //��¨���������λ�����ƣ���ʱ�������ơ�
	{
		RobotFinishFlag.AllReturnHome_Finish = 0; //��������ÿ�������˶�����Ҫ������ԭȷ�������Ŀɿ�����
	}
	//	LobotSerialServoMove(ID1, val, 100);
	Lobot2 = 0xff;
}


void Write_LobotSerialServoMove2(unsigned char addrH,unsigned char addrL,unsigned int val)
{
	switch(addrL)
	{
		case 0x01:
		{
			LobotSerialServoMove(ID1, val, 100);
			break;
		}
		case 0x02:
		{
			LobotSerialServoMove(ID2, val, 100);
			break;
		}
		case 0x03:
		{
			LobotSerialServoMove(ID3, val, 100);
			break;
		}
		case 0x04:
		{
			LobotSerialServoMove(ID4, val, 100);
			break;
		}
		case 0x05:
		{
			LobotSerialServoMove(ID5, val, 100);
			break;
		}
		case 0x06:
		{
			LobotSerialServoMove(ID7, val, 100);
			break;
		}
		case 0x07:
		{
			LobotSerialServoMove(ID7, val, 100);
			break;
		}
		case 0x08:
		{
			LobotSerialServoMove(ID8, val, 100);
			break;
		}		
	}
}

void Write_Work_Start(unsigned char addrH,unsigned char addrL,unsigned int val)		/* ������ʼ�� */
{	
	workStart_flag = 0xff;
}


void Write_ConfigToFlash(unsigned char addrH,unsigned char addrL,unsigned int val)
{
	switch(addrL)
	{
		case 0x00:
		{	
			EraseSector_Flash();																										/* �������� */
			break;
		}
		case 0x01:
		{
			WriteConfig_Flash_16Bit(val,  ADDR_ConfigData,  ADDR_ConfigData + 2);
			break;
		}	
		case 0x02:
		{
			WriteConfig_Flash_16Bit(val,  ADDR_ConfigData + 2,  ADDR_ConfigData + 4);
			break;
		}	
		case 0x03:
		{
			WriteConfig_Flash_16Bit(val,  ADDR_ConfigData + 4,  ADDR_ConfigData + 6);
			break;
		}			
	}
}

void Write_detact_bin(unsigned char addrH,unsigned char addrL,unsigned int val)
{
	if(LiftStatus() == 0)
	{
		switch(addrL)
		{
			case 0x00:																						/* ȡ�� */
			{
				LiftAllAction.TakePut = val;												/* 0xff:ȡ ��0���� */
				AGVReceive.TakePut_receive = 0xff;
				break;
			}
			case 0x01:																						/* ���� */
			{
				LiftAllAction.Dir = val;														/* 0xff:�� �� 0���� */
				AGVReceive.Dir_receive = 0xff;			
				break;
			}
			case 0x02:																						/* ������� */
			{
				if(((val>=1) && (val <=8)) || (val == 50))
				{
					LiftAllAction.Bin_Floor = val;											/* 1-8 */
					AGVReceive.Bin_Floor_receive = 0xff;	
				}
				break;
			}
			case 0x03:																						/* ��¨���� */
			{
				if((val>=1) && (val<=6))
				{
					LiftAllAction.BackPackFloor = val;									/* 1-6 */
					AGVReceive.BackPack_Floor_receive = 0xff;
				}
				break;
			}
		}
		if((AGVReceive.BackPack_Floor_receive == 0xff) && (AGVReceive.Bin_Floor_receive == 0xff) && (AGVReceive.Dir_receive == 0xff) && (AGVReceive.TakePut_receive == 0xff))
		{
			memset(&AGVReceive,0,sizeof(AGVReceive));
			RobotState.ControlState = 0xff;					/* ��������빤��״̬ */
			
			if((RobotFinishFlag.AllReturnHome_Finish == 0xff) && (RobotState.ControlState == 0xff))			/* �ѻ�ԭ�㣬�����յ�������С��ץ��ָ�� */
			{
				if(LiftAllAction.Dir == 0xff)
				{
					RobotState.GrabDir = 0xff;				/* ��ץ */
				}
				if(LiftAllAction.Dir == 0)
				{
					RobotState.GrabDir = 0;						/* ��ץ */
				}
				if(LiftAllAction.TakePut == 0xff)										/* ��ȡ��������±����Ҫȡ�ŵ���������ͱ�¨���� */
				{
					if (LiftAllAction.Dir == 0xff)
					{
						Rotate_Left_Out();																			/* С����ת ������	*/
						RobotState.TakeStept1 = 0xff;
					}
					if(LiftAllAction.Dir == 0)
					{
						Rotate_Right_Out();																			/* С����ת ������	*/
						RobotState.TakeStept1 = 0xff;
					}		
				}
				
				if(LiftAllAction.TakePut == 0)																					/* ��ж��������±����Ҫȡ�ŵ���������ͱ�¨���� */
				{
					switch(LiftAllAction.BackPackFloor)
					{
						case 0x01:
						{
							LiftMoveToBackPack_Dec(1);
							RobotState.PutStept1 = 0xff;
							
							break;
						}
						case 0x02:
						{
							LiftMoveToBackPack_Dec(2);
							RobotState.PutStept1 = 0xff;					
							break;
						}
						case 0x03:
						{					
							LiftMoveToBackPack_Dec(3);
							RobotState.PutStept1 = 0xff;					
							break;
						}
						case 0x04:
						{					
							LiftMoveToBackPack_Dec(4);
							RobotState.PutStept1 = 0xff;					
							break;
						}
						case 0x05:
						{					
							LiftMoveToBackPack_Dec(5);
							RobotState.PutStept1 = 0xff;					
							break;
						}
						case 0x06:
						{					
							LiftMoveToBackPack_Dec(6);
							RobotState.PutStept1 = 0xff;					
							break;
						}
					}
				}
			}
		}
	}
}


void Write_move_to_floor(unsigned char addrH, unsigned char addrL, unsigned int val)
{
	if(LiftStatus() == 0)
	{
		switch(addrL)
		{
			case 0x04:
				LiftAllAction.Capture = val;												/* 0xff:���գ�0�����ƶ� */
				AGVReceive.Capture_recerive = 0xff;
				break;
			case 0x05:
				LiftAllAction.Dir = val;												/* ����   0xff:�� �� 0���� */
				AGVReceive.Dir_receive = 0xff;
				break;
			case 0x06:
				if(((val>=1) && (val<=8)) || (val==50))
				{
					LiftAllAction.Bin_Floor = val;												/* ����   1-8     50������*/
					AGVReceive.Bin_Floor_receive = 0xff;
				}
				break;
		}
		if((AGVReceive.Capture_recerive == 0xff) && (AGVReceive.Dir_receive == 0xff) && (AGVReceive.Bin_Floor_receive == 0xff))
		{
			memset(&AGVReceive,0,sizeof(AGVReceive));
			RobotState.ControlState = 0xff;					/* ��������빤��״̬ */
			if((RobotFinishFlag.AllReturnHome_Finish == 0xff) && (RobotState.ControlState == 0xff))	
			{
				if(LiftAllAction.Capture == 0xff)
				{
					if (LiftAllAction.Dir == 0xff)
					{
						Rotate_Left_Out();																			/* С����ת ������	*/
						RobotState.CaptureSetpt1 = 0xff;
					}
					if(LiftAllAction.Dir == 0)
					{
						Rotate_Right_Out();																			/* С����ת ������	*/
						RobotState.CaptureSetpt1 = 0xff;
					}	
				}
				else if(LiftAllAction.Capture == 0)
				{
					LiftMoveToBinCaptureImage(LiftAllAction.Bin_Floor);
					RobotState.MoveSetpt1 = 0xff;
				}
			}
		}
	}
}
	
//int adsd = 0;
void Write_Once_Test(unsigned char addrH,unsigned char addrL,unsigned int val)
{
//	int b = 0;
//	u8 a[2] = {0};
	AllReturnHome();								//�����ԭ
}


void Write_Bigfork_Move(unsigned char addrH,unsigned char addrL,unsigned int val)
{
	if(val==1)
	{
		LiftBigFork_Move(positionActualValue_6063[0] + LiftBigforkrkMoveDetal,LiftBigForkProfile_V_short);	
	}
	else
	{
		LiftBigFork_Move(positionActualValue_6063[0] - LiftBigforkrkMoveDetal,LiftBigForkProfile_V_short);	
	}
			
}

void Write_Smallfork_Move(unsigned char addrH,unsigned char addrL,unsigned int val)
{
	if(val == 1)
	{
		LiftSmallFork_Move(positionActualValue_6063[1] + LiftSmallForkMoveDetal,LiftSmallForkProfile_V_short);
	}
	else
	{
		LiftSmallFork_Move(positionActualValue_6063[1] - LiftSmallForkMoveDetal,LiftSmallForkProfile_V_short);
	}
	
}


 
void Contact_Init(void)
{   
	//�������ñ�����ַ
	SetDecAddr(0x02);	//����ַ��Ϊ0x02

	SetAddrLenght(0x00,7);				/* ��������������ַ */
	SetAddrLenght(0x01,9);				/* С�������������ַ */
	SetAddrLenght(0x02,1);				/* ԭ�������������ַ */
	
	
	SetAddrLenght(0x03,11);				/* ���������� */
	SetAddrLenght(0x04,9);				/* �������С�� */
	
	SetAddrLenght(0x05,15);				/* ���������ת */
	

	
	SetAddrLenght(0x06,1);				/* ��� */
	
	SetAddrLenght(0x07,1);				/* ����������³�ʼ��CANOPEN */
	
	
	SetAddrLenght(0x08,4);				/* Flash�洢������Ϣ */	
	
	SetAddrLenght(0x09,7);				/* ����ץȡָ�� */
	
	SetAddrLenght(0x0a,10);				/* ������ */
	
	SetAddrLenght(0x0B,6);				/* ������ */
	
	SetAddrLenght(0x0c,5);        /*��ִ�л�������״̬*/
	SetAddrLenght(0x0d,1);        /*����--��������*/
	
  //ִ��DoInitProt()
  DoInitProt();
  //���ûص�����(������������)

	
	/* ���-λ��ģʽ */
	SetWriteFunc(0x00,0x00,Write_Bigfork_ParaConfig);				/* Ŀ��λ�� */		
	SetWriteFunc(0x00,0x01,Write_Bigfork_ParaConfig);				/* �����ٶ� */
	SetWriteFunc(0x00,0x02,Write_Bigfork_ParaConfig);				/* �Ƿ�������� 0x3f����������������  */
	
	SetWriteFunc(0x00,0x03,Write_Bigfork_MoveToFloor);		/* ����˶���ָ������λ�� */
	SetWriteFunc(0x00,0x04,Write_Bigfork_MoveToFloorADD);		/* ����˶���ָ������λ�� */
	SetWriteFunc(0x00,0x05,Write_Bigfork_Move);		/* ����ƶ� */
	SetWriteFunc(0x00,0x06,Write_Bigfork_Move);		/* ����ƶ����� */
	
	
	/* С��-λ��ģʽ */
	SetWriteFunc(0x01,0x00,Write_Smallfork_ParaConfig);			/* Ŀ��λ�� */	
	SetWriteFunc(0x01,0x01,Write_Smallfork_ParaConfig);			/* �����ٶ� */
	SetWriteFunc(0x01,0x02,Write_Smallfork_ParaConfig);			/* �Ƿ�����С�� 0x3f���������������� */
	
	SetWriteFunc(0x01,0x03,Write_Smallfork_MoveToFloor);		/* С���˶���ָ������λ�� */
	SetWriteFunc(0x01,0x04,Write_Smallfork_MoveToFloorADD);		/* С���˶���̧���߶� */
	
	SetWriteFunc(0x01,0x05,Write_Smallfork_MoveToBackpack);		/* С���˶�����¨�߶� */
	SetWriteFunc(0x01,0x06,Write_Smallfork_MoveToBackpackDec);		/* С���˶�����¨����λ�� */	
	SetWriteFunc(0x01,0x07,Write_Smallfork_Move);		/* С���ƶ� */
	SetWriteFunc(0x01,0x08,Write_Smallfork_Move);		/* С���ƶ����� */
	
	
	
	/* ԭ��ģʽ���� */
	SetWriteFunc(0x02,0x00,Write_HomeMode_move);						/* ����� BigFork 1   ��SmallFork  2  */
	
	/* �����������   ��� */
	SetWriteFunc(0x03,0x00,Write_SteptMotorB_move);
	SetWriteFunc(0x03,0x01,Write_SteptMotorB_move);
	SetWriteFunc(0x03,0x02,Write_SteptMotorB_move);
	SetWriteFunc(0x03,0x03,Write_SteptMotorB_move);
	SetWriteFunc(0x03,0x04,Write_SteptMotorB_move);
	SetWriteFunc(0x03,0x05,Write_SteptMotorB_move);
	
	SetWriteFunc(0x03,0x06,Write_SteptMotor_BigControl);							/* ����ԭ */
	SetWriteFunc(0x03,0x07,Write_SteptMotor_BigControl);							/* ������� */
	SetWriteFunc(0x03,0x08,Write_SteptMotor_BigControl);							/* ������� */
	SetWriteFunc(0x03,0x09,Write_SteptMotor_BigControl);							/* ������� */
	SetWriteFunc(0x03,0x0a,Write_SteptMotor_BigControl);							/* ������� */
			


		/* �����������  С�� */
	SetWriteFunc(0x04,0x00,Write_SteptMotorS_move);
	SetWriteFunc(0x04,0x01,Write_SteptMotorS_move);
	SetWriteFunc(0x04,0x02,Write_SteptMotorS_move);
	SetWriteFunc(0x04,0x03,Write_SteptMotorS_move);
	SetWriteFunc(0x04,0x04,Write_SteptMotorS_move);
	SetWriteFunc(0x04,0x05,Write_SteptMotorS_move);
	
	SetWriteFunc(0x04,0x06,Write_SteptMotor_SmallControl);						/* С���ԭ */
	SetWriteFunc(0x04,0x07,Write_SteptMotor_SmallControl);						/* С������ */
	SetWriteFunc(0x04,0x08,Write_SteptMotor_SmallControl);						/* С������ */
	
	
		/* �����������  ��ת */
	SetWriteFunc(0x05,0x00,Write_SteptMotorR_move);
	SetWriteFunc(0x05,0x01,Write_SteptMotorR_move);
	SetWriteFunc(0x05,0x02,Write_SteptMotorR_move);
	SetWriteFunc(0x05,0x03,Write_SteptMotorR_move);
	SetWriteFunc(0x05,0x04,Write_SteptMotorR_move);
	SetWriteFunc(0x05,0x05,Write_SteptMotorR_move);
	
	SetWriteFunc(0x05,0x06,Write_SteptMotor_RotateControl);					  /* ��ת��ԭ */
	SetWriteFunc(0x05,0x07,Write_SteptMotor_RotateControl);						/* ��ת��� */
	SetWriteFunc(0x05,0x08,Write_SteptMotor_RotateControl);						/* ��ת��� */
	SetWriteFunc(0x05,0x09,Write_SteptMotor_RotateControl);						/* ��ת�ҳ� */
	SetWriteFunc(0x05,0x0a,Write_SteptMotor_RotateControl);						/* ��ת�һ� */
	SetWriteFunc(0x05,0x0b,Write_SteptMotor_RotateControl);						/* ��ת����� */
	SetWriteFunc(0x05,0x0c,Write_SteptMotor_RotateControl);						/* ��ת��غ� */
	SetWriteFunc(0x05,0x0d,Write_SteptMotor_RotateControl);						/* ��ת�ҳ��� */
	SetWriteFunc(0x05,0x0e,Write_SteptMotor_RotateControl);						/* ��ת�һغ�*/

	/* ��� */
	SetWriteFunc(0x06,0x00,Write_LobotSerialServoMove1);						/* ������� ��ԭ */
	
	
//	SetWriteFunc(0x06,0x01,Write_LobotSerialServoMove2);						/* ���¾�Ϊ���λ�õ���ʹ�� */
//	SetWriteFunc(0x06,0x02,Write_LobotSerialServoMove2);	
//	SetWriteFunc(0x06,0x03,Write_LobotSerialServoMove2);
//	SetWriteFunc(0x06,0x04,Write_LobotSerialServoMove2);
//	SetWriteFunc(0x06,0x05,Write_LobotSerialServoMove2);
//	SetWriteFunc(0x06,0x06,Write_LobotSerialServoMove2);
//	SetWriteFunc(0x06,0x07,Write_LobotSerialServoMove2);
//	SetWriteFunc(0x06,0x08,Write_LobotSerialServoMove2);
	
	
	
	SetWriteFunc(0x07,0x00,Write_Work_Start);											/* CANOPEN������ϣ���ʼ�� */	
	
	/* flash */
	SetWriteFunc(0x08,0x00,Write_ConfigToFlash);											/* �������� */	
	SetWriteFunc(0x08,0x01,Write_ConfigToFlash);											/* ��תԭ��Ƕ� */
	SetWriteFunc(0x08,0x02,Write_ConfigToFlash);											/* ��ת���Ƕ� */
	SetWriteFunc(0x08,0x03,Write_ConfigToFlash);											/* ��ת�Ҳ�Ƕ�*/
	
	
	
/* ����ץȡ���� */
	SetWriteFunc(0x09,0x00,Write_detact_bin);
	SetWriteFunc(0x09,0x01,Write_detact_bin);
	SetWriteFunc(0x09,0x02,Write_detact_bin);	
	SetWriteFunc(0x09,0x03,Write_detact_bin);
	
/* �ƶ���Ŀ��㣬Ŀ�귽��׼�����ж��ζ�λ����*/
  SetWriteFunc(0x09, 0x04, Write_move_to_floor);
	SetWriteFunc(0x09, 0x05, Write_move_to_floor);
	SetWriteFunc(0x09, 0x06, Write_move_to_floor);

	/* �������� */
	SetWriteFunc(0x0a,0x00,Write_Once_Test);	
	
	/* ��ȡ��桢С��״̬�ֺʹ����� */
	SetReadFunc(0x0a,0x00,read_BigFork_StatusWord);
	SetReadFunc(0x0a,0x01,read_SmallFork_StatusWord);
	SetReadFunc(0x0a,0x02,read_BigFork_ErrorWord1);
	SetReadFunc(0x0a,0x03,read_BigFork_ErrorWord2);
	SetReadFunc(0x0a,0x04,read_SmallFork_ErrorWord1);	
	SetReadFunc(0x0a,0x05,read_SmallFork_ErrorWord2);	

	/* ��ȡflash���ò��� */
	SetReadFunc(0x0a,0x06,Read_Flash_Config_rotate_home);	
	SetReadFunc(0x0a,0x07,Read_Flash_Config_rotate_left);
	SetReadFunc(0x0a,0x08,Read_Flash_Config_rotate_right);
	SetReadFunc(0x0a,0x09,Read_Flash_Config_download_flag);

	/* ��ȡ���λ�� */
	SetReadFunc(0x0b,0x00,Read_SteeringGear_Position1);
	SetReadFunc(0x0b,0x01,Read_SteeringGear_Position2);
	SetReadFunc(0x0b,0x02,Read_SteeringGear_Position3);
	SetReadFunc(0x0b,0x03,Read_SteeringGear_Position4);
	SetReadFunc(0x0b,0x04,Read_SteeringGear_Position5);	
	SetReadFunc(0x0b,0x05,Read_SteeringGear_Position6);	
	
	SetReadFunc(0x0c,0x00,Read_LiftWorking_Status);
	SetReadFunc(0x0c,0x01,Read_rotate_angle);
	SetReadFunc(0x0c,0x02,Read_BackPack_Servo_status); //�ⲿ���ƺ�ı�¨���״̬
	SetReadFunc(0x0c,0x03,Read_BigFork_floor);  //�ⲿ���ƺ�Ĳ���
	SetReadFunc(0x0c,0x04,Read_SmallFork_floor); //�ⲿ�������ƺ�Ĳ���
	
	SetWriteFunc(0x0d,0x00,Jump_To_IAP);
	
}



/*
	motor:								���    				BigFork��SmallFork
	MoveMode��						����ģʽ				velocityMode��positionMode��homeMode
	targetPosition				Ŀ��λ��				
	profileV							�����ٶ�
	controlWord						������
*/
u8 LiftMove(struct LiftControl_Info para)
{
	if(para.motor  == BigFork)
	{
		modeOfOperation_6060[0] = 0x01;
		targetPosition_607A[0] = para.targetPosition;
		profileVelocity_6081[0] = para.profileV ;
		controlWord_6040[0] = 0x0f;
		BigForkTIM(0) = -1;
	}
	if(para.motor  == SmallFork)
	{
		modeOfOperation_6060[1] = 0x01;
		targetPosition_607A[1] = para.targetPosition;
		profileVelocity_6081[1] = para.profileV ;
		controlWord_6040[1] = 0x0f;
		SmallForkTIM(0) = -1;
	}
	return 0;
}


void ScanLiftControlSoftTIm(void)
{
	if(BigForkTIM(1) == -1)
	{
		controlWord_6040[0] = 0x3f;
		BigForkTIM(0) = 0;
	}
	if(SmallForkTIM(1) == -1)
	{
		controlWord_6040[1] = 0x3f;
		SmallForkTIM(0) = 0;
	}
	
	if(HomeTIM_BigFork(1) == -1)
	{
		controlWord_6040[0] = 0x1f;
		HomeTIM_BigFork(0) = 0;
	}
	if(HomeTIM_SmallFork(1) == -1)
	{
		controlWord_6040[1] = 0x1f;
		HomeTIM_SmallFork(0) = 0;
	}
}


void ScanSteptStartUp(void)
{
	if(steptBigFork_Info.startUp == 0xff)
	{
		StepMotor_PWM(BigFork_Num);

	}
	if(steptSmallFork_Info.startUp == 0xff)
	{
		StepMotor_PWM(SmallFork_Num);

	}
	if(steptRotate_Info.startUp == 0xff)
	{
		StepMotor_PWM(Rotate_Num);
	}
}


//u8 ScanRobotState(void)																		/* ɨ������˹���״̬ */
//{
//	if(RobotState.ControlState == 0xff)																								/* �������״̬��־λ�����ڿ����Ӵ���״̬����Դ״̬AGV״̬�ȱ�־���� */
//	{

//		
//	}
//}



u8 AllReturnHome(void)
{
	memset(&RobotFinishFlag,0,sizeof(RobotFinishFlag));
	memset(&RobotState,0,sizeof(RobotState));
	InitError();   // �������error��
	
	RobotState.AllRetunHome = 0xff;
	SteptBigFork_ReturnHome();																	/* ��������ԭ */
	return 0;
}	


u8 ScanReturnHome(void)
{
	if(RobotState.AllRetunHome == 0xff)
	{
		if(RobotFinishFlag.ReturnHome_State_finish[0] == 0xff)					/* ����ԭ��� */
		{
			
			RobotFinishFlag.ReturnHome_State_finish[0] = 0;								/* С���ԭ */
			delay_us(500);
			SteptSmallFork_ReturnHome();			
		}
		
		if(RobotFinishFlag.ReturnHome_State_finish[1] == 0xff)						
		{
			RobotFinishFlag.ReturnHome_State_finish[1] = 0;		
			delay_us(500);
			HomeMove_Lift(BigFork);																					/* ���������ԭ */
			HomeMove_Lift(SmallFork);																					/* С��������ԭ */
			
		}
		
//		if(RobotFinishFlag.ReturnHome_State_finish[3] == 0xff)
//		{
//			RobotFinishFlag.ReturnHome_State_finish[3] = 0;		
//			HomeMove_Lift(SmallFork);																					/* С��������ԭ */
//		}
		
		if((RobotFinishFlag.ReturnHome_State_finish[3] == 0xff) && (RobotFinishFlag.ReturnHome_State_finish[4] == 0xff))
		{
			RobotFinishFlag.ReturnHome_State_finish[3] = 0;
			RobotFinishFlag.ReturnHome_State_finish[4] = 0;
			Rotate_ReturnHome();																						/* ��ת��ԭ */			
		}
		if(RobotFinishFlag.ReturnHome_State_finish[2] == 0xff)
		{					
			RobotFinishFlag.ReturnHome_State_finish[2] = 0;		
			delay_us(500);
			Close_LobotSerialServo();
			RobotFinishFlag.AllReturnHome_Finish = 0xff;											/* ���л�ԭ��� */
			RobotState.AllRetunHome = 0;
		}
	}
	
	
	return 0;
}	



u8 ScanFinishFlag(void)
{
	if((RobotFinishFlag.AllReturnHome_Finish == 0xff) && (RobotState.ControlState == 0xff))			/* �ѻ�ԭ�㣬�����յ�������С��ץ��ָ�� */
	{
		if(LiftAllAction.TakePut == 0xff)							/* ȡ����״̬ */
		{	
			/* -----------------------------��һ��start--------------------------------------------- */
			if(RobotState.TakeStept1 == 0xff)
			{
				if((RobotFinishFlag.Rotate_out_left_finish == 0xff) || (RobotFinishFlag.Rotate_out_right_finish == 0xff))
				{
					RobotFinishFlag.Rotate_out_left_finish = 0;
					RobotFinishFlag.Rotate_out_right_finish = 0;
					LiftMoveToBin(LiftAllAction.Bin_Floor);
				}
				if((RobotFinishFlag.LiftBigFork_PositionArrive == 0xff) && (RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff))
				{
					RobotFinishFlag.LiftBigFork_PositionArrive = 0;
					RobotFinishFlag.LiftSmallFork_PositionArrive = 0;
					
					// �жϴ���Ƿ���Ҫ������
					if (LiftAllAction.Bin_Floor == 8)
					{
						RobotState.TakeStept1 = 0;
						RobotState.TakeStept_eight_floor_1 = 0xff;
						RobotState.NextSteptStart = 0xff;
					}
					else if (LiftAllAction.Bin_Floor == 50)
					{
						RobotState.TakeStept1 = 0;
						RobotState.TakeStept_Simple_2 = 0xff;
						RobotState.NextSteptStart = 0xff;
					}
					else if(((RobotState.GrabDir == 0xff) && (BigForkWorkDetect_Left == 1)) || ((RobotState.GrabDir == 0x00) && (BigForkWorkDetect_Right == 1)))
					{
						RobotState.TakeStept1 = 0;
						RobotState.TakeStept_Simple_2 = 0xff;
						RobotState.NextSteptStart = 0xff;
					}
					else
					{
						RobotState.TakeStept1 = 0;
						RobotState.TakeStept2 = 0xff;
						RobotState.NextSteptStart = 0xff;
					}
					
					
//					RobotState.TakeStept1 = 0;
//					RobotState.TakeStept2 = 0xff;
//					/* ��������������־λ����־����������һ���ĵ�һ������ */
//					RobotState.NextSteptStart = 0xff;
				}
			}
			/* ------------------------------------------------------��2��start----------------------------------------------------------------- */
			if(RobotState.TakeStept2 == 0xff)
			{
				if(RobotState.GrabDir == 0xff)																													/* ȡ������� */
				{
					if(RobotState.NextSteptStart == 0xff)
					{
						BigFork_Extend_Left();																	/* ���������� */
						SmallFork_Extend_Left();															/* С��������� */
						RobotState.NextSteptStart = 0;
					}
					if(RobotFinishFlag.BigFork_Extend_left_finish == 0xff)
					{
						RobotFinishFlag.BigFork_Extend_left_finish = 0;
						if(LiftAllAction.Bin_Floor == 0x01)
						{
							LiftBigFork_Move(LiftBigFork_Floor[LiftAllAction.Bin_Floor] ,LiftBigForkProfile_V_short);
							lift_fork_extra_action_flag = 1;
						}
						else
						{
							RobotFinishFlag.LiftBigFork_PositionArrive = 0xff;
							lift_fork_extra_action_flag = 1;
						}
					}
					if(RobotFinishFlag.SmallFork_Extend_finish == 0xff)
					{
						
						RobotFinishFlag.SmallFork_Extend_finish = 0;
						if(LiftAllAction.Bin_Floor == 0x01)
						{
							LiftSmallFork_Move(LiftSmallFork_Floor[LiftAllAction.Bin_Floor - 1] ,LiftSmallForkProfile_V_Long);
							lift_small_fork_extra_action_flag = 1;
						}
						else
						{
							RobotFinishFlag.LiftSmallFork_PositionArrive = 0xff;
							lift_small_fork_extra_action_flag = 1;
						}
					}
					if ((RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff )&& (RobotFinishFlag.LiftBigFork_PositionArrive == 0xff ) && (lift_small_fork_extra_action_flag == 1) && (lift_fork_extra_action_flag == 1))
					{
						RobotFinishFlag.LiftBigFork_PositionArrive = 0;
						RobotFinishFlag.LiftSmallFork_PositionArrive = 0;
						lift_small_fork_extra_action_flag = 0;
						ServoMove_Smallfork56_Out();
						lift_fork_extra_action_flag = 0;
						ServoMove_Bigfork12_L_Out();
					}
					
					if((RobotFinishFlag.BigForkSGear_LeftOut12_finish == 0xff) &&(RobotFinishFlag.SmallForkSGear_Out56_finish == 0xff))
					{
						RobotFinishFlag.BigForkSGear_LeftOut12_finish = 0;
						RobotFinishFlag.SmallForkSGear_Out56_finish = 0;
						RobotState.TakeStept2 = 0;
						RobotState.TakeStept3 = 0xff;
						/* ��������������־λ����־����������һ���ĵ�һ������ */
						RobotState.NextSteptStart = 0xff;
					}
				}	
				
				if(RobotState.GrabDir == 0)
				{
					if(RobotState.NextSteptStart == 0xff)
					{
						BigFork_Extend_Right();																	/* ���������� */
						SmallFork_Extend_Left();															/* С����� */
						RobotState.NextSteptStart = 0;
					}			
					if(RobotFinishFlag.BigFork_Extend_right_finish == 0xff)
					{
						RobotFinishFlag.BigFork_Extend_right_finish = 0;
						if(LiftAllAction.Bin_Floor == 0x01)
						{
							LiftBigFork_Move(LiftBigFork_Floor[LiftAllAction.Bin_Floor] ,LiftBigForkProfile_V_short);
							lift_fork_extra_action_flag = 1;
						}
						else
						{
							RobotFinishFlag.LiftBigFork_PositionArrive = 0xff;
							lift_fork_extra_action_flag = 1;
						}
					}
					if(RobotFinishFlag.SmallFork_Extend_finish == 0xff)
					{
						RobotFinishFlag.SmallFork_Extend_finish = 0;
						if(LiftAllAction.Bin_Floor == 0x01)
						{
							LiftSmallFork_Move(LiftSmallFork_Floor[LiftAllAction.Bin_Floor - 1] ,LiftSmallForkProfile_V_Long);
							lift_small_fork_extra_action_flag = 1;
						}
						else
						{
							RobotFinishFlag.LiftSmallFork_PositionArrive = 0xff;
							lift_small_fork_extra_action_flag = 1;
						}
					}
					if ((RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff )&& (RobotFinishFlag.LiftBigFork_PositionArrive == 0xff ) && (lift_small_fork_extra_action_flag == 1) && (lift_fork_extra_action_flag == 1))
					{
						RobotFinishFlag.LiftBigFork_PositionArrive = 0;
						RobotFinishFlag.LiftSmallFork_PositionArrive = 0;
						lift_small_fork_extra_action_flag = 0;
						ServoMove_Smallfork56_Out();
						lift_fork_extra_action_flag = 0;
						ServoMove_Bigfork34_R_Out();
					}
						
					if((RobotFinishFlag.SmallForkSGear_Out56_finish == 0xff) && (RobotFinishFlag.BigForkSGear_rightOut34_finish == 0xff))
					{
						RobotFinishFlag.BigForkSGear_rightOut34_finish = 0;
						RobotFinishFlag.SmallForkSGear_Out56_finish = 0;
						RobotState.TakeStept2 = 0;
						RobotState.TakeStept3 = 0xff;
						/* ��������������־λ����־����������һ���ĵ�һ������ */
						RobotState.NextSteptStart = 0xff;/* ��С��̧�� */		
					}
				}				
			}
			/* ------------------------------------------------------��3��start----------------------------------------------------------------- */
			if(RobotState.TakeStept3 == 0xff)
			{
				if(RobotState.NextSteptStart == 0xff)
				{
					RobotState.NextSteptStart = 0;
					if (LiftAllAction.Bin_Floor == 0x01)
					{
						LiftBigFork_Move(positionActualValue_6063[0] + LiftBigForkADD + AGVHigh_Big,LiftBigForkProfile_V_short);
//							LiftSmallFork_Move(positionActualValue_6063[1] + LiftSmallForkADD + AGVHigh_Small,LiftSmallForkProfile_V_short);
					}
					else if (LiftAllAction.Bin_Floor == 0x07)
					{
						LiftBigFork_Move(LiftBigFork_Floor[8], LiftBigForkProfile_V_short);
						LiftSmallFork_Move(positionActualValue_6063[1] + LiftSmallForkADD,LiftSmallForkProfile_V_short);
					}
					else
					{
						LiftBigFork_Move(positionActualValue_6063[0] + LiftBigForkADD,LiftBigForkProfile_V_short);
						LiftSmallFork_Move(positionActualValue_6063[1] + LiftSmallForkADD,LiftSmallForkProfile_V_short);
					}/* ��С��̧�� */		
				}
				if(RobotFinishFlag.LiftBigFork_PositionArrive == 0xff)
				{
					RobotFinishFlag.LiftBigFork_PositionArrive = 0;											/* ������λ�õ����־ */
					if(LiftAllAction.Bin_Floor == 0x01)
					{
						lift_small_fork_special_flag = 0;
						LiftSmallFork_Move(positionActualValue_6063[1] + LiftSmallForkADD + AGVHigh_Small,LiftSmallForkProfile_V_short);
					}
				}
				
				if(RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff)
				{
					RobotFinishFlag.LiftSmallFork_PositionArrive = 0;
					RobotState.TakeStept3 = 0;
					RobotState.TakeStept4 = 0xff;
					/* ��������������־λ����־����������һ���ĵ�һ������ */
					RobotState.NextSteptStart = 0xff;
				}				
			}
			
			/*-----------------------------------------ֻ��С�涯����1����8��start----------------------------------------------------------------*/
			if(RobotState.TakeStept_eight_floor_1 == 0xff)
			{
				if(RobotState.NextSteptStart == 0xff)
				{
					LiftSmallFork_Move(LiftSmallFork_Floor[LiftAllAction.Bin_Floor-1],LiftSmallForkProfile_V_short); //���ε�������
					RobotState.NextSteptStart = 0;
				}
				if(RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff)
				{
					RobotFinishFlag.LiftSmallFork_PositionArrive = 0;
					SmallFork_Extend_Left();															/* С��������� */
				}
				if(RobotFinishFlag.SmallFork_Extend_finish == 0xff)
				{
					RobotFinishFlag.SmallFork_Extend_finish = 0;
					ServoMove_Smallfork56_Out();
				}
				if(RobotFinishFlag.SmallForkSGear_Out56_finish == 0xff)
				{
					RobotFinishFlag.SmallForkSGear_Out56_finish = 0;
					RobotState.NextSteptStart = 0xff;
					RobotState.TakeStept_eight_floor_1 = 0;
					RobotState.TakeStept_eight_floor_2 = 0xff;
				}
			}
			/*-----------------------------------------ֻ��С�涯����2��. ��8��start----------------------------------------------------------------*/
			if(RobotState.TakeStept_eight_floor_2 == 0xff)
			{
				if(RobotState.NextSteptStart == 0xff)
				{
					LiftSmallFork_Move(LiftSmallFork_Floor[8], LiftSmallForkProfile_V_short);
					RobotState.NextSteptStart = 0;
				}
				if(RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff)
				{
					RobotFinishFlag.LiftSmallFork_PositionArrive = 0;
					SmallFork_Shrink_Left();
				}	
				if(RobotFinishFlag.SmallFork_Shrink_finish == 0xff)									/* С��������� */
				{
					RobotFinishFlag.SmallFork_Shrink_finish = 0;
					RobotState.NextSteptStart = 0xff;
					RobotState.TakeStept_eight_floor_2 = 0;
					RobotState.TakeStept_eight_floor_3 = 0xff;
				}
			}
			/*-----------------------------------------ֻ��С�涯����3��. ��8��start----------------------------------------------------------------*/
			if(RobotState.TakeStept_eight_floor_3 == 0xff)
			{
				if(RobotState.NextSteptStart == 0xff)
				{
					LiftSmallFork_Move(LiftSmallFork_Floor[12], LiftSmallForkProfile_V_short);
					RobotState.NextSteptStart = 0;
				}
				if(RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff)
				{
					RobotFinishFlag.LiftSmallFork_PositionArrive = 0;
					RobotState.NextSteptStart = 0xff;
					RobotState.TakeStept_eight_floor_3 = 0;
					RobotState.TakeStept6 = 0xff;
				}
			}
			
			/*-----------------------------------------ֻ��С�涯����2��, 1-7��start----------------------------------------------------------------*/
			if(RobotState.TakeStept_Simple_2 == 0xff)
			{
				if(RobotState.NextSteptStart == 0xff)
				{
					SmallFork_Extend_Left();
					RobotState.NextSteptStart = 0;
				}
				if(RobotFinishFlag.SmallFork_Extend_finish == 0xff)
				{
					
					RobotFinishFlag.SmallFork_Extend_finish = 0;
					if(LiftAllAction.Bin_Floor == 0x01)
					{
						LiftSmallFork_Move(LiftSmallFork_Floor[LiftAllAction.Bin_Floor - 1] ,LiftSmallForkProfile_V_Long);
						lift_small_fork_extra_action_flag = 1;
					}
					else
					{
						RobotFinishFlag.LiftSmallFork_PositionArrive = 0xff;
						lift_small_fork_extra_action_flag = 1;
					}
				}
				if ((RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff )&& (lift_small_fork_extra_action_flag == 1))
				{
					RobotFinishFlag.LiftSmallFork_PositionArrive = 0;
					lift_small_fork_extra_action_flag = 0;
					ServoMove_Smallfork56_Out();
				}
				
				if(RobotFinishFlag.SmallForkSGear_Out56_finish == 0xff)
				{
					if (LiftAllAction.Bin_Floor == 0x01)
					{
						LiftBigFork_Move(positionActualValue_6063[0] + LiftBigForkADD + AGVHigh_Big,LiftBigForkProfile_V_short);
						LiftSmallFork_Move(positionActualValue_6063[1] + LiftSmallForkADD + AGVHigh_Small,LiftSmallForkProfile_V_short);
					}
					else if (LiftAllAction.Bin_Floor == 0x07)
					{
						LiftBigFork_Move(LiftBigFork_Floor[8], LiftBigForkProfile_V_short);
						LiftSmallFork_Move(positionActualValue_6063[1] + LiftSmallForkADD,LiftSmallForkProfile_V_short);
					}
					else if(LiftAllAction.Bin_Floor == 50) //�����
					{
						if(positionActualValue_6063[0] < (LiftBigFork_Floor[12] + LiftBigForkADD + InboundOutboundADD_Big))
						{
							LiftBigFork_Move(LiftBigFork_Floor[12] + LiftBigForkADD + InboundOutboundADD_Big,LiftBigForkProfile_V_Long);
						}
						else
						{
							RobotFinishFlag.LiftBigFork_PositionArrive = 0xff;
						}
						LiftSmallFork_Move(positionActualValue_6063[1] + LiftSmallForkADD + InboundOutboundADD_Small,LiftSmallForkProfile_V_short);
					}
					else
					{
						LiftBigFork_Move(positionActualValue_6063[0] + LiftBigForkADD,LiftBigForkProfile_V_short);
						LiftSmallFork_Move(positionActualValue_6063[1] + LiftSmallForkADD,LiftSmallForkProfile_V_short);
					} /* ��С��һ��̧�� */
					RobotFinishFlag.SmallForkSGear_Out56_finish = 0;
				}
				
				
				if((RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff)  && (RobotFinishFlag.LiftBigFork_PositionArrive == 0xff))
				{
					RobotFinishFlag.LiftSmallFork_PositionArrive = 0;
					RobotFinishFlag.LiftBigFork_PositionArrive = 0;											/* ������λ�õ����־ */
					SmallFork_Shrink_Left();
				}	
				if(RobotFinishFlag.SmallFork_Shrink_finish == 0xff)									/* С��������� */
				{
					RobotFinishFlag.SmallFork_Shrink_finish = 0;
					RobotState.NextSteptStart = 0xff;
					RobotState.TakeStept_Simple_2 = 0;
					RobotState.TakeStept6 = 0xff;
				}	
			}

			/*-----------------------------------------��4��start----------------------------------------------------------------*/
			if(RobotState.TakeStept4 == 0xff)																	/* ��4��				*/
			{				
				if(RobotState.GrabDir == 0xff)																				/* ȡ������� */
				{
					if(RobotState.NextSteptStart == 0xff)
					{
						RobotState.NextSteptStart = 0;
						SmallFork_Shrink_Left();																					/* С���������� */
					}
					
					if(RobotFinishFlag.SmallFork_Shrink_finish == 0xff)		
					{
						RobotFinishFlag.SmallFork_Shrink_finish = 0;
						if(LiftAllAction.Bin_Floor == 0x01)
						{							
							LiftSmallFork_Move(LiftSmallFork_Floor[9],LiftSmallForkProfile_V_short);				/* ���ץȡ������Ϊ��1�㣬С�����е���Ͷ� */
							LiftBigFork_Move(LiftBigFork_Floor[LiftAllAction.Bin_Floor - 1],LiftBigForkProfile_V_short);		/* ����·����� */
						}
						else
						{
							LiftSmallFork_Move(LiftSmallFork_Floor[LiftAllAction.Bin_Floor-2],LiftSmallForkProfile_V_short);
							LiftBigFork_Move(LiftBigFork_Floor[LiftAllAction.Bin_Floor - 1],LiftBigForkProfile_V_short);		/* ����·����� */
						}
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
						
						if(LiftAllAction.Bin_Floor == 0x01)
						{							
							LiftSmallFork_Move(LiftSmallFork_Floor[9],LiftSmallForkProfile_V_short);				/* ���ץȡ������Ϊ��1�㣬С�����е���Ͷ� */
							LiftBigFork_Move(LiftBigFork_Floor[LiftAllAction.Bin_Floor - 1],LiftBigForkProfile_V_short);		/* ����·����� */
						}
						else
						{
							LiftSmallFork_Move(LiftSmallFork_Floor[LiftAllAction.Bin_Floor-2],LiftSmallForkProfile_V_short);
							LiftBigFork_Move(LiftBigFork_Floor[LiftAllAction.Bin_Floor - 1],LiftBigForkProfile_V_short);		/* ����·����� */
						}	
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
						RobotState.NextSteptStart = 0xff;
						RobotState.TakeStept4 = 0;
						RobotState.TakeStept5 = 0xff;
					}
				}					
				if(RobotState.GrabDir == 0)
				{			
					if(RobotFinishFlag.BigForkSGear_rightBack34_finish == 0xff )
					{
						RobotState.NextSteptStart = 0;
						RobotFinishFlag.BigForkSGear_rightBack34_finish = 0;
						RobotState.NextSteptStart = 0xff;
						RobotState.TakeStept4 = 0;
						RobotState.TakeStept5 = 0xff;
					}
				}
			}
			/* --------------------------------��5��Start--------------------------------------------- */
			if(RobotState.TakeStept5 == 0xff)
			{
				if(RobotState.NextSteptStart == 0xff)
				{
					RobotState.NextSteptStart = 0;						
					if((LiftAllAction.Bin_Floor == 0x01) || (LiftAllAction.Bin_Floor == 0x02))
					{							
						LiftBigFork_Move(LiftBigFork_Floor[10], LiftBigForkProfile_V_Long);		/* Big fork lift up extra */
					}
					else
					{
						RobotFinishFlag.LiftBigFork_PositionArrive = 0xff;  /*�������治��Ҫ���������˶�*/
					}					
				}
				if(RobotState.GrabDir == 0xff)
				{			
					if (RobotFinishFlag.LiftBigFork_PositionArrive == 0xff)
					{
						RobotFinishFlag.LiftBigFork_PositionArrive = 0;
						BigFork_Shrink_Left();
					}
					if(RobotFinishFlag.BigFork_Shrink_left_finish == 0xff)
					{
						RobotFinishFlag.BigFork_Shrink_left_finish = 0;
						RobotState.NextSteptStart = 0xff;
						RobotState.TakeStept5 = 0;
						RobotState.TakeStept6 = 0xff;
					}
				}					
				if(RobotState.GrabDir == 0)
				{
					if (RobotFinishFlag.LiftBigFork_PositionArrive == 0xff)
					{
						RobotFinishFlag.LiftBigFork_PositionArrive = 0;
						BigFork_Shrink_Right();
					}
					if(RobotFinishFlag.BigFork_Shrink_right_finish == 0xff)
					{
						RobotFinishFlag.BigFork_Shrink_right_finish = 0;
						RobotState.NextSteptStart = 0xff;
						RobotState.TakeStept5 = 0;
						RobotState.TakeStept6 = 0xff;
					}
				}
			}
			
			/* --------------------------------��6��Start--------------------------------------------- */
			if(RobotState.TakeStept6 == 0xff)
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
					if (RobotState.GrabDir == 0)
					{
						Rotate_Right_Back();
					}
					else
					{
						Rotate_Left_Back();
					}
				}
				if (RobotState.GrabDir == 0xff)
				{
					if(RobotFinishFlag.Rotate_back_left_finish == 0xff)
					{
						RobotFinishFlag.Rotate_back_left_finish = 0;
						RobotState.NextSteptStart = 0xff;
						RobotState.TakeStept6 = 0;
						RobotState.TakeStept7 = 0xff;
						
					}
				}
				if (RobotState.GrabDir == 0)
				{
					if(RobotFinishFlag.Rotate_back_right_finish == 0xff)
					{
						RobotFinishFlag.Rotate_back_right_finish = 0;
						RobotState.NextSteptStart = 0xff;
						RobotState.TakeStept6 = 0;
						RobotState.TakeStept7 = 0xff;
					}
				}
			}
				
/* ------------------------------------------��7��start---------------------------------------------------------------- */
					
			if(RobotState.TakeStept7 == 0xff)
			{
				
				if(RobotState.NextSteptStart == 0xff)
				{
					RobotState.NextSteptStart = 0;
					/*�����õ����䵽5�����6��ʱ����Ҫ����ת��λ������������Ϊ���Ƕ�׺��޷���ת*/
					if ((LiftAllAction.BackPackFloor == 5) || (LiftAllAction.BackPackFloor == 6))
					{
						LiftSmallFork_Move(BackPack_Floor[LiftAllAction.BackPackFloor-1],LiftSmallForkProfile_V_short);			/* С����ٵ���¨λ�� */
					}
					else
					{
						RobotFinishFlag.LiftSmallFork_PositionArrive = 0xff;
					}
				}
				if(RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff)
				{
					RobotFinishFlag.LiftSmallFork_PositionArrive = 0;
					ServoMove_BackPack_Back();
				}
				if(RobotFinishFlag.BackPackGear_Back_finish == 0xff)      /* ��¨��������*/
				{
					RobotFinishFlag.BackPackGear_Back_finish = 0;
					SmallFork_Extend_Left();
				}
				if(RobotFinishFlag.SmallFork_Extend_finish == 0xff)				/* С�������� */
				{
					RobotFinishFlag.SmallFork_Extend_finish = 0;
					RobotState.NextSteptStart = 0xff;
					RobotState.TakeStept7 = 0;
					RobotState.TakeStept8 = 0xff;
				}	
			}
			
				
/* ------------------------------------------��8��start---------------------------------------------------------------- */
					
			if(RobotState.TakeStept8 == 0xff)
			{
				
				if(RobotState.NextSteptStart == 0xff)
				{
					RobotState.NextSteptStart = 0;
					LiftSmallFork_Move(BackPack_Floor_PutDown[LiftAllAction.BackPackFloor -1],LiftSmallForkProfile_V_short);     /* �·����� */
				}
				
				if(RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff)
				{
					RobotFinishFlag.LiftSmallFork_PositionArrive = 0;
					ServoMove_Smallfork56_Back();																	/* �ջض�� */
				}
				if(RobotFinishFlag.SmallForkSGear_Back56_finish == 0xff)				/* С�����ջ���� */
				{
					RobotFinishFlag.SmallForkSGear_Back56_finish = 0;
					RobotState.NextSteptStart = 0xff;
					RobotState.TakeStept8 = 0;
					RobotState.TakeStept9 = 0xff;
				}
			}
			

/* ------------------------------------------��9��start---------------------------------------------------------------- */			
			if(RobotState.TakeStept9 == 0xff)
			{
				if(RobotState.NextSteptStart == 0xff)
				{
					RobotState.NextSteptStart = 0;
					if (LiftAllAction.BackPackFloor == 3)
					{
						LiftSmallFork_Move(BackPack_Floor[LiftAllAction.BackPackFloor -1],LiftSmallForkProfile_V_Long);     /* С��̧������ֹ��ײ */
					}
					else if(LiftAllAction.BackPackFloor == 5)
					{
						LiftSmallFork_Move(LiftSmallFork_Floor[12], LiftSmallForkProfile_V_Long);     /* С�����̧������ֹ��ײ */
					}
					else
					{
						RobotFinishFlag.LiftSmallFork_PositionArrive = 0xff;
					}
				}
				if(RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff)
				{
					RobotFinishFlag.LiftSmallFork_PositionArrive = 0;
					SmallFork_Shrink_Left();
				}
				if(RobotFinishFlag.SmallFork_Shrink_finish == 0xff)										/* С������ */
				{
					RobotFinishFlag.SmallFork_Shrink_finish = 0;
					RobotState.NextSteptStart = 0xff;
					RobotState.TakeStept9 = 0;
					RobotState.TakeStept10 = 0xff;
				}
			}
			/* ------------------------------------------��10��start---------------------------------------------------------------- */			
			if(RobotState.TakeStept10 == 0xff)
			{
				if(RobotState.NextSteptStart == 0xff)
				{
					RobotState.NextSteptStart = 0;
					if(LiftAllAction.BackPackFloor == 6)
					{
						LiftSmallFork_Move(LiftSmallFork_Floor[12],LiftSmallForkProfile_V_short);
					}
					else
					{
						RobotFinishFlag.LiftSmallFork_PositionArrive = 0xff;
					}
				}
				if(RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff)
				{
					RobotFinishFlag.LiftSmallFork_PositionArrive = 0;
					RobotState.TakeStept10 = 0;															
					RobotState.ControlState = 0;																				/* �������ģʽ */
					memset(&LiftAllAction,0,sizeof(LiftAllAction));
					memset(&RobotFinishFlag,0,sizeof(RobotFinishFlag));
					memset(&RobotState,0,sizeof(RobotState));
					RobotFinishFlag.AllReturnHome_Finish = 0xff; /*��ԭ�ɹ������*/  
				}
			}
		}
		/*--------------------------------------ж������---------------------------------------------------*/
		if(LiftAllAction.TakePut == 0)													/* ж�� */
		{
			/* -------------------------------ж���1��Start------------------------------------------------- */
			if(RobotState.PutStept1 == 0xff)
			{
				if((RobotFinishFlag.LiftBigFork_PositionArrive == 0xff) && (RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff) && (lift_small_fork_extra_action_flag == 0))
				{
					RobotFinishFlag.LiftBigFork_PositionArrive = 0;
					RobotFinishFlag.LiftSmallFork_PositionArrive = 0;
					if((LiftAllAction.BackPackFloor ==5) || (LiftAllAction.BackPackFloor == 6))
					{
						LiftSmallFork_Move(BackPack_Floor_PutDown[5],LiftSmallForkProfile_V_short);			/* С����ٵ���¨λ�� */
						lift_small_fork_extra_action_flag = 1;
					}
					else
					{
						lift_small_fork_extra_action_flag = 1;
						RobotFinishFlag.LiftSmallFork_PositionArrive = 0xff;
					}
				}
				if((lift_small_fork_extra_action_flag == 1) && (RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff))
				{
					lift_small_fork_extra_action_flag = 0;
					RobotFinishFlag.LiftSmallFork_PositionArrive = 0;
					// �����ת����һأ��������պ��ԭ��
					if(RobotState.GrabDir == 0xff)
					{
						Rotate_Left_Back();
					}
					if(RobotState.GrabDir == 0)
					{
						Rotate_Right_Back();
					}

				}
				if(RobotState.GrabDir == 0xff)
				{
					if(RobotFinishFlag.Rotate_back_left_finish == 0xff)
					{
						RobotFinishFlag.Rotate_back_left_finish = 0;
						RobotState.PutStept1 = 0;
						RobotState.PutStept2 = 0xff;
						/* ��������������־λ����־����������һ���ĵ�һ������ */
						RobotState.NextSteptStart = 0xff;
					}
				}
				if(RobotState.GrabDir == 0)
				{
					if(RobotFinishFlag.Rotate_back_right_finish == 0xff)
					{
						RobotFinishFlag.Rotate_back_right_finish = 0;
						RobotState.PutStept1 = 0;
						RobotState.PutStept2 = 0xff;
						/* ��������������־λ����־����������һ���ĵ�һ������ */
						RobotState.NextSteptStart = 0xff;
					}
				}
			}	
				/*-------------------------------------��2��Start--------------------------------------------------------------*/		
			if(RobotState.PutStept2 == 0xff)
			{
				if(RobotState.NextSteptStart == 0xff)
				{
					RobotState.NextSteptStart = 0;
					ServoMove_BackPack_Back();
				}
				if(RobotFinishFlag.BackPackGear_Back_finish == 0xff)   //��¨�����
				{
					RobotFinishFlag.BackPackGear_Back_finish = 0;
					SmallFork_Extend_Left();
				}
				
				if(RobotFinishFlag.SmallFork_Extend_finish == 0xff)
				{
					RobotFinishFlag.SmallFork_Extend_finish = 0;
					if (LiftAllAction.BackPackFloor == 3)
					{
						LiftSmallFork_Move(BackPack_Floor_PutDown[LiftAllAction.BackPackFloor -1], LiftSmallForkProfile_V_Long);				/* ���ﱳ¨�·�λ�� */
					}
					else if(LiftAllAction.BackPackFloor == 5)
					{
						LiftSmallFork_Move(BackPack_Floor_PutDown[LiftAllAction.BackPackFloor -1], LiftSmallForkProfile_V_short);				/* ���ﱳ¨�·�λ�� */
					}
					else
					{
						RobotFinishFlag.LiftSmallFork_PositionArrive = 0xff;
					}
				}
				if(RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff)
				{
					RobotFinishFlag.LiftSmallFork_PositionArrive = 0;
					ServoMove_Smallfork56_Out();
				}
				if(RobotFinishFlag.SmallForkSGear_Out56_finish == 0xff)
				{
					RobotFinishFlag.SmallForkSGear_Out56_finish = 0;
					RobotState.PutStept2 = 0;
					RobotState.PutStept3 = 0xff;
					RobotState.NextSteptStart = 0xff;
				}
			}
/*-------------------------------------��3��Start--------------------------------------------------------------*/		
			if(RobotState.PutStept3 == 0xff)
			{
				if(RobotState.NextSteptStart == 0xff)
				{
					RobotState.NextSteptStart = 0;
					LiftSmallFork_Move(BackPack_Floor[LiftAllAction.BackPackFloor -1],LiftSmallForkProfile_V_short);				/* ���ﱳ¨̧��λ�� */
				}

				if((RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff) && (lift_small_fork_extra_action_flag == 0))
				{
					RobotFinishFlag.LiftSmallFork_PositionArrive = 0;
					SmallFork_Shrink_Left();																			/* �õ����䣬�ջ�С�� */
				}
				
				if(RobotFinishFlag.SmallFork_Shrink_finish == 0xff)
				{
					RobotFinishFlag.SmallFork_Shrink_finish = 0;
					if((LiftAllAction.BackPackFloor ==5) || (LiftAllAction.BackPackFloor == 6))  /*С����Ҫ�ȵ���������룬�ٽ�����ת�Ȳ���*/
					{
						LiftSmallFork_Move(LiftSmallFork_Floor[12],LiftSmallForkProfile_V_short);			/* С����ٵ���¨λ�� */
						lift_small_fork_extra_action_flag = 1;
					}
					else
					{
						lift_small_fork_extra_action_flag = 1;
						RobotFinishFlag.LiftSmallFork_PositionArrive = 0xff;
					}
				}
				if((lift_small_fork_extra_action_flag == 1) && (RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff))
				{
					lift_small_fork_extra_action_flag = 0;
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
							RobotState.PutStept3 = 0;
							RobotState.PutStept4 = 0xff;
							RobotState.NextSteptStart = 0xff;
						}
					}
				if(RobotState.GrabDir == 0)
				{
					if(RobotFinishFlag.Rotate_out_right_finish == 0xff)
					{
						RobotFinishFlag.Rotate_out_right_finish = 0;
						RobotState.PutStept3 = 0;
						RobotState.PutStept4 = 0xff;
						RobotState.NextSteptStart = 0xff;
					}
				}
			}	

/*------------------------------------��4��Start-----------------------------------------------------*/			
			if(RobotState.PutStept4 == 0xff)
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
					if (LiftAllAction.Bin_Floor == 7)
					{
						LiftSmallFork_Move(LiftSmallFork_Floor[LiftAllAction.Bin_Floor-1] + LiftSmallForkADD,LiftSmallForkProfile_V_short);	 /* С��������˶���һ��λ�� */
						lift_small_fork_extra_action_flag = 1;
					}
					else if(LiftAllAction.Bin_Floor == 8)
					{
						LiftSmallFork_Move(LiftSmallFork_Floor[8],LiftSmallForkProfile_V_short);	 /* С��������˶���һ��λ�� */
						lift_small_fork_extra_action_flag = 1;
					}
					else
					{
						RobotFinishFlag.LiftSmallFork_PositionArrive = 0xff;
						lift_small_fork_extra_action_flag = 1;
					}
				}
				if ((RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff) && (lift_small_fork_extra_action_flag == 1))
				{
					// �жϴ���Ƿ���Ҫ������
					if(LiftAllAction.Bin_Floor == 8)
					{
						RobotState.PutStept4 = 0;
						RobotState.PutStept_eight_floor_1 = 0xff;
						RobotState.NextSteptStart = 0xff;
						lift_small_fork_extra_action_flag = 0;
						RobotFinishFlag.LiftSmallFork_PositionArrive = 0;
					}
					else if(LiftAllAction.Bin_Floor == 50)  //���⹤��̨
					{
						if(((RobotState.GrabDir == 0xff) && (BigForkWorkDetect_Place_Left == 1)) || ((RobotState.GrabDir == 0x00) && (BigForkWorkDetect_Place_Right == 1)))
						{
							RobotState.PutStept4 = 0;
							RobotState.PutStept_Simple_5 = 0xff;
							RobotState.NextSteptStart = 0xff;
							lift_small_fork_extra_action_flag = 0;
							RobotFinishFlag.LiftSmallFork_PositionArrive = 0;
						}

					}
					else if ((LiftAllAction.Bin_Floor == 1) && (((RobotState.GrabDir == 0xff) && (BigForkWorkDetectFirstFloor_Left == 1)) || ((RobotState.GrabDir == 0x00) && (BigForkWorkDetectFirstFloor_Right == 1))))
					{
						RobotState.PutStept4 = 0;
						RobotState.PutStept_Simple_5 = 0xff;
						RobotState.NextSteptStart = 0xff;
						lift_small_fork_extra_action_flag = 0;
						RobotFinishFlag.LiftSmallFork_PositionArrive = 0;
					}
					else if((LiftAllAction.Bin_Floor != 1) &&(((RobotState.GrabDir == 0xff) && (BigForkWorkDetect_Place_Left == 1)) || ((RobotState.GrabDir == 0x00) && (BigForkWorkDetect_Place_Right == 1))))
					{
						RobotState.PutStept4 = 0;
						RobotState.PutStept_Simple_5 = 0xff;
						RobotState.NextSteptStart = 0xff;
						lift_small_fork_extra_action_flag = 0;
						RobotFinishFlag.LiftSmallFork_PositionArrive = 0;
					}
					else
					{
						RobotState.PutStept4 = 0;
						RobotState.PutStept5 = 0xff;
						RobotState.NextSteptStart = 0xff;
						lift_small_fork_extra_action_flag = 0;
						RobotFinishFlag.LiftSmallFork_PositionArrive = 0;						
					}
				}
			}

/*---------------------------��5��Start--------------------------------------------*/			
			if(RobotState.PutStept5 == 0xff)
			{
				if(RobotState.NextSteptStart == 0xff)
				{
					RobotState.NextSteptStart = 0;
					if((LiftAllAction.Bin_Floor == 1) || (LiftAllAction.Bin_Floor == 2))
					{
						LiftSmallFork_Move(LiftSmallFork_Floor[9], LiftSmallForkProfile_V_short);
					}
					else
					{
						LiftSmallFork_Move(LiftSmallFork_Floor[LiftAllAction.Bin_Floor-2], LiftSmallForkProfile_V_Long);
					}
				}
				if(RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff)
				{
					RobotFinishFlag.LiftSmallFork_PositionArrive = 0;
					if(RobotState.GrabDir == 0xff)
					{
						BigFork_Extend_Left();
					}
					if(RobotState.GrabDir == 0)
					{
						BigFork_Extend_Right();
					}
				}
				if(RobotState.GrabDir == 0xff)
				{
					if(RobotFinishFlag.BigFork_Extend_left_finish == 0xff)
					{
						RobotFinishFlag.BigFork_Extend_left_finish = 0;
						RobotState.PutStept5 = 0;
						RobotState.PutStept6 = 0xff;
						RobotState.NextSteptStart = 0xff;	
					
					}
				}
				if(RobotState.GrabDir == 0)
				{		
					if(RobotFinishFlag.BigFork_Extend_right_finish == 0xff)
					{
						RobotFinishFlag.BigFork_Extend_right_finish = 0;
						RobotState.PutStept5 = 0;
						RobotState.PutStept6 = 0xff;
						RobotState.NextSteptStart = 0xff;
					}							
				}	
			}

/*---------------------------��治�����򵥵�1�� ��8��Start����8�㣩--------------------------------------------*/	
			if(RobotState.PutStept_eight_floor_1 == 0xff)
			{
				if(RobotState.NextSteptStart == 0xff)
				{
					RobotState.NextSteptStart = 0;
					SmallFork_Extend_Left();																				/* ���С�� */		
				}
				if(RobotFinishFlag.SmallFork_Extend_finish ==0xff)
				{
					RobotFinishFlag.SmallFork_Extend_finish = 0;
					RobotState.PutStept_eight_floor_1 = 0;
					RobotState.PutStept_eight_floor_2 = 0xff;
					RobotState.NextSteptStart = 0xff;
				}
			}
/*---------------------------��治�����򵥵�2�� ��8��Start����8�㣩--------------------------------------------*/	
			if(RobotState.PutStept_eight_floor_2 == 0xff)
			{
				if(RobotState.NextSteptStart == 0xff)
				{
					RobotState.NextSteptStart = 0;
					LiftSmallFork_Move(LiftSmallFork_Floor[LiftAllAction.Bin_Floor - 1],LiftSmallForkProfile_V_short);
				}
				if(RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff)
				{
					RobotFinishFlag.LiftSmallFork_PositionArrive = 0;
					ServoMove_Smallfork56_Back();
				}
				
				if(RobotFinishFlag.SmallForkSGear_Back56_finish == 0xff)
				{
					RobotFinishFlag.SmallForkSGear_Back56_finish = 0;
					lift_fork_extra_action_flag = 0;
					SmallFork_Shrink_Left();
				}
				
				if(RobotFinishFlag.SmallFork_Shrink_finish == 0xff)
				{
					RobotFinishFlag.SmallFork_Shrink_finish = 0;
					RobotState.PutStept_eight_floor_2 = 0;
					RobotState.PutStept9 = 0xff;
					RobotState.NextSteptStart = 0xff;
				}
			}			
			
/*---------------------------��治�����򵥵�5��Start��1-7�㣩--------------------------------------------*/			
			if(RobotState.PutStept_Simple_5 == 0xff)
			{
				if(RobotState.NextSteptStart == 0xff)
				{
					RobotState.NextSteptStart = 0;
					SmallFork_Extend_Left();
				}
				if(RobotFinishFlag.SmallFork_Extend_finish ==0xff)
				{
					RobotFinishFlag.SmallFork_Extend_finish = 0;
					
					if(LiftAllAction.Bin_Floor == 50) //����
					{
						LiftSmallFork_Move(LiftSmallFork_Floor[14],LiftSmallForkProfile_V_short);
						lift_fork_action_flag = 1;
					}
					else
					{
						LiftSmallFork_Move(LiftSmallFork_Floor[LiftAllAction.Bin_Floor - 1],LiftSmallForkProfile_V_short);
						lift_fork_action_flag = 1;
					}
				}
				
				if((RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff) && (lift_fork_action_flag == 1))
				{
					RobotFinishFlag.LiftSmallFork_PositionArrive = 0;
					lift_fork_action_flag = 0;
					ServoMove_Smallfork56_Back();
				}
				
				if(RobotFinishFlag.SmallForkSGear_Back56_finish == 0xff)
				{
					RobotFinishFlag.SmallForkSGear_Back56_finish = 0;
					if(LiftAllAction.Bin_Floor == 0x01)
					{
						LiftSmallFork_Move(LiftSmallFork_Floor[10],LiftSmallForkProfile_V_short);
						lift_fork_extra_action_flag = 1;
					}
					else
					{
						RobotFinishFlag.LiftSmallFork_PositionArrive = 0xff;
						lift_fork_extra_action_flag = 1;
					}
				}
				if((RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff) && (lift_fork_extra_action_flag == 1))
				{
					RobotFinishFlag.LiftSmallFork_PositionArrive = 0;
					lift_fork_extra_action_flag = 0;
					SmallFork_Shrink_Left();
				}
				
				if(RobotFinishFlag.SmallFork_Shrink_finish == 0xff)
				{
					RobotFinishFlag.SmallFork_Shrink_finish = 0;
					RobotState.PutStept_Simple_5 = 0;
					RobotState.PutStept9 = 0xff;
					RobotState.NextSteptStart = 0xff;
				}
			}			
			
			
/*---------------------------��6��Start--------------------------------------------*/			
			if(RobotState.PutStept6 == 0xff)
			{
				if(RobotState.NextSteptStart == 0xff)
				{
					RobotState.NextSteptStart = 0;						
					LiftBigFork_Move(LiftBigFork_Floor[LiftAllAction.Bin_Floor - 1],LiftBigForkProfile_V_Long);		/* ���������ٴ��½� */
				}
				
				if(RobotState.GrabDir == 0xff)
				{
					if (RobotFinishFlag.LiftBigFork_PositionArrive == 0xff)
					{
						RobotFinishFlag.LiftBigFork_PositionArrive = 0;
						ServoMove_Bigfork12_L_Out();
					}
					if(RobotFinishFlag.BigForkSGear_LeftOut12_finish == 0xff) 
					{
						RobotState.PutStept6 = 0;
						RobotState.PutStept7 = 0xff;
						RobotState.NextSteptStart = 0xff;							
						RobotFinishFlag.BigForkSGear_LeftOut12_finish = 0;
					}
					
				}
				if(RobotState.GrabDir == 0)
				{		
					if (RobotFinishFlag.LiftBigFork_PositionArrive == 0xff)
					{
						RobotFinishFlag.LiftBigFork_PositionArrive = 0;
						ServoMove_Bigfork34_R_Out();
					}
					if(RobotFinishFlag.BigForkSGear_rightOut34_finish == 0xff)
					{
						RobotState.PutStept6 = 0;
						RobotState.PutStept7 = 0xff;
						RobotState.NextSteptStart = 0xff;
						RobotFinishFlag.BigForkSGear_rightOut34_finish = 0;
					}							
				}		
			}

/*---------------------------��7��Start--------------------------------------------*/
			if(RobotState.PutStept7 == 0xff)
			{
				if(RobotState.NextSteptStart == 0xff)
				{
					RobotState.NextSteptStart = 0;
					if(LiftAllAction.Bin_Floor == 0x01)
					{
						LiftBigFork_Move(LiftBigFork_Floor[LiftAllAction.Bin_Floor] + LiftBigForkADD + AGVHigh_Big,LiftBigForkProfile_V_short);
						LiftSmallFork_Move(LiftSmallFork_Floor[LiftAllAction.Bin_Floor - 1] + LiftSmallForkADD + AGVHigh_Small,LiftSmallForkProfile_V_short);
					}
					else if(LiftAllAction.Bin_Floor == 0x07)
					{
						LiftBigFork_Move(LiftBigFork_Floor[8], LiftBigForkProfile_V_short);
						LiftSmallFork_Move(LiftSmallFork_Floor[LiftAllAction.Bin_Floor - 1] + LiftSmallForkADD,LiftSmallForkProfile_V_short);
					}
					else
					{
						LiftBigFork_Move(LiftBigFork_Floor[LiftAllAction.Bin_Floor] + LiftBigForkADD,LiftBigForkProfile_V_short);
						LiftSmallFork_Move(LiftSmallFork_Floor[LiftAllAction.Bin_Floor - 1] + LiftSmallForkADD,LiftSmallForkProfile_V_short);
					}
				}
				
				if((RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff) && (RobotFinishFlag.LiftBigFork_PositionArrive == 0xff))
				{
					RobotFinishFlag.LiftSmallFork_PositionArrive = 0;
					RobotFinishFlag.LiftBigFork_PositionArrive = 0;
					SmallFork_Extend_Left();																							/* ���С�� */					
				}
				
				if(RobotFinishFlag.SmallFork_Extend_finish ==0xff)
				{
					RobotState.PutStept7 = 0;
					RobotState.PutStept8 = 0xff;
					RobotState.NextSteptStart = 0xff;
					RobotFinishFlag.SmallFork_Extend_finish = 0;
				}	
			}
/* --------------------------��8����ʼ----------------------------------------- */				
			if(RobotState.PutStept8 == 0xff)
			{
				if(RobotState.NextSteptStart == 0xff)
				{
					RobotState.NextSteptStart = 0;
					
					if(LiftAllAction.Bin_Floor == 0x01)
					{
						LiftSmallFork_Move(LiftSmallFork_Floor[9],LiftSmallForkProfile_V_short);
						lift_fork_action_flag = 1;
					}
					else
					{
						LiftSmallFork_Move(LiftSmallFork_Floor[LiftAllAction.Bin_Floor - 1],LiftSmallForkProfile_V_short);
						lift_fork_action_flag = 1;
					}
				}
				
				if((RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff) && (lift_fork_action_flag == 1))
				{
					RobotFinishFlag.LiftSmallFork_PositionArrive = 0;
					lift_fork_action_flag = 0;
					
					ServoMove_Smallfork56_Back();
				}
				
				if(RobotFinishFlag.SmallForkSGear_Back56_finish == 0xff)
				{
					RobotFinishFlag.SmallForkSGear_Back56_finish = 0;
					if(LiftAllAction.Bin_Floor == 0x01)
					{
						LiftSmallFork_Move(LiftSmallFork_Floor[10],LiftSmallForkProfile_V_short);
						lift_fork_extra_action_flag = 1;
					}
					else
					{
						RobotFinishFlag.LiftSmallFork_PositionArrive = 0xff;
						lift_fork_extra_action_flag = 1;
					}
				}
				
				if((RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff) && (lift_fork_extra_action_flag == 1))
				{
					RobotFinishFlag.LiftSmallFork_PositionArrive = 0;
					lift_fork_extra_action_flag = 0;
					SmallFork_Shrink_Left();
				}
				
				if(RobotState.GrabDir == 0xff)
				{
					if(RobotFinishFlag.SmallFork_Shrink_finish == 0xff)
					{
						RobotFinishFlag.SmallFork_Shrink_finish = 0;
						LiftBigFork_Move(LiftBigFork_Floor[LiftAllAction.Bin_Floor],LiftBigForkProfile_V_short);
						lift_fork_action_flag = 1;
					}
					if((RobotFinishFlag.LiftBigFork_PositionArrive ==0xff) && (lift_fork_action_flag == 1))
					{
						RobotFinishFlag.LiftBigFork_PositionArrive = 0;
						lift_fork_action_flag = 0;
						ServoMove_Bigfork12_L_Back();
					}
					
					if(RobotFinishFlag.BigForkSGear_LeftBack12_finish == 0xff)
					{
						RobotFinishFlag.BigForkSGear_LeftBack12_finish = 0;
						if(LiftAllAction.Bin_Floor == 0x01)
						{
							LiftBigFork_Move(LiftBigFork_Floor[10],LiftBigForkProfile_V_Long);
							lift_fork_extra_action_flag = 1;
						}
						else
						{
							RobotFinishFlag.LiftBigFork_PositionArrive = 0xff;
							lift_fork_extra_action_flag = 1;
						}
					}
					
					if ((RobotFinishFlag.LiftBigFork_PositionArrive == 0xff) && (lift_fork_extra_action_flag == 1))
					{
						lift_fork_extra_action_flag = 0;
						RobotFinishFlag.LiftBigFork_PositionArrive = 0;
						BigFork_Shrink_Left();
					}
					
					if(RobotFinishFlag.BigFork_Shrink_left_finish == 0xff)
					{
						RobotFinishFlag.BigFork_Shrink_left_finish = 0;
						RobotState.PutStept8 = 0;
						RobotState.PutStept9 = 0xff;
						RobotState.NextSteptStart = 0xff;
					}
				}
				if(RobotState.GrabDir == 0)
				{
					if(RobotFinishFlag.SmallFork_Shrink_finish == 0xff)
					{
						RobotFinishFlag.SmallFork_Shrink_finish = 0;
						LiftBigFork_Move(LiftBigFork_Floor[LiftAllAction.Bin_Floor],LiftBigForkProfile_V_short);
						lift_fork_action_flag = 1;
					}
					if((RobotFinishFlag.LiftBigFork_PositionArrive ==0xff) && (lift_fork_action_flag == 1))
					{
						lift_fork_action_flag = 0;
						RobotFinishFlag.LiftBigFork_PositionArrive = 0;
						ServoMove_Bigfork34_R_Back();
					}
					if(RobotFinishFlag.BigForkSGear_rightBack34_finish == 0xff)
					{
						RobotFinishFlag.BigForkSGear_rightBack34_finish = 0;
						if(LiftAllAction.Bin_Floor == 0x01)
						{
							LiftBigFork_Move(LiftBigFork_Floor[10],LiftBigForkProfile_V_Long);
							lift_fork_extra_action_flag = 1;
						}
						else
						{
							RobotFinishFlag.LiftBigFork_PositionArrive = 0xff;
							lift_fork_extra_action_flag = 1;
						}
					}
					if ((RobotFinishFlag.LiftBigFork_PositionArrive == 0xff) && (lift_fork_extra_action_flag == 1))
					{
						lift_fork_extra_action_flag = 0;
						RobotFinishFlag.LiftBigFork_PositionArrive = 0;
						BigFork_Shrink_Right();
					}
					if(RobotFinishFlag.BigFork_Shrink_right_finish == 0xff)
					{
						RobotFinishFlag.BigFork_Shrink_right_finish = 0;
						RobotState.PutStept8 = 0;
						RobotState.PutStept9 = 0xff;
						RobotState.NextSteptStart = 0xff;
					}
				}
			}
			/*------------------------��9������--------------------------------------------*/	
			
			if(RobotState.PutStept9 == 0xff)
			{
				if(RobotState.NextSteptStart == 0xff)
				{
					RobotState.NextSteptStart = 0;
					if(LiftAllAction.Bin_Floor == 1)
					{
						LiftMoveToBin(2);
					}
					else if(LiftAllAction.Bin_Floor == 8)
					{
						LiftSmallFork_Move(LiftSmallFork_Floor[12],LiftSmallForkProfile_V_short);
						RobotFinishFlag.LiftBigFork_PositionArrive = 0xff;
					}
					else
					{
						RobotFinishFlag.LiftBigFork_PositionArrive = 0xff;
						RobotFinishFlag.LiftSmallFork_PositionArrive = 0xff;
					}
				}
				if ((RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff) && (RobotFinishFlag.LiftBigFork_PositionArrive == 0xff))
				{
					RobotFinishFlag.LiftBigFork_PositionArrive = 0x00;
					RobotFinishFlag.LiftSmallFork_PositionArrive = 0x00;
					if(RobotState.GrabDir == 0xff)
					{
						Rotate_Left_Back();
					}
					if(RobotState.GrabDir == 0)
					{
						Rotate_Right_Back();
					}
				}
				if((RobotFinishFlag.Rotate_back_left_finish == 0xff) || (RobotFinishFlag.Rotate_back_right_finish == 0xff))
				{
					RobotFinishFlag.Rotate_back_right_finish = 0x00;
					RobotFinishFlag.Rotate_back_left_finish = 0x00;
					RobotState.PutStept9 = 0;
					RobotState.ControlState = 0;																				/* �������ģʽ */
					memset(&LiftAllAction,0,sizeof(LiftAllAction));
					memset(&RobotFinishFlag,0,sizeof(RobotFinishFlag));
					memset(&RobotState,0,sizeof(RobotState));
					RobotFinishFlag.AllReturnHome_Finish = 0xff; /*��ԭ�ɹ������*/  
				}
				
				
			}
			/* -----------------------------ж�����---------------------------------------------- */
	}
		if(LiftAllAction.Capture == 0xff)
		{
			if(RobotState.CaptureSetpt1 == 0xff)
			{
				if((RobotFinishFlag.Rotate_out_left_finish == 0xff) || (RobotFinishFlag.Rotate_out_right_finish == 0xff))
				{
					RobotFinishFlag.Rotate_out_left_finish = 0;
					RobotFinishFlag.Rotate_out_right_finish = 0;
					LiftMoveToBinCaptureImage(LiftAllAction.Bin_Floor);
				}
				if((RobotFinishFlag.LiftBigFork_PositionArrive == 0xff) && (RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff))
				{
					RobotFinishFlag.LiftBigFork_PositionArrive = 0;
					RobotFinishFlag.LiftSmallFork_PositionArrive = 0;
					RobotState.CaptureSetpt1 = 0;															
					RobotState.ControlState = 0;																				/* �������ģʽ */
					memset(&LiftAllAction,0,sizeof(LiftAllAction));
					memset(&RobotFinishFlag,0,sizeof(RobotFinishFlag));
					memset(&RobotState,0,sizeof(RobotState));
					RobotFinishFlag.AllReturnHome_Finish = 0xff; /*��ԭ�ɹ������*/ 
				}
			}
		}
		if(LiftAllAction.Capture == 0)
		{
			if(RobotState.MoveSetpt1 == 0xff)
			{
				if((RobotFinishFlag.LiftBigFork_PositionArrive == 0xff) && (RobotFinishFlag.LiftSmallFork_PositionArrive == 0xff))
				{
					RobotFinishFlag.LiftBigFork_PositionArrive = 0;
					RobotFinishFlag.LiftSmallFork_PositionArrive = 0;
					RobotState.MoveSetpt1 = 0;															
					RobotState.ControlState = 0;																				/* �������ģʽ */
					memset(&LiftAllAction,0,sizeof(LiftAllAction));
					memset(&RobotFinishFlag,0,sizeof(RobotFinishFlag));
					memset(&RobotState,0,sizeof(RobotState));
					RobotFinishFlag.AllReturnHome_Finish = 0xff; /*��ԭ�ɹ������*/ 
				}
			}
		}
	}
	return 0;
}

