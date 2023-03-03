#ifndef _SOFTTIMER_H
#define _SOFTTIMER_H
#include "main.h"
#include "sys.h"

#define    TIMER_        27


#define BigForkSpeedUp(x)								timera[0][x]			/* 大叉加速 */
#define BigForkSpeedCst(x)							timera[1][x]			/* 大叉减速 */
#define BigForkSpeedDown(x)							timera[2][x]			/* 大叉匀速 */
#define SmallForkSpeedUp(x)							timera[3][x]			/* 小叉加速 */
#define SmallForkSpeedCst(x)						timera[4][x]			/* 小叉减速 */
#define SmallForkSpeedDown(x)						timera[5][x]			/* 小叉匀速 */
#define RotateSpeedUp(x)								timera[6][x]			/* 旋转加速 */
#define RotateSpeedCst(x)								timera[7][x]			/* 旋转减速 */
#define RotateSpeedDown(x)							timera[8][x]			/* 旋转匀速 */
#define RS485_LIM_TIM(x)								timera[9][x]			/* 485保护定时器 */
#define BigForkTIM(x)										timera[10][x]			/* 大叉控制用 */
#define SmallForkTIM(x)									timera[11][x]			/* 小叉控制用 */
#define HomeTIM_BigFork(x)							timera[12][x]			/* 大叉原点控制用 */
#define HomeTIM_SmallFork(x)						timera[13][x]			/* 小叉原点控制用 */
#define modeOfOperation_Init(x)					timera[14][x]			/* CANOPEN清除故障，初始化用 */
#define lOBOTsERIALpOSITION(x)					timera[15][x]			/* 舵机位置读取 */

#define BigForkReturnHome(x)						timera[16][x]			/* 大叉回原优化时间16 */
#define SmallForkReturnHome(x)					timera[17][x]			/* 小叉回原优化时间17 */
#define RotateReturnHome(x)							timera[18][x]			/* 旋转回原优化时间18 */

#define BigForkReturnHome1(x)						timera[19][x]			/* 大叉回原优化时间19 */
#define SmallForkReturnHome1(x)					timera[20][x]			/* 小叉回原优化时间20 */
#define RotateReturnHome1(x)						timera[21][x]			/* 旋转回原优化时间21 */
#define ServoMotor(x)						        timera[22][x]			/* 舵机超时检测22 */
#define RotateMotorPoistion(x)					timera[23][x]			/* 旋转电机控制超回23 */
#define TenMsTimer(x)					timera[24][x]			/* 10ms定时24 */
#define FiveMsTimer(x)					timera[25][x]			/* 5ms定时25 */
#define OneMsTimer(x)					timera[26][x]			/* 1ms定时26 */


void softTimer_Init(u16 arr,u16 psc);
void timerp(void);
void	ScanSoftTimerState(void);

bool ServoMotorTimeOut(void);
void DisableServoMotorTimeOut(void);


extern int32_t timera[TIMER_][4];
#endif			/* _SOFTTIMER_H */
