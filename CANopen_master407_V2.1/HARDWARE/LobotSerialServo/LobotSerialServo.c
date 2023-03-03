/*******************************************************************************
* 文件名： LobotSerial.c
* 作者： 深圳乐幻索尔科技
* 日期：20170217
* LX串口舵机Demo
*******************************************************************************/

#include "LobotSerialServo.h"
#include <stdarg.h>
#include <string.h>

#define LobotSerialWrite  uartWriteBuf

#define GET_LOW_BYTE(A) ((uint8_t)(A))
//宏函数 获得A的低八位
#define GET_HIGH_BYTE(A) ((uint8_t)((A) >> 8))
//宏函数 获得A的高八位
#define BYTE_TO_HW(A, B) ((((uint16_t)(A)) << 8) | (uint8_t)(B))
//宏函数 将高地八位合成为十六位

uint8_t LobotRxBuf[16];

struct BigForkServoState big_fork_servo_state;
struct SmallForkServoState small_fork_servo_state;
struct BackPackServoState back_pack_servo_state;
	
uint8_t LobotCheckSum(uint8_t buf[])
{
  uint8_t i;
  uint16_t temp = 0;
  for (i = 2; i < buf[3] + 2; i++) {
    temp += buf[i];
  }
  temp = ~temp;
  i = (uint8_t)temp;
  return i;
}

void LobotSerialServoSetID(uint8_t oldID, uint8_t newID)
{
	uint8_t buf[7];
  buf[0] = buf[1] = LOBOT_SERVO_FRAME_HEADER;
  buf[2] = oldID;
  buf[3] = 4;
  buf[4] = LOBOT_SERVO_ID_WRITE;
  buf[5] = newID;
  buf[6] = LobotCheckSum(buf);
  LobotSerialWrite(buf, 7);
}


void LobotSerialServoMove(uint8_t id, int16_t position, uint16_t time)
{
  uint8_t buf[10];
  if(position < 0)
    position = 0;
  if(position > 1000)
	position = 1000;
  buf[0] = buf[1] = LOBOT_SERVO_FRAME_HEADER;
  buf[2] = id;
  buf[3] = 7;
  buf[4] = LOBOT_SERVO_MOVE_TIME_WRITE;
  buf[5] = GET_LOW_BYTE(position);
  buf[6] = GET_HIGH_BYTE(position);
  buf[7] = GET_LOW_BYTE(time);
  buf[8] = GET_HIGH_BYTE(time);
  buf[9] = LobotCheckSum(buf);
  LobotSerialWrite(buf, 10);
}

void LobotSerialServoUnload(uint8_t id)
{
  uint8_t buf[7];
  buf[0] = buf[1] = LOBOT_SERVO_FRAME_HEADER;
  buf[2] = id;
  buf[3] = 4;
  buf[4] = LOBOT_SERVO_LOAD_OR_UNLOAD_WRITE;
  buf[5] = 0;
  buf[6] = LobotCheckSum(buf);
  LobotSerialWrite(buf, 7);
}

void LobotSerialServoLoad(uint8_t id)
{
  uint8_t buf[7];
  buf[0] = buf[1] = LOBOT_SERVO_FRAME_HEADER;
  buf[2] = id;
  buf[3] = 4;
  buf[4] = LOBOT_SERVO_LOAD_OR_UNLOAD_WRITE;
  buf[5] = 1;
  buf[6] = LobotCheckSum(buf);
  LobotSerialWrite(buf, 7);
}

int LobotSerialServoReadPosition(uint8_t id)
{
  int ret;
  uint8_t buf[6];

  buf[0] = buf[1] = LOBOT_SERVO_FRAME_HEADER;
  buf[2] = id;
  buf[3] = 3;
  buf[4] = LOBOT_SERVO_POS_READ;
  buf[5] = LobotCheckSum(buf);
	
	LobotSerialWrite(buf, 6);
	
	ret = LobotSerialMsgHandle();
  return ret;
}

int LobotSerialMsgHandle(void)
{
	int count = 50000;
	uint8_t cmd;
	int ret;
	
	while(!isRxCompleted())
	{
		count--;
		if(count < 0)
			return -2048;
	}
	
	if(LobotCheckSum(LobotRxBuf) != LobotRxBuf[LobotRxBuf[3]+2])
	{
		return -2049;
	}
	
	cmd = LobotRxBuf[4];
	switch(cmd)
	{
		case LOBOT_SERVO_POS_READ:
			ret = (int)BYTE_TO_HW(LobotRxBuf[6], LobotRxBuf[5]);
			return ret;
		default:
			break;
	}
	return 0;
}



