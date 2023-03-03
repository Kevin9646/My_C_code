#ifndef _SOFTTIMER_H
#define _SOFTTIMER_H
#include "main.h"
#include "sys.h"

#define    TIMER_        27


#define BigForkSpeedUp(x)								timera[0][x]			/* ������ */
#define BigForkSpeedCst(x)							timera[1][x]			/* ������ */
#define BigForkSpeedDown(x)							timera[2][x]			/* ������� */
#define SmallForkSpeedUp(x)							timera[3][x]			/* С����� */
#define SmallForkSpeedCst(x)						timera[4][x]			/* С����� */
#define SmallForkSpeedDown(x)						timera[5][x]			/* С������ */
#define RotateSpeedUp(x)								timera[6][x]			/* ��ת���� */
#define RotateSpeedCst(x)								timera[7][x]			/* ��ת���� */
#define RotateSpeedDown(x)							timera[8][x]			/* ��ת���� */
#define RS485_LIM_TIM(x)								timera[9][x]			/* 485������ʱ�� */
#define BigForkTIM(x)										timera[10][x]			/* �������� */
#define SmallForkTIM(x)									timera[11][x]			/* С������� */
#define HomeTIM_BigFork(x)							timera[12][x]			/* ���ԭ������� */
#define HomeTIM_SmallFork(x)						timera[13][x]			/* С��ԭ������� */
#define modeOfOperation_Init(x)					timera[14][x]			/* CANOPEN������ϣ���ʼ���� */
#define lOBOTsERIALpOSITION(x)					timera[15][x]			/* ���λ�ö�ȡ */

#define BigForkReturnHome(x)						timera[16][x]			/* ����ԭ�Ż�ʱ��16 */
#define SmallForkReturnHome(x)					timera[17][x]			/* С���ԭ�Ż�ʱ��17 */
#define RotateReturnHome(x)							timera[18][x]			/* ��ת��ԭ�Ż�ʱ��18 */

#define BigForkReturnHome1(x)						timera[19][x]			/* ����ԭ�Ż�ʱ��19 */
#define SmallForkReturnHome1(x)					timera[20][x]			/* С���ԭ�Ż�ʱ��20 */
#define RotateReturnHome1(x)						timera[21][x]			/* ��ת��ԭ�Ż�ʱ��21 */
#define ServoMotor(x)						        timera[22][x]			/* �����ʱ���22 */
#define RotateMotorPoistion(x)					timera[23][x]			/* ��ת������Ƴ���23 */
#define TenMsTimer(x)					timera[24][x]			/* 10ms��ʱ24 */
#define FiveMsTimer(x)					timera[25][x]			/* 5ms��ʱ25 */
#define OneMsTimer(x)					timera[26][x]			/* 1ms��ʱ26 */


void softTimer_Init(u16 arr,u16 psc);
void timerp(void);
void	ScanSoftTimerState(void);

bool ServoMotorTimeOut(void);
void DisableServoMotorTimeOut(void);


extern int32_t timera[TIMER_][4];
#endif			/* _SOFTTIMER_H */
