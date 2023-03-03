 
#include "bsp_internalFlash.h"   

/*׼��д��Ĳ�������*/
#define DATA_32                 ((uint32_t)0x00000001)
 
static uint32_t GetSector(uint32_t Address);



int EraseSector_Flash(void)
{
	
	/*Ҫ��������ʼ����(����)����������(������)����8-12����ʾ����8��9��10��11����*/
	uint32_t uwStartSector = 0;
	uint32_t uwEndSector = 0;
	

	uint32_t i = 0;

	
  /* FLASH ���� ********************************/
  /* ʹ�ܷ���FLASH���ƼĴ��� */
  FLASH_Unlock();
    
  /* �����û����� (�û�����ָ������û��ʹ�õĿռ䣬�����Զ���)**/
  /* �������FLASH�ı�־λ */  
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 


	uwStartSector = GetSector(FLASH_USER_START_ADDR);
	uwEndSector = GetSector(FLASH_USER_END_ADDR);

  /* ��ʼ�������� */
	 for(i = uwStartSector; i < uwEndSector; i += 8)
  {
    /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
       be done by word */ 
    if (FLASH_EraseSector(i, VoltageRange_3) != FLASH_COMPLETE)
    {
      /* Error occurred while page erase */
      return (-1);
    }
  }
  /* ��FLASH��������ֹ���ݱ��۸�*/
  FLASH_Lock(); 
  return (0);
}

int WriteConfig_Flash(uint32_t data, uint32_t Start_address, uint32_t End_address)
{
	
		uint32_t uwAddress = 0;
	 uwAddress = Start_address;

	
	  /* FLASH ���� ********************************/
  /* ʹ�ܷ���FLASH���ƼĴ��� */
  FLASH_Unlock();
	
  while (uwAddress < End_address)
  {
    if (FLASH_ProgramWord(uwAddress, data) == FLASH_COMPLETE)
    {
      uwAddress = uwAddress + 4;
    }
    else
    { 
      /*д��������أ�ʵ��Ӧ���пɼ��봦�� */
			return -1;
    }
  }
	
		  /* ��FLASH��������ֹ���ݱ��۸�*/
  FLASH_Lock(); 
	return 0;
}


/* д16λ */
int WriteConfig_Flash_16Bit(u16 data, uint32_t Start_address, uint32_t End_address)
{
	
	 uint32_t uwAddress = 0;
	 uwAddress = Start_address;

	
	  /* FLASH ���� ********************************/
  /* ʹ�ܷ���FLASH���ƼĴ��� */
  FLASH_Unlock();
	
  while (uwAddress < End_address)
  {
    if (FLASH_ProgramHalfWord(uwAddress, data) == FLASH_COMPLETE)
    {
      uwAddress = uwAddress + 2;
    }
    else
    { 
      /*д��������أ�ʵ��Ӧ���пɼ��봦�� */
			return -1;
    }
  }
	
		  /* ��FLASH��������ֹ���ݱ��۸�*/
  FLASH_Lock(); 
	return 0;
}




u16 ReadConfig_Flash_16Bit(uint32_t Start_address, uint32_t End_address)				/* һ��ֻ�������ֽ� */
{
		uint32_t uwAddress = 0;
		u16 data = 0;
	__IO uint32_t uwMemoryProgramStatus = 0;
	
	  /* FLASH ���� ********************************/
  /* ʹ�ܷ���FLASH���ƼĴ��� */
  FLASH_Unlock();
	
  /* ��FLASH�ж�ȡ�����ݽ���У��***************************************/
  /*  MemoryProgramStatus = 0: д���������ȷ
      MemoryProgramStatus != 0: д������ݴ�����ֵΪ����ĸ��� */
  uwAddress = Start_address;
  uwMemoryProgramStatus = 0;
  
  while (uwAddress < End_address)
  {
    data = *(__IO uint16_t*)uwAddress;

		
    uwAddress = uwAddress + 2;
  }  
  
	
		  /* ��FLASH��������ֹ���ݱ��۸�*/
  FLASH_Lock(); 
	return data;
}

