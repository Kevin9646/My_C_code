#ifndef __CONTROL_H
#define __CONTROL_H
#include "main.h"




/* ��λ������ָ�� */
struct FromAGVCommand
{
	u8  TakePut;									/* ȡ�� */
	u8	Dir;											/* ���� */
	u8	Bin_Floor;								/* ������� */
	u8	BackPackFloor;						/* ��¨���� */
	u8  Capture;                  /* ����*/
};
extern struct FromAGVCommand LiftAllAction;


struct FromAGVReceive
{
	u8  TakePut_receive;									/* ȡ�� */
	u8	Dir_receive;											/* ���� */
	u8	Bin_Floor_receive;								/* ������� */
	u8	BackPack_Floor_receive;						/* ��¨���� */
	u8  Capture_recerive;                 /* ���� */
};
extern struct FromAGVReceive AGVReceive;



/* canopen����ָ��ṹ�� */
struct LiftControl_Info
{
	u8 motor;
	u8 MoveMode;
	int32_t targetPosition;
	u32 profileV;
};

struct SteptControl_Info
{
	u32 pulse;
	float accV;
	float decV;
	float	speed;
	u8 	dir;
	u8	startUp;					/* ������־λ */
};



struct Robot_Finish_Flag
{
	u8	ReturnHome_State_finish[5];									 	  /* �ҵ�ԭ�� */
	u8	AllReturnHome_Finish;														/* ���л�ԭ��� */
	u8	BigFork_Extend_left_finish;											/* ���������� */	
	u8	BigFork_Shrink_left_finish;											/* ������� */		
	u8	BigFork_Extend_right_finish;										/* ������� */		
	u8	BigFork_Shrink_right_finish;										/* ������� */			
	u8	SmallFork_Extend_finish;												/* С����չ */	
	u8	SmallFork_Shrink_finish;												/* С������ */	
	u8	Rotate_out_left_finish;													/* ��ת��� */	
	u8	Rotate_back_left_finish;											  /* ��ת��� */		
	u8	Rotate_out_right_finish;											  /* ��ת�ҳ� */	
	u8	Rotate_back_right_finish;										    /* ��ת�һ� */
	u8  Rotate_out_behind_left_finish;                  /* ��ת��� */
	u8  Rotate_back_behind_left_finish;                 /* ��ת���� */
	u8  Rotate_out_behind_right_finish;                  /* ��ת��� */
	u8  Rotate_back_behind_right_finish;                 /* ��ת���� */
	
	u8	BigForkSGear_LeftOut12_finish;							/* ������������ */
	u8	BigForkSGear_rightOut34_finish;							/* ����ҳ������� */	
	u8	SmallForkSGear_Out56_finish;										/* С��������� */	
	u8  BackPackGear_Out_finish;                    /* ��¨�������� */
	
	u8	BigForkSGear_LeftBack12_finish;							/* ���������� */
	u8	BigForkSGear_rightBack34_finish;							/* ����Ҷ����� */	
	u8	SmallForkSGear_Back56_finish;										/* С������� */	
	u8  BackPackGear_Back_finish;                      /* ��¨�������� */
	
	u8	LiftBigFork_PositionArrive;						      /* ���λ�õ���  9:�ϼ���  10���¼��� */
	u8	LiftSmallFork_PositionArrive;					    	/* С��λ�õ��� 9���ϼ��� ��10���¼��� */		
};
extern struct Robot_Finish_Flag RobotFinishFlag;