int LobotSerialMsgHandle_new(uint8_t id)
{
	int count = 50000;
	int ret;
	while(!isRxCompleted())
	{
		count--;
		if(count < 0)
			return -2048;
	}
	if((LobotRxBuf[2] == id) && (LobotRxBuf[3] == 0x04) && (LobotRxBuf[4] == 0))
	{
		ret = (int)BYTE_TO_HW(LobotRxBuf[5], LobotRxBuf[6]);
		memset(&LobotRxBuf, 0, sizeof(LobotRxBuf));	
		return ret;
	}
	else
	{
		memset(&LobotRxBuf, 0, sizeof(LobotRxBuf));
		return -2048;
	}
}

void Close_LobotSerialServo(void)
{
	LobotSerialServoMoveConfig(ID1, ID1_BACK, 100);						/* ID1配置完成 */	
	delay_ms(100);	
	LobotSerialServoMoveConfig(ID2, ID2_BACK, 100);						/* ID2配置完成 */	
	delay_ms(100);	
	LobotSerialServoMoveConfig(ID3, ID3_BACK, 100);						/* ID3配置完成 */
	delay_ms(100);
	LobotSerialServoMoveConfig(ID4, ID4_BACK, 100);						/* ID4配置完成 */	
	delay_ms(100);

	
	LobotSerialServoMoveConfig(ID7, ID7_BACK, 100);						/* ID7配置完成 */
delay_ms(100);	
	LobotSerialServoMoveConfig(ID8, ID8_BACK, 100);						/* ID8配置完成 */
delay_ms(100);	
	LobotSerialServoMoveConfig(ID9, ID9_BACK, 100);						/* ID9配置完成 */
	delay_ms(100);
	LobotSerialServoMoveConfig(ID10, ID10_BACK, 100);						/* ID10配置完成 */
	delay_ms(100);	

	LobotSerialServoMoveConfig(ID13, ID13_OUT, 100);						/* ID13配置完成 */
delay_ms(100);	
	LobotSerialServoMoveConfig(ID14, ID14_OUT, 100);						/* ID14配置完成 */
delay_ms(100);	
	LobotSerialServoMoveConfig(ID15, ID15_OUT, 100);						/* ID15配置完成 */
	delay_ms(100);
	LobotSerialServoMoveConfig(ID16, ID16_OUT, 100);						/* ID16配置完成 */
delay_ms(100);	
	LobotSerialServoMoveConfig(ID17, ID17_OUT, 100);						/* ID17配置完成 */
	delay_ms(100);
	LobotSerialServoMoveConfig(ID18, ID18_OUT, 100);						/* ID18配置完成 */
delay_ms(100);	
	LobotSerialServoMoveConfig(ID19, ID19_OUT, 100);						/* ID19配置完成 */
	delay_ms(100);
	LobotSerialServoMoveConfig(ID20, ID20_OUT, 100);						/* ID20配置完成 */
}