u8 ReadConfig_Flash_8Bit(uint32_t Start_address, uint32_t End_address)				/* һ��ֻ��1���ֽ� */
{
		uint32_t uwAddress = 0;
		u32 data = 0;
	__IO uint32_t uwMemoryProgramStatus = 0;
	
	  /* FLASH ���� ********************************/
  /* ʹ�ܷ���FLASH���ƼĴ��� */
  FLASH_Unlock();
	
  /* ��FLASH�ж�ȡ�����ݽ���У��***************************************/
  /*  MemoryProgramStatus = 0: д���������ȷ
      MemoryProgramStatus != 0: д������ݴ�����ֵΪ����ĸ��� */
  uwAddress = Start_address;
  uwMemoryProgramStatus = 0;
  
  while (uwAddress < End_address)
  {
    data = *(__IO uint32_t*)uwAddress;

		
    uwAddress = uwAddress + 1;
  }  
  
	
		  /* ��FLASH��������ֹ���ݱ��۸�*/
  FLASH_Lock(); 
	return data;
}

void Init_IAP_Flash_Flag(void)
{
	u16 IAP_FLAG = ReadConfig_Flash_16Bit(ADDR_CodeWriteFlag, ADDR_CodeWriteFlag+2);
	if (IAP_FLAG != 0x55)
	{
		WriteConfig_Flash_16Bit(0x55, ADDR_CodeWriteFlag, ADDR_CodeWriteFlag+2);
	}
}


void Jump_To_IAP_Program(void)
{
	pFunction Jump_To_Application;
	uint32_t JumpAddress;
	WriteConfig_Flash_16Bit(0, ADDR_CodeWriteFlag, ADDR_CodeWriteFlag+2); //ʹ��IAP��������
	    /* Test if user code is programmed starting from address "IAP_ADDRESS" */
//	if (((*(__IO uint32_t*)IAP_ADDRESS) & 0x2FFE0000 ) == 0x20000000)
//	{ 
//		/* Jump to IAP application */
//		JumpAddress = *(__IO uint32_t*) (IAP_ADDRESS + 4);
//		Jump_To_Application = (pFunction) JumpAddress;
//		/* Initialize IAP's Stack Pointer */
//		__set_MSP(*(__IO uint32_t*) IAP_ADDRESS);
//		Jump_To_Application();
//	}
	NVIC_SystemReset();
}


