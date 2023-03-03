#ifndef __MOTORCONTRAL_H
#define __MOTORCONTRAL_H
#include "main.h"
#include "sys.h"

/* motorState */
#define     UP									1
#define     CONSTANT						2
#define     DOWN								3
#define     WAIT								0

/* �˶����� */
#define  spr    								1600     					/* ���������תһȦ������ */
#define	 Pi											3.14   							/* Բ���� */
#define  ft											(84000000/21)				/* ����Ƶ�� */
#define  count_speedPara				(ft / spr)			/* ����ֵ���ٶȵ�ת��ϵ��  200 */
#define  startSpeed_count 			2500								/* ��ʼ�ٶȼ���ֵ 0.5Ȧ/s */
#define  startSpeed 						1
#define  accel_T								1										/* �Ӽ�������  ��λms */

/* �˶�ģʽ */
#define	 acc_dec								0					/* �޷��ﵽ���ʱ�ͼ��� */
#define	 acc_constant_dec		    1					/* �������ٺͼ��� */

/* PWM��ʱ���궨�� */

#define		TIM_BigFork						TIM4
#define		TIM_SmallFork					TIM3
#define		TIM_Rotate						TIM8


/* ������������ timePara */
#define		BigFork_Num						0
#define		SmallFork_Num					3
#define		Rotate_Num						6

/* ���� */
#define 	Positive							0									/* ��ʱ�� */			//��С����������		����ת����
#define 	negative							1									/* ˳ʱ�� */			//��С����������		����ת����



/* ��ԭ���� */
#define		ReturnHome_Left				1									/* �����ԭ */
#define		ReturnHome_Right			2									/* ���һ�ԭ */
	


/* ���������һ�����з��� */
#define		Last_Left							1
#define		Last_Right						0



/* ��������˶����� */

#define		SteptBigforkPositionCompensation						0.053
#define		SteptSmallforkPositionCompensation					0.053
#define		SteptRotatePositionCompensation							0.053
#define		SteptRotateOvershootPosiition 							0.5

struct StepMotorInitDef
{
	u8 	moveMode;											/* �˶�ģʽ:(0).�޷��ﵽ���ʱ�ͼ��٣�1�����������ٺͼ��� */
	u8  motorState;										/* ���״̬ */
	u32 accel_step;										/* ���ٲ��� */
	u32 decel_step;										/* ���ٲ��� */
	u32 constant_step;								/* ���ٲ��� */
	u32 max_speed_count;							/* ���ٵ�����ٶȵļ���ֵ */
	double countRate_Acc;							/* ���������ٵı仯�� */
	double countRate_Dec;							/* ���������ٵı仯�� */
	u32 allStep;											/* �ܲ��� */
	float constantTime;								/* ����ʱ�� */
	float accelTime;									/* ����ʱ�� */
	float decelTime;									/* ����ʱ�� */
	double TIMCount_Acc;							/* ������ټ��� */
	double TIMCount_Dec;							/* ������ټ��� */
	float acc_dec_t;									/* �޷��ﵽ����ٶ�ʱ�ļ���ʱ�� */
};

extern struct StepMotorInitDef  StepSmallFork;
extern struct StepMotorInitDef  StepBigFork;
extern struct StepMotorInitDef  StepRotateFork;




struct StepReturnHome
{
	float	ReturnHome_Position;
	float	ReturnHome_Accel;
	float	ReturnHome_Decel;	
	float	ReturnHome_Speed;
};
extern struct StepReturnHome	BigFork_ReturnHome,SmallFork_Return,Rotate_Return;








/* 
		����100Ϊ1sһȦ  ����ʱ����21/84M*20000 * 100 = 0.5S ����ռ�ձ�50��
		��������ά��ʱ��Ҳ��Ҫ0.5s,�ʼ���ֵΪ100ʱ��1Ȧ/s 
*/




struct StepMotorInitDef stepMotor_Calculate(u32 step,float accel,float decel, float speed);
u32 compute_count(u16 c , u16 n);
u32 CountSpeedChange(float speed);
float SpeedCountChange(u32 count);
float FunLine(float y0,float k,float x);
float FunLine_dec(float y0,float k,float x);
void SteptMotor_SoferTime_Big(u8 timePara, TIM_TypeDef *TIM_NUM);
void SteptMotor_SoferTime_Small(u8 timePara, TIM_TypeDef *TIM_NUM);
void SteptMotor_SoferTime_Rotate(u8 timePara, TIM_TypeDef *TIM_NUM);
void StepMotor_PWM(u8 timePara);
void	BigFork_SteptMotor_Start(float Position,float speed,float acc, float dec, u8 dir);
void	SmallFork_SteptMotor_Start(float Position,float speed,float acc, float dec, u8 dir);
void	Rotate_SteptMotor_Start(float Position,float speed,float acc, float dec, u8 dir);

void SteptBigFork_ReturnHome(void);
void BigFork_Extend_Left(void);																/* ���������չ */
void BigFork_Shrink_Left(void);																/* ����������� */
void BigFork_Extend_Right(void);																/* ���������չ */
void BigFork_Shrink_Right(void);																/* ����������� */

void SmallFork_Extend_Left(void);																/* С����չ */
void SmallFork_Shrink_Left(void);																/* С������ */
void SteptSmallFork_ReturnHome(void);														/* С���ԭ */


/* ���� */
void Rotate_Left_Out(void);																	/* ����ת��*/
void Rotate_Left_Back(void);																/* ����ת��*/

void Rotate_Right_Out(void);																/* ����ת�� */
void Rotate_Right_Back(void);																/* ����ת�� */

void Rotate_Behind_Out_left(void);																	/* ����ת����*/
void Rotate_Behind_Back_left(void);																	/* ����ת�غ�*/
void Rotate_Behind_Out_right(void);																	/* ����ת����*/
void Rotate_Behind_Back_right(void);																	/* ����ת����*/
		

void Rotate_ReturnHome(void);																/* ��ת��ԭ */


void	StepBigFork_Left_ReturnHomeFun(void);
void	StepBigFork_Right_ReturnHomeFun(void);

#endif			/* __MOTORCONTRAL_H */