void Scan_LobotSerialServo(void)
{
	/*  1/2号舵机 */
		if(Lobot2 == 0xff)
		{
			if(dir_lobnot == 1)
			{
				LobotSerialServoMoveConfig(ID1, ID1_OUT, 100);						/* ID1配置完成 */
				delay_ms(100);				
				LobotSerialServoMoveConfig(ID2, ID2_OUT, 100);						/* ID2配置完成 */	
				delay_ms(100);				
				LobotSerialServoMoveConfig(ID3, ID3_OUT, 100);						/* ID3配置完成 */
				delay_ms(100);
				LobotSerialServoMoveConfig(ID4, ID4_OUT, 100);						/* ID4配置完成 */
			}
			if(dir_lobnot == 2)																																					/* 大叉右回 */
			{
				LobotSerialServoMoveConfig(ID1, ID1_BACK, 100);						/* ID1配置完成 */
				delay_ms(100);				
				LobotSerialServoMoveConfig(ID2, ID2_BACK, 100);						/* ID2配置完成 */	
				delay_ms(100);				
				LobotSerialServoMoveConfig(ID3, ID3_BACK, 100);						/* ID3配置完成 */
				delay_ms(100);
				LobotSerialServoMoveConfig(ID4, ID4_BACK, 100);						/* ID4配置完成 */					
				
			}
			/*  3/4号舵机 */
			if(dir_lobnot == 3)																																			/* 大叉左出 */
			{
				LobotSerialServoMoveConfig(ID1, ID1_OUT, 100);						/* ID1配置完成 */	
				delay_ms(100);				
				LobotSerialServoMoveConfig(ID2, ID2_OUT, 100);						/* ID2配置完成 */	
				delay_ms(100);				
				LobotSerialServoMoveConfig(ID3, ID3_OUT, 100);						/* ID3配置完成 */
				delay_ms(100);
				LobotSerialServoMoveConfig(ID4, ID4_OUT, 100);						/* ID4配置完成 */			
			}
			if(dir_lobnot == 4)																																				/* 大叉左回 */
			{
				LobotSerialServoMoveConfig(ID1, ID1_BACK, 100);						/* ID1配置完成 */	
				delay_ms(100);				
				LobotSerialServoMoveConfig(ID2, ID2_BACK, 100);						/* ID2配置完成 */
				delay_ms(100);				
				LobotSerialServoMoveConfig(ID3, ID3_BACK, 100);						/* ID3配置完成 */
				delay_ms(100);				
				LobotSerialServoMoveConfig(ID4, ID4_BACK, 100);						/* ID4配置完成 */		
			}
			/*  5/6号舵机 */
			if(dir_lobnot == 5)															/* ID5/ID6配置完成 */											/* 小叉出 */
			{
				LobotSerialServoMoveConfig(ID7, ID7_OUT, 100);						/* ID1配置完成 */
				delay_ms(100);				
				LobotSerialServoMoveConfig(ID8, ID8_OUT, 100);						/* ID2配置完成 */	
				delay_ms(100);				
				LobotSerialServoMoveConfig(ID9, ID9_OUT, 100);						/* ID3配置完成 */
				delay_ms(100);
				LobotSerialServoMoveConfig(ID10, ID10_OUT, 100);						/* ID4配置完成 */	
			}
			if(dir_lobnot == 6)
			{
				LobotSerialServoMoveConfig(ID7, ID7_BACK, 100);						/* ID1配置完成 */	
				delay_ms(100);				
				LobotSerialServoMoveConfig(ID8, ID8_BACK, 100);						/* ID2配置完成 */	
				delay_ms(100);				
				LobotSerialServoMoveConfig(ID9, ID9_BACK, 100);						/* ID3配置完成 */	
				delay_ms(100);				
				LobotSerialServoMoveConfig(ID10, ID10_BACK, 100);						/* ID4配置完成 */		
			}
			
			/*  5/6号舵机 */
			if(dir_lobnot == 7)															/* ID5/ID6配置完成 */											/* 小叉出 */
			{
				LobotSerialServoMoveConfig(ID13, ID13_OUT, 100);						/* ID1配置完成 */	
				delay_ms(100);				
				LobotSerialServoMoveConfig(ID14, ID14_OUT, 100);						/* ID2配置完成 */
				delay_ms(100);				
				LobotSerialServoMoveConfig(ID15, ID15_OUT, 100);						/* ID3配置完成 */
				delay_ms(100);
				LobotSerialServoMoveConfig(ID16, ID16_OUT, 100);						/* ID4配置完成 */	
				delay_ms(100);				
				LobotSerialServoMoveConfig(ID17, ID17_OUT, 100);						/* ID7配置完成 */
				delay_ms(100);				
				LobotSerialServoMoveConfig(ID18, ID18_OUT, 100);						/* ID8配置完成 */	
				delay_ms(100);				
				LobotSerialServoMoveConfig(ID19, ID19_OUT, 100);						/* ID8配置完成 */
				delay_ms(100);				
				LobotSerialServoMoveConfig(ID20, ID20_OUT, 100);						/* ID8配置完成 */
				backpack_servo_status = 0;
			}
			if(dir_lobnot == 8)															/* ID5/ID6配置完成 */											/* 小叉出 */
			{
				LobotSerialServoMoveConfig(ID13, ID13_BACK, 100);						/* ID1配置完成 */
				delay_ms(100);				
				LobotSerialServoMoveConfig(ID14, ID14_BACK, 100);						/* ID2配置完成 */	
				delay_ms(100);				
				LobotSerialServoMoveConfig(ID15, ID15_BACK, 100);						/* ID3配置完成 */
				delay_ms(100);
				LobotSerialServoMoveConfig(ID16, ID16_BACK, 100);						/* ID4配置完成 */	
				delay_ms(100);				
				LobotSerialServoMoveConfig(ID17, ID17_BACK, 100);						/* ID7配置完成 */
				delay_ms(100);				
				LobotSerialServoMoveConfig(ID18, ID18_BACK, 100);						/* ID8配置完成 */
				delay_ms(100);				
				LobotSerialServoMoveConfig(ID19, ID19_BACK, 100);						/* ID8配置完成 */
				delay_ms(100);				
				LobotSerialServoMoveConfig(ID20, ID20_BACK, 100);						/* ID8配置完成 */	
				backpack_servo_status = 1;
			}
			Lobot2 = 0;
		}
}

void ServoMove_Bigfork12_L_Out(void)													/* 大叉左出 */
{
	LobotSerialServoMoveConfig(ID1, ID1_OUT, 100);						/* ID1配置完成 */
	delay_ms(100);	
	LobotSerialServoMoveConfig(ID2, ID2_OUT, 100);						/* ID2配置完成 */	
	delay_ms(100);	
	LobotSerialServoMoveConfig(ID3, ID3_OUT, 100);						/* ID3配置完成 */
	delay_ms(100);
	LobotSerialServoMoveConfig(ID4, ID4_OUT, 100);						/* ID4配置完成 */	
	memset(&big_fork_servo_state,0,sizeof(big_fork_servo_state));
	DisableServoMotorTimeOut();
	RobotState.BigForkSingGear_LeftOut12_State = 0xff;
}