struct Robot_Move_State
{
	u8	ReturnHome_State[5];												/* ��ԭ:0�����������1��С��������2��С����ת��3�����������4��С������     0xff:��ԭ״̬��0���ǻ�ԭ״̬*/
	u8	ControlState;																/* 0xff:����״̬��0��������״̬ */	
	u8	BigFork_Extend_left;												/* ������� */
	u8	BigFork_Shrink_left;												/* ������� */
	u8	BigFork_Extend_right;												/* ������� */
	u8	BigFork_Shrink_right;												/* ������� */	
	u8	SmallFork_Extend;														/* С����չ */
	u8	SmallFork_Shrink;														/* С������ */
	u8	Rotate_out_left;														/* ��ת��� */
	u8	Rotate_back_left;														/* ��ת��� */	
	u8	Rotate_out_right;														/* ��ת�ҳ� */
	u8	Rotate_back_right;													/* ��ת�һ� */
	u8	Rotate_behind_out_left;											/* ��ת����� */
	u8	Rotate_behind_back_left;										/* ��ת������*/
	u8	Rotate_behind_out_right;										/* ��ת����� */
	u8	Rotate_behind_back_right;										/* ��ת������*/		
	u8	BigForkBlindFindHome;												/* ���ä��ԭ�� */
	u8	RotateBlindFindHome;												/* ��תä��ԭ�� */
	
	u8	LiftBigFork_Move_State;											/* ��������˶� */
	u8	LiftSmallFork_move_State;										/* С�������˶� */
	
	u8	TakeUpXFloorBin_State[8];										/* ץȡ�ڼ������� */
	u8	PutDownXFloorBin_State[8];									/* ж�䵽�ڼ��� */
	u8	TakeUPXFloorBackPack_State[4];							/* ץȡ�ڼ��㱳¨���� */
	u8	PutDownXFloorBackPack_State[4];							/* ��������ڱ�¨�ڼ��� */
	
	u8	BigForkSingGear_LeftOut12_State;						/* ��������� */
	u8	BigForkSGear_RightOut34_State;						/* ����ҳ���� */	
	u8	SmallForkSGear_Out56_State;								/* С������ */
	u8  BackPackGear_Out_State;                   /* ��¨����� */
	
	u8	BigForkSGear_LeftBack12_State;							/* �����ض�� */
	u8	BigForkSGear_RightBack34_State;						/* ����һض�� */	
	u8	SmallForkSGear_Back56_State;								/* С��ض�� */
	u8  BackPackGear_Back_State;                   /* ��¨�ض�� */
	
	u8	TakeStept1;																	/* ȡ���1�� */	
	u8	TakeStept2;																	/* ȡ���2�� */		
	u8	TakeStept3;																	/* ȡ���3�� */		
	u8	TakeStept4;																	/* ȡ���4�� */		
	u8	TakeStept5;																	/* ȡ���5�� */	
	u8	TakeStept6;																	/* ȡ���6�� */	
	u8	TakeStept7;																	/* ȡ���7�� */	
	u8	TakeStept8;																	/* ȡ���8�� */	
	u8	TakeStept9;																	/* ȡ���9�� */	
	u8  TakeStept10;                                /* ȡ���10�� */
	u8  TakeStept_Simple_1;                         /*  ��治��ȡ�䵥������1 */
	u8  TakeStept_Simple_2;                         /*  ��治��ȡ�䵥������2 */
	u8  TakeStept_Simple_3;                         /*  ��治��ȡ�䵥������3 */
	u8  TakeStept_Simple_4;                         /*  ��治��ȡ�䵥������4*/
	u8  TakeStept_Simple_5;                         /*  ��治��ȡ�䵥������5 */
	
	u8  TakeStept_eight_floor_1;                     /*  ��8���治��ȡ�䵥������1 */
	u8  TakeStept_eight_floor_2;                     /*  ��8���治��ȡ�䵥������2 */
	u8  TakeStept_eight_floor_3;                     /*  ��8���治��ȡ�䵥������2 */
	
	u8	PutStept1;																	/* ж���1�� */	
	u8	PutStept2;																	/* ж���2�� */		
	u8	PutStept3;																	/* ж���3�� */		
	u8	PutStept4;																	/* ж���4�� */		
	u8	PutStept5;																	/* ж���5�� */			
	u8	PutStept6;																	/* ж���6�� */		
	u8	PutStept7;																	/* ж���7�� */				
	u8	PutStept8;																	/* ж���8�� */
	u8  PutStept9;
	u8  PutStept_Simple_1;                         /*  ��治�����䵥������1 */
	u8  PutStept_Simple_2;                         /*  ��治�����䵥������2 */
	u8  PutStept_Simple_3;                         /*  ��治�����䵥������3 */
	u8  PutStept_Simple_4;                         /*  ��治�����䵥������4 */
	u8  PutStept_Simple_5;                         /*  ��治�����䵥������5 */
	
