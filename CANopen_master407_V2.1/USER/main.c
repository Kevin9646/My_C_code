#include "main.h"


/* ȫ�ֱ��� */

u16 DD1;
u16 DD2;
u16 DD3;
u16 DD4;
u16 DD5;
u16 DD6;
u8 error_flag = 0;

int main(void)
{ 
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����ϵͳ�ж����ȼ�����4
	SCB->VTOR = FLASH_BASE | 0x10000;  // �������г���ƫ��
	/* ms����ʱ���� */
	delay_init(168);    	//��ʼ����ʱ����
	Init_IAP_Flash_Flag(); //��ʼ����������IAP flag
	
	
	/* ��FLASH�ж�ȡ���������� */
	Robot_ConfigInit_FromFlash();
	
	/* canfestival���� */
	CAN1_Init(&Master_Data,500000);							/* canopenʹ�ã�can1 */
	TIMx_Configuration();													/* canfestival tim6 */

	/* ��ʼ������1��485 */
	USART1_Init(115200);                            /* ��λ������ͨѶ��ʼ�� */
	
	/* ��ʼ����λ��Э��RS485 */
	Contact_Init();
	USART2_Init(115200);														/* ���ͨѶ */
	
	USART3_Init(115200);                            /*�Ƕȴ�����ͨѶ*/
	
	/* ��ʼ��GPIO */
	gpio_Init();
	Sensor_Init();

	softTimer_Init(999, 83);		/* Timer�����ʱ����ʼ�� 1ms�ж�һ�� */


//	/* CANOPEN��ʼ�� */
	canopenDataInit();

	TIM_GPIO_Init();
	InitError();

	delay_ms(2000);
			// ������ѭ��
	TenMsTimer(0) = -1;
	FiveMsTimer(0) = -1;
	
	while(1)
	{
				//5msѭ��һ��
		if (FiveMsTimer(1) == -1)
		{
			FiveMsTimer(1) = 0;
			FiveMsTimer(3) = 0;
			/* �ŷ��������  START */
			ScanFinishFlag();												/* ɨ���߼����ƴ��� */
			ScanForkFinishFLag();										/* ɨ���С����ɱ�־ */
			Read_Rotate_Angle();                    /* �����Զ�ȡ��ת�Ƕ�*/

		}
		
		//10msѭ��һ��
		if (TenMsTimer(1) == -1)
		{
			TenMsTimer(1) = 0;
			TenMsTimer(3) = 0;
			Scan_LobotSerialServo();							/* ���������־ɨ�� */
				/* canopen���� start */		
			if(workStart_flag == 0xff)
			{
					unsigned char nodeID = 0x00;                   //�ڵ�ID
					workStart_flag = 0;
					Robot_ConfigInit_FromFlash();  /* ��ʼ��FLASH���� */
					
					setNodeId(&Master_Data, nodeID);
					setState(&Master_Data, Initialisation);				//�ڵ��ʼ��
					setState(&Master_Data, Pre_operational);	
					setState(&Master_Data, Operational);
				
				 targetPosition_607A[0]  = 0;
				 targetPosition_607A[1]  = 0;	
				 delay_ms(10);
					masterSendNMTstateChange(&Master_Data, 1, NMT_Start_Node);
					delay_ms(10);
					masterSendNMTstateChange(&Master_Data, 2, NMT_Start_Node);	
					delay_ms(10);


				
				 controlWord_6040[0] = 0x86;
				 controlWord_6040[1] = 0x86;
				
				 modeOfOperation_Init(0) = -1;	
				 memset(&RobotState,0,sizeof(RobotState)); 
				 InitError(); // �������error��
				 memset(&RobotFinishFlag,0,sizeof(RobotFinishFlag));
	//				RobotFinishFlag.AllReturnHome_Finish = 0xff;						//��ԭ��ɱ�־
			}
	/* canopen���� end */		
			ScanServoError();
			ScanStept();
			Scanf_Lift_message();										/* ����ԭ�㼫�޵�ɨ�� */
			ScanServoFinishFlag();									/* ɨ������ɱ�־ */
			ScanReturnHome();												/* ɨ���ԭ���״̬ */		

			if((errorWord2[0] | errorWord2[1] | errorWord1[1] | errorWord1[0]) != 0)				/* ������ */
			{
				RobotState.motorerror = 0xff;	
			}
		}
		if(modeOfOperation_Init(1) == -1)				/* canopen����Ϻ��ϵ�ɨ�� */
		{
			controlWord_6040[0]  = 0x0f;
			controlWord_6040[1]  = 0x0f;
			
			modeOfOperation_6060[0] = 0x01;
			modeOfOperation_6060[1] = 0x01;
			modeOfOperation_Init(0) = 0;
			RobotState.motorerror = 0;											/* �����־ */
		}
		
		ScanSteptStartUp();					/* �������������־ɨ�貢��ʼ */
		ScanLiftControlSoftTIm();						/* λ�ÿ������������ʱ��ɨ�� */

		
		/* ѭ��ɨ�貽����������ʱ�� */
	 SteptMotor_SoferTime_Big(BigFork_Num,TIM_BigFork);								/* ��������Ӽ���ɨ�� */
	 SteptMotor_SoferTime_Small(SmallFork_Num,TIM_SmallFork);
	 SteptMotor_SoferTime_Rotate(Rotate_Num,TIM_Rotate);
		
//		/* ѭ��ɨ��485������ʱ�� */
//		rs485_contral_lim();
		Sensor_Scan();													/* ������ɨ�� */
//		SmallForkAllowExtend();
//		BigForkAllowExtend();
//		ScanBigForkNearSmallFork();								/* ��С��ӽ������� */
		ScanSoftTimerState();										/* ɨ�������ʱ��  Ŀǰֻ�в�����ԭ�����������ʱ��ɨ����ڴ˺����� */	
	}
}