void ServoMove_Bigfork12_L_Back(void)							/* 大叉左回 */
{
	LobotSerialServoMoveConfig(ID1, ID1_BACK, 100);						/* ID1配置完成 */	
	delay_ms(100);	
	LobotSerialServoMoveConfig(ID2, ID2_BACK, 100);						/* ID2配置完成 */	
	delay_ms(100);	
	LobotSerialServoMoveConfig(ID3, ID3_BACK, 100);						/* ID3配置完成 */	
	delay_ms(100);	
	LobotSerialServoMoveConfig(ID4, ID4_BACK, 100);						/* ID4配置完成 */	
	memset(&big_fork_servo_state,0,sizeof(big_fork_servo_state));
	DisableServoMotorTimeOut();
	RobotState.BigForkSGear_LeftBack12_State = 0xff;
}

void ServoMove_Bigfork34_R_Out(void)														/* 大叉右出 */
{
	LobotSerialServoMoveConfig(ID1, ID1_OUT, 100);						/* ID1配置完成 */	
	delay_ms(100);	
	LobotSerialServoMoveConfig(ID2, ID2_OUT, 100);						/* ID2配置完成 */	
	delay_ms(100);	
	LobotSerialServoMoveConfig(ID3, ID3_OUT, 100);						/* ID3配置完成 */
	delay_ms(100);
	LobotSerialServoMoveConfig(ID4, ID4_OUT, 100);						/* ID4配置完成 */	
	memset(&big_fork_servo_state,0,sizeof(big_fork_servo_state));
	DisableServoMotorTimeOut();	
	RobotState.BigForkSGear_RightOut34_State = 0xff;
}

void ServoMove_Bigfork34_R_Back(void)												/* 大叉右回 */
{
	LobotSerialServoMoveConfig(ID1, ID1_BACK, 100);						/* ID1配置完成 */	
	delay_ms(100);	
	LobotSerialServoMoveConfig(ID2, ID2_BACK, 100);						/* ID2配置完成 */
	delay_ms(100);	
	LobotSerialServoMoveConfig(ID3, ID3_BACK, 100);						/* ID3配置完成 */
	delay_ms(100);
	LobotSerialServoMoveConfig(ID4, ID4_BACK, 100);						/* ID4配置完成 */	
	memset(&big_fork_servo_state,0,sizeof(big_fork_servo_state));	
	DisableServoMotorTimeOut();
	RobotState.BigForkSGear_RightBack34_State = 0xff;
}

void ServoMove_Smallfork56_Out(void)
{
	LobotSerialServoMoveConfig(ID7, ID7_OUT, 100);						/* ID1配置完成 */
	delay_ms(100);	
	LobotSerialServoMoveConfig(ID8, ID8_OUT, 100);						/* ID2配置完成 */
	delay_ms(100);	
	LobotSerialServoMoveConfig(ID9, ID9_OUT, 100);						/* ID3配置完成 */
	delay_ms(100);
	LobotSerialServoMoveConfig(ID10, ID10_OUT, 100);						/* ID4配置完成 */	
	memset(&small_fork_servo_state,0,sizeof(small_fork_servo_state));
	DisableServoMotorTimeOut();
	RobotState.SmallForkSGear_Out56_State = 0xff;
}

void ServoMove_Smallfork56_Back(void)
{
	LobotSerialServoMoveConfig(ID7, ID7_BACK, 100);						/* ID1配置完成 */	
	delay_ms(100);	
	LobotSerialServoMoveConfig(ID8, ID8_BACK, 100);						/* ID2配置完成 */	
	delay_ms(100);	
	LobotSerialServoMoveConfig(ID9, ID9_BACK, 100);						/* ID3配置完成 */
	delay_ms(100);
	LobotSerialServoMoveConfig(ID10, ID10_BACK, 100);						/* ID4配置完成 */	
	memset(&small_fork_servo_state,0,sizeof(small_fork_servo_state));
	DisableServoMotorTimeOut();
	RobotState.SmallForkSGear_Back56_State = 0xff;
}

void ServoMove_BackPack_Out(void)
{
	LobotSerialServoMoveConfig(ID13, ID13_OUT, 100);						/* ID13配置完成 */
	delay_ms(100);	
	LobotSerialServoMoveConfig(ID14, ID14_OUT, 100);						/* ID14配置完成 */
	delay_ms(100);	
	LobotSerialServoMoveConfig(ID15, ID15_OUT, 100);						/* ID15配置完成 */
	delay_ms(100);
	LobotSerialServoMoveConfig(ID16, ID16_OUT, 100);						/* ID16配置完成 */
	delay_ms(100);	
	LobotSerialServoMoveConfig(ID17, ID17_OUT, 100);						/* ID17配置完成 */
	delay_ms(100);
	LobotSerialServoMoveConfig(ID18, ID18_OUT, 100);					/* ID18配置完成 */
	delay_ms(100);
	LobotSerialServoMoveConfig(ID19, ID19_OUT, 100);					/* ID19配置完成 */
	delay_ms(100);
	LobotSerialServoMoveConfig(ID20, ID20_OUT, 100);					/* ID20配置完成 */
	memset(&back_pack_servo_state,0,sizeof(back_pack_servo_state));
	DisableServoMotorTimeOut();
	RobotState.BackPackGear_Out_State = 0xff;
}