/* ��FLASH�ж�ȡ���������� */
void Robot_ConfigInit_FromFlash(void)
{
	u8 i = 0;
//	int32_t	paraTTT = 0;
	/* ���������������ת�ٶȼ��Ӽ��ٶ����� ����100�� */
//	FlashConfig.BigFork_ES_V = (float)ReadConfig_Flash_16Bit(ADDR_FLASH_SECTOR_6, ADDR_FLASH_SECTOR_6 + 2)/100;		/* ��������ٶ� */
//	FlashConfig.BigFork_ES_AV = (float)ReadConfig_Flash_16Bit(ADDR_FLASH_SECTOR_6 + 2, ADDR_FLASH_SECTOR_6 + 4)/100;		/* ����������ٶ� */
//	FlashConfig.BigFork_ES_DV = (float)ReadConfig_Flash_16Bit(ADDR_FLASH_SECTOR_6 + 4, ADDR_FLASH_SECTOR_6 + 6)/100;		/* ����������ٶ� */
//	
	FlashConfig.BigFork_ES_V = 10;		/* ��������ٶ� */																	/* ��沽��������� */
	FlashConfig.BigFork_ES_AV = 10;		/* ����������ٶ� */
	FlashConfig.BigFork_ES_DV = 5;		/* ����������ٶ� */	
	
	
	
	
//	FlashConfig.SmallFork_ES_V = (float)ReadConfig_Flash_16Bit(ADDR_FLASH_SECTOR_6 + 6, ADDR_FLASH_SECTOR_6 + 8)/100;		/* С�������ٶ� */
//	FlashConfig.SmallFork_ES_AV = (float)ReadConfig_Flash_16Bit(ADDR_FLASH_SECTOR_6 + 8, ADDR_FLASH_SECTOR_6 + 10)/100;		/* С���������ٶ� */
//	FlashConfig.SmallFork_ES_DV = (float)ReadConfig_Flash_16Bit(ADDR_FLASH_SECTOR_6 + 10, ADDR_FLASH_SECTOR_6 + 12)/100;		/* С���������ٶ� */

FlashConfig.SmallFork_ES_V  = 20;
FlashConfig.SmallFork_ES_AV = 20;
FlashConfig.SmallFork_ES_DV = 15;

//	FlashConfig.Rotate_V = (float)ReadConfig_Flash_16Bit(ADDR_FLASH_SECTOR_6 + 12, ADDR_FLASH_SECTOR_6 + 14)/100;			/* ��ת�ٶ� */
//	FlashConfig.Rotate_AV = (float)ReadConfig_Flash_16Bit(ADDR_FLASH_SECTOR_6 + 14, ADDR_FLASH_SECTOR_6 + 16)/100;		/* ��ת���ٶ� */
//	FlashConfig.Rotate_DV = (float)ReadConfig_Flash_16Bit(ADDR_FLASH_SECTOR_6 + 16, ADDR_FLASH_SECTOR_6 + 18)/100;		/* ��ת���ٶ� */

	FlashConfig.Rotate_V = 8;			/* ��ת�ٶ� */
	FlashConfig.Rotate_AV = 10;		/* ��ת���ٶ� */
	FlashConfig.Rotate_DV = 3;		/* ��ת���ٶ� */




	/* ��������ת�˶���������		��ֵ����100�� */
//FlashConfig.BigFork_ES_Position = (float)ReadConfig_Flash_16Bit(ADDR_FLASH_SECTOR_6 + 18, ADDR_FLASH_SECTOR_6 + 20)/100;		/* �������λ�� ��ֵ����100�� */

FlashConfig.BigFork_ES_Position = 3.2;

//FlashConfig.SmallFork_ES_Position = (float)ReadConfig_Flash_16Bit(ADDR_FLASH_SECTOR_6 + 20, ADDR_FLASH_SECTOR_6 + 22)/100;		/* С������λ�� ��ֵ����100�� */

FlashConfig.SmallFork_ES_Position = 4.7; //4.08
	
	
//	FlashConfig.Rotate_Position = (float)ReadConfig_Flash_16Bit(ADDR_FLASH_SECTOR_6 + 22, ADDR_FLASH_SECTOR_6 + 24)/100;		/* ��תλ�� ��ֵ����100�� */
	FlashConfig.Rotate_Position = 9.5;  //7.42 \  7.62
	
	
	
	/* �������λ�� */
//	FlashConfig.BigFork_FirstBin[0] = ReadConfig_Flash_16Bit(ADDR_FLASH_SECTOR_6 + 24, ADDR_FLASH_SECTOR_6 + 26) ;		/* ����һ������λ�� */
//	FlashConfig.BigFork_FirstBin[1] = ReadConfig_Flash_16Bit(ADDR_FLASH_SECTOR_6 + 26, ADDR_FLASH_SECTOR_6 + 28) ;		/* ���ڶ�������λ�� */	
//	FlashConfig.BigFork_FirstBin[2] = ReadConfig_Flash_16Bit(ADDR_FLASH_SECTOR_6 + 28, ADDR_FLASH_SECTOR_6 + 30) ;		/* ������������λ�� */
//	FlashConfig.BigFork_FirstBin[3] = ReadConfig_Flash_16Bit(ADDR_FLASH_SECTOR_6 + 30, ADDR_FLASH_SECTOR_6 + 32) ;		/* �����Ĳ�����λ�� */		
//	FlashConfig.BigFork_FirstBin[4] = ReadConfig_Flash_16Bit(ADDR_FLASH_SECTOR_6 + 32, ADDR_FLASH_SECTOR_6 + 34) ;		/* �����������λ�� */
//	FlashConfig.BigFork_FirstBin[5] = ReadConfig_Flash_16Bit(ADDR_FLASH_SECTOR_6 + 34, ADDR_FLASH_SECTOR_6 + 36) ;		/* ������������λ�� */	
//	FlashConfig.BigFork_FirstBin[6] = ReadConfig_Flash_16Bit(ADDR_FLASH_SECTOR_6 + 36, ADDR_FLASH_SECTOR_6 + 38) ;		/* �����߲�����λ�� */
//	FlashConfig.BigFork_FirstBin[7] = ReadConfig_Flash_16Bit(ADDR_FLASH_SECTOR_6 + 38, ADDR_FLASH_SECTOR_6 + 40) ;		/* ���ڰ˲�����λ�� */			
	
	
	FlashConfig.BigFork_FirstBin[0] = -735;		/* ����һ������λ�� */  //-745
	FlashConfig.BigFork_FirstBin[1] = -495;		/* ���ڶ�������λ�� */	//-495
	FlashConfig.BigFork_FirstBin[2] = -245;		/* ������������λ�� */  //-250
	FlashConfig.BigFork_FirstBin[3] = -5;		/* �����Ĳ�����λ�� */	//-10	
	FlashConfig.BigFork_FirstBin[4] = 240;		/* �����������λ�� */  //235
	FlashConfig.BigFork_FirstBin[5] = 490;		/* ������������λ�� */	//485
	FlashConfig.BigFork_FirstBin[6] = 735;		/* �����߲�����λ�� */  //730
	FlashConfig.BigFork_FirstBin[7] = 975;    /* ���ڰ˲�����λ�� */  //975
	
//	/* С������λ�� */	
//	FlashConfig.SmallFork_FirstBin[0] = ReadConfig_Flash_16Bit(ADDR_FLASH_SECTOR_6 + 40, ADDR_FLASH_SECTOR_6 + 42);		/* С���һ������λ�� */
//	FlashConfig.SmallFork_FirstBin[1] = ReadConfig_Flash_16Bit(ADDR_FLASH_SECTOR_6 + 42, ADDR_FLASH_SECTOR_6 + 44);		/* С���һ������λ�� */	
//	FlashConfig.SmallFork_FirstBin[2] = ReadConfig_Flash_16Bit(ADDR_FLASH_SECTOR_6 + 44, ADDR_FLASH_SECTOR_6 + 46);		/* С���һ������λ�� */	
//	FlashConfig.SmallFork_FirstBin[3] = ReadConfig_Flash_16Bit(ADDR_FLASH_SECTOR_6 + 46, ADDR_FLASH_SECTOR_6 + 48);		/* С���һ������λ�� */	
//	FlashConfig.SmallFork_FirstBin[4] = ReadConfig_Flash_16Bit(ADDR_FLASH_SECTOR_6 + 48, ADDR_FLASH_SECTOR_6 + 50);		/* С���һ������λ�� */
//	FlashConfig.SmallFork_FirstBin[5] = ReadConfig_Flash_16Bit(ADDR_FLASH_SECTOR_6 + 50, ADDR_FLASH_SECTOR_6 + 52);		/* С���һ������λ�� */	
//	FlashConfig.SmallFork_FirstBin[6] = ReadConfig_Flash_16Bit(ADDR_FLASH_SECTOR_6 + 52, ADDR_FLASH_SECTOR_6 + 54);		/* С���һ������λ�� */	
//	FlashConfig.SmallFork_FirstBin[7] = ReadConfig_Flash_16Bit(ADDR_FLASH_SECTOR_6 + 54, ADDR_FLASH_SECTOR_6 + 56);		/* С���һ������λ�� */	
	
		/* С������λ�� */	
	FlashConfig.SmallFork_FirstBin[0] = -443;	/* С���һ������λ�� */
	FlashConfig.SmallFork_FirstBin[1] = -235;		/* С��ڶ�������λ�� */	
	FlashConfig.SmallFork_FirstBin[2] = 10;		/* С�����������λ�� */	
	FlashConfig.SmallFork_FirstBin[3] = 255;		/* С����Ĳ�����λ�� */
	FlashConfig.SmallFork_FirstBin[4] = 500;		/* С����������λ�� */
	FlashConfig.SmallFork_FirstBin[5] = 745;		/* С�����������λ�� */	
	FlashConfig.SmallFork_FirstBin[6] = 950;		/* С����߲�����λ�� */	
	FlashConfig.SmallFork_FirstBin[7] = 1235;		/* С��ڰ˲�����λ�� */	
	
	
	/* ��¨ */
//	FlashConfig.BackPack_FirstBin[0] = ReadConfig_Flash_16Bit(ADDR_FLASH_SECTOR_6 + 56, ADDR_FLASH_SECTOR_6 + 58);		/* ��¨��һ������λ�� */	
//	FlashConfig.BackPack_FirstBin[1] = ReadConfig_Flash_16Bit(ADDR_FLASH_SECTOR_6 + 58, ADDR_FLASH_SECTOR_6 + 60);		/* ��¨��һ������λ�� */		
//	FlashConfig.BackPack_FirstBin[2] = ReadConfig_Flash_16Bit(ADDR_FLASH_SECTOR_6 + 60, ADDR_FLASH_SECTOR_6 + 62);		/* ��¨��һ������λ�� */		
//	FlashConfig.BackPack_FirstBin[3] = ReadConfig_Flash_16Bit(ADDR_FLASH_SECTOR_6 + 62, ADDR_FLASH_SECTOR_6 + 64);		/* ��¨��һ������λ�� */		
	
	FlashConfig.BackPack_FirstBin[0] = 0;		/* ��¨��һ������λ�� -20*/	
	FlashConfig.BackPack_FirstBin[1] = 250;		/* ��¨��2������λ�� 220*/		
	FlashConfig.BackPack_FirstBin[2] = 495;		/* ��¨��3������λ�� 515*/		
	FlashConfig.BackPack_FirstBin[3] = 735;	  /* ��¨��4������λ�� 740*/
	FlashConfig.BackPack_FirstBin[4] = 980;		/* ��¨��5������λ�� 1000*/	
	FlashConfig.BackPack_FirstBin[5] = 1225;		/* ��¨��6������λ�� 1190*/	

	FlashConfig.BackPack_FirstBin_real[0] = 10;		/* ��¨��һ������λ�� 10*/	
	FlashConfig.BackPack_FirstBin_real[1] = 250;		/* ��¨��2������λ�� 250*/		
	FlashConfig.BackPack_FirstBin_real[2] = 735;		/* ��¨��3������λ�� 740*/		
	FlashConfig.BackPack_FirstBin_real[3] = 735;	/* ��¨��4������λ�� 740*/
	FlashConfig.BackPack_FirstBin_real[4] = 1225;		/* ��¨��5������λ�� 1210*/	
	FlashConfig.BackPack_FirstBin_real[5] = 1225;		/* ��¨��6������λ�� 1210*/
	
	FlashConfig.Rotate_Angle_Home = ReadConfig_Flash_16Bit(ADDR_ConfigData, ADDR_ConfigData + 2);
	FlashConfig.Rotate_Angle_Left = ReadConfig_Flash_16Bit(ADDR_ConfigData+2, ADDR_ConfigData + 4);
	FlashConfig.Rotate_Angle_Right = ReadConfig_Flash_16Bit(ADDR_ConfigData+4, ADDR_ConfigData + 6);
	
	
	for(i = 0; i<8; i++)
	{
		if(FlashConfig.BigFork_FirstBin[i] < 0)
		{
				LiftBigFork_Floor[i] =-((int32_t)((((double)(-FlashConfig.BigFork_FirstBin[i]) * 10000) / LiftBigFork_1RadToMM)*10000));
		}
		else
		{
				LiftBigFork_Floor[i] =(int32_t)(((((double)FlashConfig.BigFork_FirstBin[i]) * 10000) / LiftBigFork_1RadToMM)*10000);
		}
	}
	
	LiftBigFork_Floor[8] = (int32_t)(((((double)BigFork_MaxPosition) * 10000) / LiftBigFork_1RadToMM)*10000);
	LiftBigFork_Floor[9] = -((int32_t)((((double)(-BigFork_MinPosition) * 10000) / LiftBigFork_1RadToMM)*10000));
	LiftBigFork_Floor[10] = -((int32_t)((((double)(-BigFork_MinBackPosition) * 10000) / LiftBigFork_1RadToMM)*10000));
	LiftBigFork_Floor[11] = -((int32_t)((((double)(-BigFork_Position_When_SmallFork_LowSpeed) * 10000) / LiftBigFork_1RadToMM)*10000));
	LiftBigFork_Floor[12] = ((int32_t)((((double)(BigFork_InboundPositin) * 10000) / LiftBigFork_1RadToMM)*10000));
	LiftBigFork_Floor[13] = ((int32_t)((((double)(BigFork_OutboundPosition) * 10000) / LiftBigFork_1RadToMM)*10000));
	
	for(i = 0; i<8; i++)
	{
		if(FlashConfig.SmallFork_FirstBin[i] < 0)
		{
				LiftSmallFork_Floor[i] =-((int32_t)((((double)(-FlashConfig.SmallFork_FirstBin[i]) * 10000) / LiftSmallFork_1RadToMM)*10000));
		}
		else
		{
				LiftSmallFork_Floor[i] =(int32_t)(((((double)FlashConfig.SmallFork_FirstBin[i]) * 10000) / LiftSmallFork_1RadToMM)*10000);
		}
	}
	LiftSmallFork_Floor[8] = (int32_t)(((((double)SmallFork_MaxPosition) * 10000) / LiftSmallFork_1RadToMM)*10000);
	LiftSmallFork_Floor[9] = -((int32_t)((((double)(-SmallFork_MinPosition) * 10000) / LiftSmallFork_1RadToMM)*10000));
	LiftSmallFork_Floor[10] = -((int32_t)((((double)(-SmallFork_MinBackPosition) * 10000) / LiftSmallFork_1RadToMM)*10000));
	LiftSmallFork_Floor[11] = -((int32_t)((((double)(-SmallFork_MinPosition_HighSpeed_HaveTote) * 10000) / LiftSmallFork_1RadToMM)*10000));
	LiftSmallFork_Floor[12] = ((int32_t)((((double)(SmallFork_MaxPositon_Collisiion_BigFork) * 10000) / LiftSmallFork_1RadToMM)*10000));
	LiftSmallFork_Floor[13] = ((int32_t)((((double)(SmallFork_InboundPosition) * 10000) / LiftSmallFork_1RadToMM)*10000));
	LiftSmallFork_Floor[14] = ((int32_t)((((double)(SmallFork_OutboundPosition) * 10000) / LiftSmallFork_1RadToMM)*10000));
	
	
	
	
	for(i = 0; i<6; i++)
	{
		if(FlashConfig.BackPack_FirstBin_real[i] < 0)
		{
				BackPack_Floor[i] =-((int32_t)((((double)(-FlashConfig.BackPack_FirstBin_real[i]) * 10000) / LiftSmallFork_1RadToMM)*10000));
		}
		else
		{
				BackPack_Floor[i] =(int32_t)(((((double)FlashConfig.BackPack_FirstBin_real[i]) * 10000) / LiftSmallFork_1RadToMM)*10000);
		}
	}

	for(i = 0; i<6; i++)
	{
		if(FlashConfig.BackPack_FirstBin[i] < 0)
		{
				BackPack_Floor_PutDown[i] =-((int32_t)((((double)(-FlashConfig.BackPack_FirstBin[i]) * 10000) / LiftSmallFork_1RadToMM)*10000)) - LiftSmallForkBackPackDec;
		}
		else
		{
				BackPack_Floor_PutDown[i] =(int32_t)(((((double)FlashConfig.BackPack_FirstBin[i]) * 10000) / LiftSmallFork_1RadToMM)*10000) - LiftSmallForkBackPackDec;
		}
	}
	
	BackPack_BigFork_sync[0] = (int32_t)(((((double)0.0) * 10000) / LiftBigFork_1RadToMM)*10000);
	BackPack_BigFork_sync[1] = (int32_t)(((((double)260.0) * 10000) / LiftBigFork_1RadToMM)*10000);
	BackPack_BigFork_sync[2] = (int32_t)(((((double)750.0) * 10000) / LiftBigFork_1RadToMM)*10000);
	BackPack_BigFork_sync[3] = (int32_t)(((((double)750.0) * 10000) / LiftBigFork_1RadToMM)*10000);
	BackPack_BigFork_sync[4] = (int32_t)(((((double)BigFork_MaxPosition) * 10000) / LiftBigFork_1RadToMM)*10000);
	BackPack_BigFork_sync[5] = (int32_t)(((((double)BigFork_MaxPosition) * 10000) / LiftBigFork_1RadToMM)*10000);
	
	
	LiftBigForkCapture_Floor[0] =-(int32_t)(((((double)250) * 10000) / LiftBigFork_1RadToMM)*10000);
	LiftBigForkCapture_Floor[1] =-(int32_t)(((((double)250) * 10000) / LiftBigFork_1RadToMM)*10000);
	LiftBigForkCapture_Floor[2] =-(int32_t)(((((double)10) * 10000) / LiftBigFork_1RadToMM)*10000);
	LiftBigForkCapture_Floor[3] =(int32_t)(((((double)235) * 10000) / LiftBigFork_1RadToMM)*10000);
	LiftBigForkCapture_Floor[4] =(int32_t)(((((double)485) * 10000) / LiftBigFork_1RadToMM)*10000);
	LiftBigForkCapture_Floor[5] =(int32_t)(((((double)725) * 10000) / LiftBigFork_1RadToMM)*10000);
	LiftBigForkCapture_Floor[6] =(int32_t)(((((double)970) * 10000) / LiftBigFork_1RadToMM)*10000);
	LiftBigForkCapture_Floor[7] =(int32_t)(((((double)BigFork_MaxPosition) * 10000) / LiftBigFork_1RadToMM)*10000);
	LiftBigForkCapture_Floor[8] =(int32_t)(((((double)455) * 10000) / LiftBigFork_1RadToMM)*10000);  //��������մ�С��߶�
	
	LiftSmallForkCapute_Floor[0] = -(int32_t)(((((double)335.0) * 10000) / LiftSmallFork_1RadToMM)*10000);
	LiftSmallForkCapute_Floor[1] = -(int32_t)(((((double)335.0) * 10000) / LiftSmallFork_1RadToMM)*10000);
	LiftSmallForkCapute_Floor[2] = -(int32_t)(((((double)90.0) * 10000) / LiftSmallFork_1RadToMM)*10000);
	LiftSmallForkCapute_Floor[3] = (int32_t)(((((double)155.0) * 10000) / LiftSmallFork_1RadToMM)*10000);
	LiftSmallForkCapute_Floor[4] = (int32_t)(((((double)400.0) * 10000) / LiftSmallFork_1RadToMM)*10000);
	LiftSmallForkCapute_Floor[5] = (int32_t)(((((double)645.0) * 10000) / LiftSmallFork_1RadToMM)*10000);
	LiftSmallForkCapute_Floor[6] = (int32_t)(((((double)890.0) * 10000) / LiftSmallFork_1RadToMM)*10000);
	LiftSmallForkCapute_Floor[7] = (int32_t)(((((double)1135.0) * 10000) / LiftSmallFork_1RadToMM)*10000);
	LiftSmallForkCapute_Floor[8] = (int32_t)(((((double)125.0) * 10000) / LiftSmallFork_1RadToMM)*10000); //������С�����ո߶�


	
	/* -----------------------------------------------------------------------------------���������ԭ������ʼ��----------------------- */
	BigFork_ReturnHome.ReturnHome_Accel = 2;
	BigFork_ReturnHome.ReturnHome_Decel = 2;
	BigFork_ReturnHome.ReturnHome_Position = 4.2; 
	BigFork_ReturnHome.ReturnHome_Speed = 2;
	
	SmallFork_Return.ReturnHome_Accel = 3;
	SmallFork_Return.ReturnHome_Decel = 3;
	SmallFork_Return.ReturnHome_Position = 8; 
	SmallFork_Return.ReturnHome_Speed = 3;
	
	Rotate_Return.ReturnHome_Accel = 3;
	Rotate_Return.ReturnHome_Decel = 3;
	Rotate_Return.ReturnHome_Position = 22; 
	Rotate_Return.ReturnHome_Speed = 2;
	
	/*--------------------------------------------------------------------------------------------------------*/
	
	
	
	
	
	
	
	
	
	

	
	
//	/* ��һ������λ�� */
//	
//	FlashConfig.BigFork_FirstBin = ReadConfig_Flash_16Bit(ADDR_FLASH_SECTOR_6 + 24, ADDR_FLASH_SECTOR_6 + 26) ;		/* ����һ������λ�� */
//	
//	paraTTT = FlashConfig.BigFork_FirstBin;
//	for(i = 0; i<8; i++)
//	{
//		LiftBigFork_Floor[i] = paraTTT;
//		paraTTT = (u16)(paraTTT + Bin_Distance);
//	}
//	
//	FlashConfig.SmallFork_FirstBin = ReadConfig_Flash_16Bit(ADDR_FLASH_SECTOR_6 + 26, ADDR_FLASH_SECTOR_6 + 28);		/* С���һ������λ�� */
//	
//	paraTTT = FlashConfig.SmallFork_FirstBin;
//	for(i = 0; i<8; i++)
//	{
//		LiftSmallFork_Floor[i] = paraTTT;
//		paraTTT = (u16)(paraTTT + Bin_Distance);
//	}
//	
//	
//	FlashConfig.BackPack_FirstBin = ReadConfig_Flash_16Bit(ADDR_FLASH_SECTOR_6 + 28, ADDR_FLASH_SECTOR_6 + 30);		/* ��¨��һ������λ�� */	
//	paraTTT = FlashConfig.BackPack_FirstBin;
//	for(i = 0; i<4; i++)
//	{
//		BackPack_Floor[i] = paraTTT;
//		paraTTT = paraTTT + BackPack_Distance;
//	}
//	
	
	/* ��ʼ������ */
}




