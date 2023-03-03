#include "main.h"


/* 全局变量 */

u16 DD1;
u16 DD2;
u16 DD3;
u16 DD4;
u16 DD5;
u16 DD6;
u8 error_flag = 0;

int main(void)
{ 
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4
	SCB->VTOR = FLASH_BASE | 0x10000;  // 用来进行程序偏移
	/* ms级延时函数 */
	delay_init(168);    	//初始化延时函数
	Init_IAP_Flash_Flag(); //初始化程序升级IAP flag
	
	
	/* 从FLASH中读取机器人配置 */
	Robot_ConfigInit_FromFlash();
	
	/* canfestival驱动 */
	CAN1_Init(&Master_Data,500000);							/* canopen使用，can1 */
	TIMx_Configuration();													/* canfestival tim6 */

	/* 初始化串口1，485 */
	USART1_Init(115200);                            /* 上位机串口通讯初始化 */
	
	/* 初始化上位机协议RS485 */
	Contact_Init();
	USART2_Init(115200);														/* 舵机通讯 */
	
	USART3_Init(115200);                            /*角度传感器通讯*/
	
	/* 初始化GPIO */
	gpio_Init();
	Sensor_Init();

	softTimer_Init(999, 83);		/* Timer软件定时器初始化 1ms中断一次 */


//	/* CANOPEN初始化 */
	canopenDataInit();

	TIM_GPIO_Init();
	InitError();

	delay_ms(2000);
			// 启动主循环
	TenMsTimer(0) = -1;
	FiveMsTimer(0) = -1;
	
	while(1)
	{
				//5ms循环一次
		if (FiveMsTimer(1) == -1)
		{
			FiveMsTimer(1) = 0;
			FiveMsTimer(3) = 0;
			/* 伺服电机测试  START */
			ScanFinishFlag();												/* 扫描逻辑控制代码 */
			ScanForkFinishFLag();										/* 扫描大小叉完成标志 */
			Read_Rotate_Angle();                    /* 周期性读取旋转角度*/

		}
		
		//10ms循环一次
		if (TenMsTimer(1) == -1)
		{
			TenMsTimer(1) = 0;
			TenMsTimer(3) = 0;
			Scan_LobotSerialServo();							/* 舵机启动标志扫描 */
				/* canopen重启 start */		
			if(workStart_flag == 0xff)
			{
					unsigned char nodeID = 0x00;                   //节点ID
					workStart_flag = 0;
					Robot_ConfigInit_FromFlash();  /* 初始化FLASH参数 */
					
					setNodeId(&Master_Data, nodeID);
					setState(&Master_Data, Initialisation);				//节点初始化
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
				 InitError(); // 清除故障error；
				 memset(&RobotFinishFlag,0,sizeof(RobotFinishFlag));
	//				RobotFinishFlag.AllReturnHome_Finish = 0xff;						//回原完成标志
			}
	/* canopen重启 end */		
			ScanServoError();
			ScanStept();
			Scanf_Lift_message();										/* 升降原点极限等扫描 */
			ScanServoFinishFlag();									/* 扫描舵机完成标志 */
			ScanReturnHome();												/* 扫描回原完成状态 */		

			if((errorWord2[0] | errorWord2[1] | errorWord1[1] | errorWord1[0]) != 0)				/* 错误检测 */
			{
				RobotState.motorerror = 0xff;	
			}
		}
		if(modeOfOperation_Init(1) == -1)				/* canopen清故障后上电扫描 */
		{
			controlWord_6040[0]  = 0x0f;
			controlWord_6040[1]  = 0x0f;
			
			modeOfOperation_6060[0] = 0x01;
			modeOfOperation_6060[1] = 0x01;
			modeOfOperation_Init(0) = 0;
			RobotState.motorerror = 0;											/* 错误标志 */
		}
		
		ScanSteptStartUp();					/* 步进电机启动标志扫描并开始 */
		ScanLiftControlSoftTIm();						/* 位置控制启动软件定时器扫描 */

		
		/* 循环扫描步进电机软件定时器 */
	 SteptMotor_SoferTime_Big(BigFork_Num,TIM_BigFork);								/* 步进电机加减速扫描 */
	 SteptMotor_SoferTime_Small(SmallFork_Num,TIM_SmallFork);
	 SteptMotor_SoferTime_Rotate(Rotate_Num,TIM_Rotate);
		
//		/* 循环扫描485保护定时器 */
//		rs485_contral_lim();
		Sensor_Scan();													/* 传感器扫描 */
//		SmallForkAllowExtend();
//		BigForkAllowExtend();
//		ScanBigForkNearSmallFork();								/* 大小叉接近传感器 */
		ScanSoftTimerState();										/* 扫描软件定时器  目前只有步进回原，后期软件定时器扫描均在此函数中 */	
	}
}