void ServoMove_BackPack_Back(void)
{
	LobotSerialServoMoveConfig(ID13, ID13_BACK, 100);						/* ID13配置完成 */
	delay_ms(100);	
	LobotSerialServoMoveConfig(ID14, ID14_BACK, 100);						/* ID14配置完成 */
	delay_ms(100);	
	LobotSerialServoMoveConfig(ID15, ID15_BACK, 100);						/* ID15配置完成 */
	delay_ms(100);
	LobotSerialServoMoveConfig(ID16, ID16_BACK, 100);						/* ID16配置完成 */
	delay_ms(100);	
	LobotSerialServoMoveConfig(ID17, ID17_BACK, 100);						/* ID17配置完成 */
	delay_ms(100);
	LobotSerialServoMoveConfig(ID18, ID18_BACK, 100);					/* ID18配置完成 */
	delay_ms(100);
	LobotSerialServoMoveConfig(ID19, ID19_BACK, 100);					/* ID19配置完成 */
	delay_ms(100);
	LobotSerialServoMoveConfig(ID20, ID20_BACK, 100);					/* ID20配置完成 */
	memset(&back_pack_servo_state,0,sizeof(back_pack_servo_state));
	DisableServoMotorTimeOut();
	RobotState.BackPackGear_Back_State = 0xff;
}

