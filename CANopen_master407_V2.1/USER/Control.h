#ifndef __CONTROL_H
#define __CONTROL_H
#include "main.h"




/* 上位机控制指令 */
struct FromAGVCommand
{
	u8  TakePut;									/* 取放 */
	u8	Dir;											/* 方向 */
	u8	Bin_Floor;								/* 料箱层数 */
	u8	BackPackFloor;						/* 背篓层数 */
	u8  Capture;                  /* 拍照*/
};
extern struct FromAGVCommand LiftAllAction;


struct FromAGVReceive
{
	u8  TakePut_receive;									/* 取放 */
	u8	Dir_receive;											/* 方向 */
	u8	Bin_Floor_receive;								/* 料箱层数 */
	u8	BackPack_Floor_receive;						/* 背篓层数 */
	u8  Capture_recerive;                 /* 拍照 */
};
extern struct FromAGVReceive AGVReceive;



/* canopen控制指令结构体 */
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
	u8	startUp;					/* 启动标志位 */
};



struct Robot_Finish_Flag
{
	u8	ReturnHome_State_finish[5];									 	  /* 找到原点 */
	u8	AllReturnHome_Finish;														/* 所有回原完成 */
	u8	BigFork_Extend_left_finish;											/* 大叉左伸完成 */	
	u8	BigFork_Shrink_left_finish;											/* 大叉左收 */		
	u8	BigFork_Extend_right_finish;										/* 大叉右伸 */		
	u8	BigFork_Shrink_right_finish;										/* 大叉右收 */			
	u8	SmallFork_Extend_finish;												/* 小叉伸展 */	
	u8	SmallFork_Shrink_finish;												/* 小叉收缩 */	
	u8	Rotate_out_left_finish;													/* 旋转左出 */	
	u8	Rotate_back_left_finish;											  /* 旋转左回 */		
	u8	Rotate_out_right_finish;											  /* 旋转右出 */	
	u8	Rotate_back_right_finish;										    /* 旋转右回 */
	u8  Rotate_out_behind_left_finish;                  /* 旋转向后 */
	u8  Rotate_back_behind_left_finish;                 /* 旋转向后回 */
	u8  Rotate_out_behind_right_finish;                  /* 旋转向后 */
	u8  Rotate_back_behind_right_finish;                 /* 旋转向后回 */
	
	u8	BigForkSGear_LeftOut12_finish;							/* 大叉左出舵机完成 */
	u8	BigForkSGear_rightOut34_finish;							/* 大叉右出舵机完成 */	
	u8	SmallForkSGear_Out56_finish;										/* 小叉出舵机完成 */	
	u8  BackPackGear_Out_finish;                    /* 背篓舵机出完成 */
	
	u8	BigForkSGear_LeftBack12_finish;							/* 大叉左舵机完成 */
	u8	BigForkSGear_rightBack34_finish;							/* 大叉右舵机完成 */	
	u8	SmallForkSGear_Back56_finish;										/* 小叉舵机完成 */	
	u8  BackPackGear_Back_finish;                      /* 背篓舵机回完成 */
	
	u8	LiftBigFork_PositionArrive;						      /* 大叉位置到达  9:上极限  10：下极限 */
	u8	LiftSmallFork_PositionArrive;					    	/* 小叉位置到达 9：上极限 ，10：下极限 */		
};
extern struct Robot_Finish_Flag RobotFinishFlag;





struct Robot_Move_State
{
	u8	ReturnHome_State[5];												/* 回原:0、大叉伸缩，1、小叉伸缩，2、小叉旋转，3、大叉升降，4、小叉升降     0xff:回原状态，0：非回原状态*/
	u8	ControlState;																/* 0xff:运行状态，0：非运行状态 */	
	u8	BigFork_Extend_left;												/* 大叉左伸 */
	u8	BigFork_Shrink_left;												/* 大叉左收 */
	u8	BigFork_Extend_right;												/* 大叉右伸 */
	u8	BigFork_Shrink_right;												/* 大叉右收 */	
	u8	SmallFork_Extend;														/* 小叉伸展 */
	u8	SmallFork_Shrink;														/* 小叉收缩 */
	u8	Rotate_out_left;														/* 旋转左出 */
	u8	Rotate_back_left;														/* 旋转左回 */	
	u8	Rotate_out_right;														/* 旋转右出 */
	u8	Rotate_back_right;													/* 旋转右回 */
	u8	Rotate_behind_out_left;											/* 旋转向后左 */
	u8	Rotate_behind_back_left;										/* 旋转向后回左*/
	u8	Rotate_behind_out_right;										/* 旋转向后右 */
	u8	Rotate_behind_back_right;										/* 旋转向后回右*/		
	u8	BigForkBlindFindHome;												/* 大叉盲找原点 */
	u8	RotateBlindFindHome;												/* 旋转盲找原点 */
	
	u8	LiftBigFork_Move_State;											/* 大叉升降运动 */
	u8	LiftSmallFork_move_State;										/* 小叉升降运动 */
	
	u8	TakeUpXFloorBin_State[8];										/* 抓取第几层料箱 */
	u8	PutDownXFloorBin_State[8];									/* 卸箱到第几层 */
	u8	TakeUPXFloorBackPack_State[4];							/* 抓取第几层背篓料箱 */
	u8	PutDownXFloorBackPack_State[4];							/* 将料箱放在背篓第几层 */
	
	u8	BigForkSingGear_LeftOut12_State;						/* 大叉左出舵机 */
	u8	BigForkSGear_RightOut34_State;						/* 大叉右出舵机 */	
	u8	SmallForkSGear_Out56_State;								/* 小叉出舵机 */
	u8  BackPackGear_Out_State;                   /* 背篓出舵机 */
	