	u8  PutStept_eight_floor_1;                     /*  ��8���治��ȡ�䵥������1 */
	u8  PutStept_eight_floor_2;                     /*  ��8���治��ȡ�䵥������2 */
	u8  PutStept_eight_floor_3;                     /*  ��8���治��ȡ�䵥������3 */
	
	u8  CaptureSetpt1;                              /*  ���յ�1��*/
	u8  MoveSetpt1;                              /*  ���յ�1��*/

	u8	NextSteptStart;															/* ��һ������ĵ�һ���������� */
	
	u8	GrabDir;																		/* ץȡ���� ��0xff    �ң�0x00 */
	
	u8	steptBigForkReturnHomeDir;									/* ��������ԭ���� */
	u8	steptSmallForkReturnHomeDir;								/* ����С���ԭ���� */
	u8	steptRotateReturnHomeDir;										/* ������ת��ԭ���� */
	u8	steptBigForkReturnChangeDir;								/* ����ԭ����ı��־	*/
	u8	steptSmallForkReturnChangeDir;							/* С���ԭ����ı��־	*/	
	u8	steptRotatekReturnChangeDir;								/* ��ת��ԭ����ı��־	*/		
	
	u8	SteptBigFork_LastDir;												/* ������������һ�η��� */
	u8	SteptSmallFork_LastDir;											/* �������С����һ�η��� */
	u8	SteptRotateFork_LastDir;										/* ���������ת��һ�η��� */
	
	u8	AllRetunHome;																/* ȫ���ԭ��־ */
	
	u8  motorerror; 
	
	
	
};
extern struct Robot_Move_State	RobotState;



struct FromFlashConfig	
{
	float BigFork_ES_V;																							/* ��������ٶ� */
	float BigFork_ES_AV;																							/* ����������ٶ� */
	float BigFork_ES_DV;																							/* ����������ٶ� */
		
	float SmallFork_ES_V;																						/* С�������ٶ� */
	float SmallFork_ES_AV;																						/* С���������ٶ� */
	float SmallFork_ES_DV;																						/* С���������ٶ� */	
	
	float Rotate_V;																									/* ��ת�ٶ� */
	float Rotate_AV;																									/* ��ת���ٶ� */
	float Rotate_DV;																									/* ��ת���ٶ� */	
	
	float	BigFork_ES_Position;																			/* �������λ�� */
	float	SmallFork_ES_Position;																		/* С������λ�� */
	float	Rotate_Position;																					/* ��תλ�� */
	  
	int16_t	BigFork_FirstBin[8];																						/* ��������һ��λ�� */
	int16_t	SmallFork_FirstBin[8];																					/* ����С���һ��λ�� */	
	int16_t	BackPack_FirstBin[6];																					/* С�汳¨��һ��λ�� */
	int16_t	BackPack_FirstBin_real[6];																		/* С�汳¨ʵ����Ҫλ�� */	
	int16_t Rotate_Angle_Home;
	int16_t Rotate_Angle_Left;
	int16_t Rotate_Angle_Right;
	
};




extern struct FromFlashConfig FlashConfig;




extern struct SteptControl_Info steptBigFork_Info,steptSmallFork_Info,steptRotate_Info;

extern struct SteptControl_Info steptBigFork_Config[3],steptSmallFork_Config[2],steptRotate_Config[2];
	

extern u8 Lobot2;
extern u8 dir_lobnot;
extern struct LiftControl_Info BigFork_Control,SmallFork_Control;

extern u8 workStart_flag;
extern u8 backpack_servo_status;

void Contact_Init(void);
void ScanLiftControlSoftTIm(void);
u8 LiftMove(struct LiftControl_Info para);
u8 HomeMove_Lift(u8 motor);
void ScanSteptStartUp(void);
u8 ScanReturnHome(void);

u8 ScanFinishFlag(void);
u8 AllReturnHome(void);

#endif											/* __CONTROL_H */