void ScanServoFinishFlag(void)
{
	if(RobotState.BigForkSingGear_LeftOut12_State == 0xff)			/* 大叉左出 */
	{
		if (ServoMotorTimeOut())
		{
			ServoContorlError.BigFork_Servo_Error = 0xff;
			DisableServoMotorTimeOut();
		}
		if (big_fork_servo_state.ID1_state == 0)
		{
			if (MakeServoFinished(ID1, ID1_OUT))
			{
				big_fork_servo_state.ID1_state = 0xff;
			}
		}
		if (big_fork_servo_state.ID2_state == 0)
		{
			if (MakeServoFinished(ID2, ID2_OUT))
			{
				big_fork_servo_state.ID2_state = 0xff;
			}
		}
		if (big_fork_servo_state.ID3_state == 0)
		{
			if (MakeServoFinished(ID3, ID3_OUT))
			{
				big_fork_servo_state.ID3_state = 0xff;
			}
		}
		if (big_fork_servo_state.ID4_state == 0)
		{
			if (MakeServoFinished(ID4, ID4_OUT))
			{
				big_fork_servo_state.ID4_state = 0xff;
			}
		}
		if ((big_fork_servo_state.ID1_state == 0xff) && (big_fork_servo_state.ID2_state == 0xff) && (big_fork_servo_state.ID3_state == 0xff) && (big_fork_servo_state.ID4_state == 0xff))
		{
			RobotFinishFlag.BigForkSGear_LeftOut12_finish = 0xff;
			RobotState.BigForkSingGear_LeftOut12_State = 0;
			DisableServoMotorTimeOut();
		}
	}
	
	if(RobotState.BigForkSGear_RightOut34_State == 0xff)				/* 大叉右出 */
	{
		if (ServoMotorTimeOut())
		{
			ServoContorlError.BigFork_Servo_Error = 0xff;
			DisableServoMotorTimeOut();
		}
		if (big_fork_servo_state.ID1_state == 0)
		{
			if (MakeServoFinished(ID1, ID1_OUT))
			{
				big_fork_servo_state.ID1_state = 0xff;
			}
		}
		if (big_fork_servo_state.ID2_state == 0)
		{
			if (MakeServoFinished(ID2, ID2_OUT))
			{
				big_fork_servo_state.ID2_state = 0xff;
			}
		}
		if (big_fork_servo_state.ID3_state == 0)
		{
			if (MakeServoFinished(ID3, ID3_OUT))
			{
				big_fork_servo_state.ID3_state = 0xff;
			}
		}
		if (big_fork_servo_state.ID4_state == 0)
		{
			if (MakeServoFinished(ID4, ID4_OUT))
			{
				big_fork_servo_state.ID4_state = 0xff;
			}
		}
		if ((big_fork_servo_state.ID1_state == 0xff) && (big_fork_servo_state.ID2_state == 0xff) && (big_fork_servo_state.ID3_state == 0xff) && (big_fork_servo_state.ID4_state == 0xff))
		{
			RobotFinishFlag.BigForkSGear_rightOut34_finish = 0xff;
			RobotState.BigForkSGear_RightOut34_State = 0;
			DisableServoMotorTimeOut();
		}
	}	
	
	if(RobotState.SmallForkSGear_Out56_State == 0xff)						/* 小叉出 */
	{
		if (ServoMotorTimeOut())
		{
			ServoContorlError.SmallFork_Servo_Error = 0xff;
			DisableServoMotorTimeOut();
		}
		if (small_fork_servo_state.ID7_state == 0)
		{
			if (MakeServoFinished(ID7, ID7_OUT))
			{
				small_fork_servo_state.ID7_state = 0xff;
			}
		}
		if (small_fork_servo_state.ID8_state == 0)
		{
			if (MakeServoFinished(ID8, ID8_OUT))
			{
				small_fork_servo_state.ID8_state = 0xff;
			}
		}
		if (small_fork_servo_state.ID9_state == 0)
		{
			if (MakeServoFinished(ID9, ID9_OUT))
			{
				small_fork_servo_state.ID9_state = 0xff;
			}
		}
		if (small_fork_servo_state.ID10_state == 0)
		{
			if (MakeServoFinished(ID10, ID10_OUT))
			{
				small_fork_servo_state.ID10_state = 0xff;
			}
		}
		if ((small_fork_servo_state.ID7_state == 0xff) && (small_fork_servo_state.ID8_state == 0xff) && (small_fork_servo_state.ID9_state == 0xff) && (small_fork_servo_state.ID10_state == 0xff))
		{
			RobotFinishFlag.SmallForkSGear_Out56_finish = 0xff;
			RobotState.SmallForkSGear_Out56_State = 0;
			DisableServoMotorTimeOut();
		}
	}	
	
	if(RobotState.BigForkSGear_LeftBack12_State == 0xff)				/* 大叉左回 */
	{
		if (ServoMotorTimeOut())
		{
			ServoContorlError.BigFork_Servo_Error = 0xff;
			DisableServoMotorTimeOut();
		}
		if (big_fork_servo_state.ID1_state == 0)
		{
			if (MakeServoFinished(ID1, ID1_BACK))
			{
				big_fork_servo_state.ID1_state = 0xff;
			}
		}
		if (big_fork_servo_state.ID2_state == 0)
		{
			if (MakeServoFinished(ID2, ID2_BACK))
			{
				big_fork_servo_state.ID2_state = 0xff;
			}
		}
		if (big_fork_servo_state.ID3_state == 0)
		{
			if (MakeServoFinished(ID3, ID3_BACK))
			{
				big_fork_servo_state.ID3_state = 0xff;
			}
		}
		if (big_fork_servo_state.ID4_state == 0)
		{
			if (MakeServoFinished(ID4, ID4_BACK))
			{
				big_fork_servo_state.ID4_state = 0xff;
			}
		}
		if ((big_fork_servo_state.ID1_state == 0xff) && (big_fork_servo_state.ID2_state == 0xff) && (big_fork_servo_state.ID3_state == 0xff) && (big_fork_servo_state.ID4_state == 0xff))
		{
			RobotFinishFlag.BigForkSGear_LeftBack12_finish = 0xff;
			RobotState.BigForkSGear_LeftBack12_State = 0;
			DisableServoMotorTimeOut();
		}
	}
	
	if(RobotState.BigForkSGear_RightBack34_State == 0xff)				/* 大叉右回 */
	{
		if (ServoMotorTimeOut())
		{
			ServoContorlError.BigFork_Servo_Error = 0xff;
			DisableServoMotorTimeOut();
		}
		if (big_fork_servo_state.ID1_state == 0)
		{
			if (MakeServoFinished(ID1, ID1_BACK))
			{
				big_fork_servo_state.ID1_state = 0xff;
			}
		}
		if (big_fork_servo_state.ID2_state == 0)
		{
			if (MakeServoFinished(ID2, ID2_BACK))
			{
				big_fork_servo_state.ID2_state = 0xff;
			}
		}
		if (big_fork_servo_state.ID3_state == 0)
		{
			if (MakeServoFinished(ID3, ID3_BACK))
			{
				big_fork_servo_state.ID3_state = 0xff;
			}
		}
		if (big_fork_servo_state.ID4_state == 0)
		{
			if (MakeServoFinished(ID4, ID4_BACK))
			{
				big_fork_servo_state.ID4_state = 0xff;
			}
		}
		if ((big_fork_servo_state.ID1_state == 0xff) && (big_fork_servo_state.ID2_state == 0xff) && (big_fork_servo_state.ID3_state == 0xff) && (big_fork_servo_state.ID4_state == 0xff))
		{
			RobotFinishFlag.BigForkSGear_rightBack34_finish = 0xff;
			RobotState.BigForkSGear_RightBack34_State = 0;
			DisableServoMotorTimeOut();
		}
	}	
	
	if(RobotState.SmallForkSGear_Back56_State == 0xff)					/* 小叉回 */
	{
		if (ServoMotorTimeOut())
		{
			ServoContorlError.SmallFork_Servo_Error = 0xff;
			DisableServoMotorTimeOut();
		}
		if (small_fork_servo_state.ID7_state == 0)
		{
			if (MakeServoFinished(ID7, ID7_BACK))
			{
				small_fork_servo_state.ID7_state = 0xff;
			}
		}
		if (small_fork_servo_state.ID8_state == 0)
		{
			if (MakeServoFinished(ID8, ID8_BACK))
			{
				small_fork_servo_state.ID8_state = 0xff;
			}
		}
		if (small_fork_servo_state.ID9_state == 0)
		{
			if (MakeServoFinished(ID9, ID9_BACK))
			{
				small_fork_servo_state.ID9_state = 0xff;
			}
		}
		if (small_fork_servo_state.ID10_state == 0)
		{
			if (MakeServoFinished(ID10, ID10_BACK))
			{
				small_fork_servo_state.ID10_state = 0xff;
			}
		}
		if ((small_fork_servo_state.ID7_state == 0xff) && (small_fork_servo_state.ID8_state == 0xff) && (small_fork_servo_state.ID9_state == 0xff) && (small_fork_servo_state.ID10_state == 0xff))
		{
			RobotFinishFlag.SmallForkSGear_Back56_finish = 0xff;
			RobotState.SmallForkSGear_Back56_State = 0;
			DisableServoMotorTimeOut();
		}
	}
	
	
	if(RobotState.BackPackGear_Back_State == 0xff)
	{
		if (ServoMotorTimeOut())
		{
			ServoContorlError.BackPack_Servo_Error = 0xff;
			DisableServoMotorTimeOut();
		}
		if (back_pack_servo_state.ID13_state == 0)
		{
			if (MakeServoFinished(ID13, ID13_BACK))
			{
				back_pack_servo_state.ID13_state = 0xff;
			}
		}
		if (back_pack_servo_state.ID14_state == 0)
		{
			if (MakeServoFinished(ID14, ID14_BACK))
			{
				back_pack_servo_state.ID14_state = 0xff;
			}
		}
		if (back_pack_servo_state.ID15_state == 0)
		{
			if (MakeServoFinished(ID15, ID15_BACK))
			{
				back_pack_servo_state.ID15_state = 0xff;
			}
		}
		if (back_pack_servo_state.ID16_state == 0)
		{
			if (MakeServoFinished(ID16, ID16_BACK))
			{
				back_pack_servo_state.ID16_state = 0xff;
			}
		}
		if (back_pack_servo_state.ID17_state == 0)
		{
			if (MakeServoFinished(ID17, ID17_BACK))
			{
				back_pack_servo_state.ID17_state = 0xff;
			}
		}
		if (back_pack_servo_state.ID18_state == 0)
		{
			if (MakeServoFinished(ID18, ID18_BACK))
			{
				back_pack_servo_state.ID18_state = 0xff;
			}
		}
		if (back_pack_servo_state.ID19_state == 0)
		{
			if (MakeServoFinished(ID19, ID19_BACK))
			{
				back_pack_servo_state.ID19_state = 0xff;
			}
		}
		if (back_pack_servo_state.ID20_state == 0)
		{
			if (MakeServoFinished(ID20, ID20_BACK))
			{
				back_pack_servo_state.ID20_state = 0xff;
			}
		}
		if((back_pack_servo_state.ID13_state == 0xff) && (back_pack_servo_state.ID14_state == 0xff) && (back_pack_servo_state.ID15_state == 0xff) && 
			(back_pack_servo_state.ID16_state == 0xff) && (back_pack_servo_state.ID17_state == 0xff) && (back_pack_servo_state.ID18_state == 0xff) && 
			(back_pack_servo_state.ID19_state == 0xff) && (back_pack_servo_state.ID20_state == 0xff))
		{
			RobotFinishFlag.BackPackGear_Back_finish = 0xff;
			RobotState.BackPackGear_Back_State = 0;
			backpack_servo_status = 1;
			DisableServoMotorTimeOut();
		}
	}
	
	if(RobotState.BackPackGear_Out_State == 0xff)
	{
		if (ServoMotorTimeOut())
		{
			ServoContorlError.BackPack_Servo_Error = 0xff;
			DisableServoMotorTimeOut();
		}
		if (back_pack_servo_state.ID13_state == 0)
		{
			if (MakeServoFinished(ID13, ID13_OUT))
			{
				back_pack_servo_state.ID13_state = 0xff;
			}
		}
		if (back_pack_servo_state.ID14_state == 0)
		{
			if (MakeServoFinished(ID14, ID14_OUT))
			{
				back_pack_servo_state.ID14_state = 0xff;
			}
		}
		if (back_pack_servo_state.ID15_state == 0)
		{
			if (MakeServoFinished(ID15, ID15_OUT))
			{
				back_pack_servo_state.ID15_state = 0xff;
			}
		}
		if (back_pack_servo_state.ID16_state == 0)
		{
			if (MakeServoFinished(ID16, ID16_OUT))
			{
				back_pack_servo_state.ID16_state = 0xff;
			}
		}
		if (back_pack_servo_state.ID17_state == 0)
		{
			if (MakeServoFinished(ID17, ID17_OUT))
			{
				back_pack_servo_state.ID17_state = 0xff;
			}
		}
		if (back_pack_servo_state.ID18_state == 0)
		{
			if (MakeServoFinished(ID18, ID18_OUT))
			{
				back_pack_servo_state.ID18_state = 0xff;
			}
		}
		if (back_pack_servo_state.ID19_state == 0)
		{
			if (MakeServoFinished(ID19, ID19_OUT))
			{
				back_pack_servo_state.ID19_state = 0xff;
			}
		}
		if (back_pack_servo_state.ID20_state == 0)
		{
			if (MakeServoFinished(ID20, ID20_OUT))
			{
				back_pack_servo_state.ID20_state = 0xff;
			}
		}
		if ((back_pack_servo_state.ID13_state == 0xff) && (back_pack_servo_state.ID14_state == 0xff) && (back_pack_servo_state.ID15_state == 0xff) && 
			(back_pack_servo_state.ID16_state == 0xff) && (back_pack_servo_state.ID17_state == 0xff) && (back_pack_servo_state.ID18_state == 0xff) && 
			(back_pack_servo_state.ID19_state == 0xff) && (back_pack_servo_state.ID20_state == 0xff))
		{
			RobotFinishFlag.BackPackGear_Out_finish = 0xff;
			RobotState.BackPackGear_Out_State = 0;
			backpack_servo_status = 0;
			DisableServoMotorTimeOut();
		}
	}
}


