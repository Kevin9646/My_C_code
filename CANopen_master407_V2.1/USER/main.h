#ifndef _MAIN_H
#define	_MAIN_H


#include "stm32f4xx.h"
#include "sys.h"
#include "delay.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "usart.h"
#include "sensor.h"
#include "protocol.h"
#include "gpio.h"
#include "Control.h"
#include "stm32f4xx_it.h"
#include "error.h"

/* 步进电机脉冲 */
#include "PWMTimer.h"
#include "softtimer.h"
#include "motorContral.h"
/*****CANopen 相关头文件********/

#include "CanopenCan.h"
#include "CanopenTimer.h"
#include "data.h"
#include "Master.h"
#include "canfestival.h"
#include "CanopenContral.h"							/* canopen控制文件 */


/* 内部FLASH */
#include "bsp_internalFlash.h"



/* 舵机 */
#include "LobotSerialServo.h"





extern u16 DD1;
extern u16 DD2;
extern u16 DD3;
extern u16 DD4;
extern u16 DD5;
extern u16 DD6;






#endif