/**
  * @brief  InternalFlash_Test,���ڲ�FLASH���ж�д����
  * @param  None
  * @retval None
  */
int InternalFlash_Test(void)
{
	/*Ҫ��������ʼ����(����)����������(������)����8-12����ʾ����8��9��10��11����*/
	uint32_t uwStartSector = 0;
	uint32_t uwEndSector = 0;
	
	uint32_t uwAddress = 0;
	uint32_t uwSectorCounter = 0;

	__IO uint32_t uwData32 = 0;
	__IO uint32_t uwMemoryProgramStatus = 0;
	
  /* FLASH ���� ********************************/
  /* ʹ�ܷ���FLASH���ƼĴ��� */
  FLASH_Unlock();
    
  /* �����û����� (�û�����ָ������û��ʹ�õĿռ䣬�����Զ���)**/
  /* �������FLASH�ı�־λ */  
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 


	uwStartSector = GetSector(FLASH_USER_START_ADDR);
	uwEndSector = GetSector(FLASH_USER_END_ADDR);

  /* ��ʼ�������� */
  uwSectorCounter = uwStartSector;
  while (uwSectorCounter <= uwEndSector) 
  {
    /* VoltageRange_3 �ԡ��֡��Ĵ�С���в��� */ 
    if (FLASH_EraseSector(uwSectorCounter, VoltageRange_3) != FLASH_COMPLETE)
    { 
      /*�����������أ�ʵ��Ӧ���пɼ��봦�� */
			return -1;
    }
    /* ������ָ����һ������ */
    if (uwSectorCounter == FLASH_Sector_11)
    {
      uwSectorCounter += 40;
    } 
    else 
    {
      uwSectorCounter += 8;
    }
  }

  /* �ԡ��֡��Ĵ�СΪ��λд������ ********************************/
  uwAddress = FLASH_USER_START_ADDR;

  while (uwAddress < FLASH_USER_END_ADDR)
  {
    if (FLASH_ProgramWord(uwAddress, DATA_32) == FLASH_COMPLETE)
    {
      uwAddress = uwAddress + 4;
    }
    else
    { 
      /*д��������أ�ʵ��Ӧ���пɼ��봦�� */
			return -1;
    }
  }
	

  /* ��FLASH��������ֹ���ݱ��۸�*/
  FLASH_Lock(); 


  /* ��FLASH�ж�ȡ�����ݽ���У��***************************************/
  /*  MemoryProgramStatus = 0: д���������ȷ
      MemoryProgramStatus != 0: д������ݴ�����ֵΪ����ĸ��� */
  uwAddress = FLASH_USER_START_ADDR;
  uwMemoryProgramStatus = 0;
  
  while (uwAddress < FLASH_USER_END_ADDR)
  {
    uwData32 = *(__IO uint32_t*)uwAddress;

    if (uwData32 != DATA_32)
    {
      uwMemoryProgramStatus++;  
    }

    uwAddress = uwAddress + 4;
  }  
  /* ����У�鲻��ȷ */
  if(uwMemoryProgramStatus)
  {    
		return -1;
  }
  else /*����У����ȷ*/
  { 
		return 0;   
  }
}

/**
  * @brief  ��������ĵ�ַ���������ڵ�sector
  *					���磺
						uwStartSector = GetSector(FLASH_USER_START_ADDR);
						uwEndSector = GetSector(FLASH_USER_END_ADDR);	
  * @param  Address����ַ
  * @retval ��ַ���ڵ�sector
  */
static uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;
  
  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_Sector_0;  
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_Sector_1;  
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_Sector_2;  
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_Sector_3;  
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_Sector_4;  
  }
  else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_Sector_5;  
  }
  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
  {
    sector = FLASH_Sector_6;  
  }
  else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
  {
    sector = FLASH_Sector_7;  
  }
  else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
  {
    sector = FLASH_Sector_8;  
  }
  else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
  {
    sector = FLASH_Sector_9;  
  }
  else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
  {
    sector = FLASH_Sector_10;  
  }
  else /*((Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_11))*/
  {
    sector = FLASH_Sector_11;  
  }
  return sector;
}