bool MakeServoFinished(int servoid, int position)
{
	int current_position = LobotSerialServoReadPosition_new((uint8_t) servoid);
	if(abs(current_position - position) <= FloatWindow_SG)
	{
		return true;
	}
	else
	{
		LobotSerialServoMoveConfig(servoid, position, 100);
		return false;
	}
}


void LobotSerialServoSetID_new(uint8_t oldID, uint8_t newID)												/* 写ID */
{
	uint8_t buf[8];
  buf[0] = buf[1] = 0xff;
  buf[1] = buf[1] = 0xff;	
  buf[2] = oldID;
  buf[3] = 0x04;
  buf[4] = 0x03;
  buf[5] = 0x05;
  buf[6] = newID;	
  buf[7] = LobotCheckSum_new(buf);
  LobotSerialWrite(buf, 8);
}




void LobotSerialServoReadID_new(void)												/* 读ID */
{
	uint8_t buf[8];
  buf[0] = buf[1] = 0xff;
  buf[1] = buf[1] = 0xff;	
  buf[2] = 0xFE;
  buf[3] = 0x04;
  buf[4] = 0x02;
  buf[5] = 0x05;
  buf[6] = 0x01;	
  buf[7] = LobotCheckSum_new(buf);
  LobotSerialWrite(buf, 8);
}

