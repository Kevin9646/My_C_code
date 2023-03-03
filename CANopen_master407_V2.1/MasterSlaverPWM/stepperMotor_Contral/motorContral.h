#ifndef __MOTORCONTRAL_H
#define __MOTORCONTRAL_H
#include "main.h"
#include "sys.h"

/* motorState */
#define     UP									1
#define     CONSTANT						2
#define     DOWN								3
#define     WAIT								0

/* 运动参数 */
#define  spr    								1600     					/* 步进电机旋转一圈脉冲数 */
#define	 Pi											3.14   							/* 圆周率 */
#define  ft											(84000000/21)				/* 计数频率 */
#define  count_speedPara				(ft / spr)			/* 计数值和速度的转换系数  200 */
#define  startSpeed_count 			2500								/* 起始速度计数值 0.5圈/s */
#define  startSpeed 						1
#define  accel_T								1										/* 加减速周期  单位ms */

/* 运动模式 */
#define	 acc_dec								0					/* 无法达到最高时就减速 */
#define	 acc_constant_dec		    1					/* 加速匀速和减速 */

/* PWM定时器宏定义 */

#define		TIM_BigFork						TIM4
#define		TIM_SmallFork					TIM3
#define		TIM_Rotate						TIM8


/* 步进电机辨别编号 timePara */
#define		BigFork_Num						0
#define		SmallFork_Num					3
#define		Rotate_Num						6

/* 方向 */
#define 	Positive							0									/* 逆时针 */			//大小叉伸缩向右		，旋转向右
#define 	negative							1									/* 顺时针 */			//大小叉伸缩向左		，旋转向左



/* 回原方向 */
#define		ReturnHome_Left				1									/* 向左回原 */
#define		ReturnHome_Right			2									/* 向右回原 */
	


/* 步进电机上一次运行方向 */
#define		Last_Left							1
#define		Last_Right						0



/* 步进电机运动补偿 */

#define		SteptBigforkPositionCompensation						0.053
#define		SteptSmallforkPositionCompensation					0.053
#define		SteptRotatePositionCompensation							0.053
#define		SteptRotateOvershootPosiition 							0.5

struct StepMotorInitDef
{
	u8 	moveMode;											/* 运动模式:(0).无法达到最高时就减速（1）、加速匀速和减速 */
	u8  motorState;										/* 电机状态 */
	u32 accel_step;										/* 加速步数 */
	u32 decel_step;										/* 减速步数 */
	u32 constant_step;								/* 匀速步数 */
	u32 max_speed_count;							/* 加速到最大速度的计数值 */
	double countRate_Acc;							/* 计数器加速的变化率 */
	double countRate_Dec;							/* 计数器加速的变化率 */
	u32 allStep;											/* 总步数 */
	float constantTime;								/* 匀速时间 */
	float accelTime;									/* 加速时间 */
	float decelTime;									/* 减速时间 */
	double TIMCount_Acc;							/* 电机加速计数 */
	double TIMCount_Dec;							/* 电机减速计数 */
	float acc_dec_t;									/* 无法达到最大速度时的加速时间 */
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
		计数100为1s一圈  脉冲时间间隔21/84M*20000 * 100 = 0.5S 由于占空比50，
		所以脉冲维持时间也需要0.5s,故计数值为100时，1圈/s 
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
void BigFork_Extend_Left(void);																/* 大叉向左伸展 */
void BigFork_Shrink_Left(void);																/* 大叉由左收缩 */
void BigFork_Extend_Right(void);																/* 大叉向右伸展 */
void BigFork_Shrink_Right(void);																/* 大叉由右收缩 */

void SmallFork_Extend_Left(void);																/* 小叉伸展 */
void SmallFork_Shrink_Left(void);																/* 小叉收缩 */
void SteptSmallFork_ReturnHome(void);														/* 小叉回原 */


/* 步进 */
void Rotate_Left_Out(void);																	/* 向左转出*/
void Rotate_Left_Back(void);																/* 由左转回*/

void Rotate_Right_Out(void);																/* 向右转出 */
void Rotate_Right_Back(void);																/* 向右转回 */

void Rotate_Behind_Out_left(void);																	/* 向左转出后*/
void Rotate_Behind_Back_left(void);																	/* 向左转回后*/
void Rotate_Behind_Out_right(void);																	/* 向右转出后*/
void Rotate_Behind_Back_right(void);																	/* 向右转出后*/
		

void Rotate_ReturnHome(void);																/* 旋转回原 */


void	StepBigFork_Left_ReturnHomeFun(void);
void	StepBigFork_Right_ReturnHomeFun(void);

#endif			/* __MOTORCONTRAL_H */