	u8	BigForkSGear_LeftBack12_State;							/* 大叉左回舵机 */
	u8	BigForkSGear_RightBack34_State;						/* 大叉右回舵机 */	
	u8	SmallForkSGear_Back56_State;								/* 小叉回舵机 */
	u8  BackPackGear_Back_State;                   /* 背篓回舵机 */
	
	u8	TakeStept1;																	/* 取箱第1步 */	
	u8	TakeStept2;																	/* 取箱第2步 */		
	u8	TakeStept3;																	/* 取箱第3步 */		
	u8	TakeStept4;																	/* 取箱第4步 */		
	u8	TakeStept5;																	/* 取箱第5步 */	
	u8	TakeStept6;																	/* 取箱第6步 */	
	u8	TakeStept7;																	/* 取箱第7步 */	
	u8	TakeStept8;																	/* 取箱第8步 */	
	u8	TakeStept9;																	/* 取箱第9步 */	
	u8  TakeStept10;                                /* 取箱第10步 */
	u8  TakeStept_Simple_1;                         /*  大叉不动取箱单独步骤1 */
	u8  TakeStept_Simple_2;                         /*  大叉不动取箱单独步骤2 */
	u8  TakeStept_Simple_3;                         /*  大叉不动取箱单独步骤3 */
	u8  TakeStept_Simple_4;                         /*  大叉不动取箱单独步骤4*/
	u8  TakeStept_Simple_5;                         /*  大叉不动取箱单独步骤5 */
	
	u8  TakeStept_eight_floor_1;                     /*  第8层大叉不动取箱单独步骤1 */
	u8  TakeStept_eight_floor_2;                     /*  第8层大叉不动取箱单独步骤2 */
	u8  TakeStept_eight_floor_3;                     /*  第8层大叉不动取箱单独步骤2 */
	
	u8	PutStept1;																	/* 卸箱第1步 */	
	u8	PutStept2;																	/* 卸箱第2步 */		
	u8	PutStept3;																	/* 卸箱第3步 */		
	u8	PutStept4;																	/* 卸箱第4步 */		
	u8	PutStept5;																	/* 卸箱第5步 */			
	u8	PutStept6;																	/* 卸箱第6步 */		
	u8	PutStept7;																	/* 卸箱第7步 */				
	u8	PutStept8;																	/* 卸箱第8步 */
	u8  PutStept9;
	u8  PutStept_Simple_1;                         /*  大叉不动放箱单独步骤1 */
	u8  PutStept_Simple_2;                         /*  大叉不动放箱单独步骤2 */
	u8  PutStept_Simple_3;                         /*  大叉不动放箱单独步骤3 */
	u8  PutStept_Simple_4;                         /*  大叉不动放箱单独步骤4 */
	u8  PutStept_Simple_5;                         /*  大叉不动放箱单独步骤5 */
	
	u8  PutStept_eight_floor_1;                     /*  第8层大叉不动取箱单独步骤1 */
	u8  PutStept_eight_floor_2;                     /*  第8层大叉不动取箱单独步骤2 */
	u8  PutStept_eight_floor_3;                     /*  第8层大叉不动取箱单独步骤3 */
	
	u8  CaptureSetpt1;                              /*  拍照第1步*/
	u8  MoveSetpt1;                              /*  拍照第1步*/

	u8	NextSteptStart;															/* 下一个步骤的第一步可以启动 */
	
	u8	GrabDir;																		/* 抓取方向 左：0xff    右：0x00 */
	
	u8	steptBigForkReturnHomeDir;									/* 步进大叉回原方向 */
	u8	steptSmallForkReturnHomeDir;								/* 步进小叉回原方向 */
	u8	steptRotateReturnHomeDir;										/* 步进旋转回原方向 */
	u8	steptBigForkReturnChangeDir;								/* 大叉回原方向改变标志	*/
	u8	steptSmallForkReturnChangeDir;							/* 小叉回原方向改变标志	*/	
	u8	steptRotatekReturnChangeDir;								/* 旋转回原方向改变标志	*/		
	
	u8	SteptBigFork_LastDir;												/* 步进电机大叉上一次方向 */
	u8	SteptSmallFork_LastDir;											/* 步进电机小叉上一次方向 */
	u8	SteptRotateFork_LastDir;										/* 步进电机旋转上一次方向 */
	
	u8	AllRetunHome;																/* 全体回原标志 */
	
	u8  motorerror; 
	
	
	
};
extern struct Robot_Move_State	RobotState;



struct FromFlashConfig	
{
	float BigFork_ES_V;																							/* 大叉伸缩速度 */
	float BigFork_ES_AV;																							/* 大叉伸缩加速度 */
	float BigFork_ES_DV;																							/* 大叉伸缩减速度 */
		
	float SmallFork_ES_V;																						/* 小叉伸缩速度 */
	float SmallFork_ES_AV;																						/* 小叉伸缩加速度 */
	float SmallFork_ES_DV;																						/* 小叉伸缩减速度 */	
	
	float Rotate_V;																									/* 旋转速度 */
	float Rotate_AV;																									/* 旋转加速度 */
	float Rotate_DV;																									/* 旋转减速度 */	
	
	float	BigFork_ES_Position;																			/* 大叉伸缩位置 */
	float	SmallFork_ES_Position;																		/* 小叉伸缩位置 */
	float	Rotate_Position;																					/* 旋转位置 */
	  
	int16_t	BigFork_FirstBin[8];																						/* 升降大叉第一箱位置 */
	int16_t	SmallFork_FirstBin[8];																					/* 升降小叉第一箱位置 */	
	int16_t	BackPack_FirstBin[6];																					/* 小叉背篓第一箱位置 */
	int16_t	BackPack_FirstBin_real[6];																		/* 小叉背篓实际需要位置 */	
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