/* 控制电机 */

void LobotSerialServoMoveConfig(uint8_t id, int16_t position, uint16_t time)													/* 新舵机舵机动作 */
{
	uint8_t buf[11];
	if(position < 0)
  position = 0;
  if(position > 4351)
	position = 4351;
	buf[0] = buf[1] = 0xff;
  buf[2] = id;
  buf[3] = 7;
  buf[4] = 0x03;																			/* 指令类型 */
	buf[5] = 0x2A;																				/* 异步写指令地址 */
  buf[6] = GET_HIGH_BYTE(position);
  buf[7] = GET_LOW_BYTE(position);
  buf[8] = GET_HIGH_BYTE(time);
  buf[9] = GET_LOW_BYTE(time);
  buf[10] = LobotCheckSum_new(buf);
  LobotSerialWrite(buf, 11);
}


void	LobotSerialServoMove_new(void)
{
	uint8_t buf[6];
	buf[0] = buf[1] = 0xff;
	buf[2] = 0xfe;
	buf[3] = 0x02;
	buf[4] = 0x05;
	buf[5] = LobotCheckSum_new(buf);
	LobotSerialWrite(buf, 6);
}


int LobotSerialServoReadPosition_new(uint8_t id)
{
	int ret = 0;
  static uint8_t buf[8];

  buf[0] = buf[1] = 0xff;
  buf[2] = id;
  buf[3] = 0x04;
  buf[4] = 0x02;
  buf[5] = 0x38;																			/* 读位置地址 */
  buf[6] = 0x02;	
	buf[7] = LobotCheckSum_new(buf);
	LobotSerialWrite(buf, 8);
	ret = LobotSerialMsgHandle_new(id);
	return ret;
}



uint8_t LobotCheckSum_new(uint8_t *buf)
{
  uint8_t i;
  uint16_t temp = 0;
  for (i = 2; i < buf[3] + 3; i++) {
    temp = buf[i] + temp;
  }
  temp = (~temp)& 0xff;
  i = (uint8_t)temp;
  return i;
}



