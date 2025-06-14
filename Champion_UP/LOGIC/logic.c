#include "types.h"
#include "logic.h"
#include "main.h"

int8_t step_init=0;
int8_t step_start=0;
int8_t step_bounce=0;
int8_t step_lay=0;
int8_t step_aim=0;
int8_t step_shot=0;

int time0_1=0;
int time0_2=0;

int time1_1=0;
int time1_2=0;
int time1_3=0;

int time2_1=0;
int time2_2=0;
int time2_3=0;
int time2_4=0;
int time2_5=0;

int time3_1=0;
int time3_2=0;
int time3_3=0;
int time3_4=0;

int time4_1=0;
int time4_2=0;
int time4_3=0;
int time4_4=0;

int time5_1=0;
int time5_2=0;
int time5_3=0;
int time5_4=0;

int time6_0=0;
int time6_1=0;
int time6_2=0;
int time6_3=0;


uint8_t flag_bounce=0;
uint8_t flag_lay=0;
uint8_t flag_aim=0;
uint8_t flag_shot=0;
uint8_t flag_init=0;
uint8_t flag_start=0;


void Logic(const ST_COMMAND my_command)
{
		//初始化任务
			if(my_command.init)
			{
				flag_init=1;
				step_init=0;
			}
			if(flag_init)
			{
				Init_Task();
			}
		//启动任务
			if(my_command.start)
			{
				flag_start=1;
				step_start=0;
			}
			if(flag_start)
			{
				Start_Task();
			}

		//运球任务
			if(my_command.bounce)
			{
				flag_bounce=1;
				step_bounce=0;
			}
			if(flag_bounce)
			{
				Bounce_Task();
			}
		//放球任务
			if(my_command.lay)
			{
				flag_lay=1;
				step_lay=0;
			}
			if(flag_lay)
			{
				Lay_Task();
			}

		//瞄准任务
			if(my_command.aim)
			{
				flag_aim=1;
				step_aim=0;
			}
			if(flag_aim)
			{
				Aim_Task(my_command);
			}
		//发射任务
			if(my_command.shot)
			{
				flag_shot=1;
				step_shot=0;
			}
			if(flag_shot)
			{
				Belt_Shot(my_command);
			}


 
			
}


void Init_Task(void)//初始化
{

	switch (step_init)
	{
		case 0://
			desq_catch=CATCH_CYLINDER_CATCH;
			des_shot=SHOT_INIT;
			des_pitch=PITCH_INIT;
			if(fabs(fb_shot-SHOT_INIT)<5&&abs(fb_pitch-PITCH_INIT)<5)
			{
				step_init=1;
			}		
			break;
		case 1:
			des_rotate=ROTATE_INIT;
			if(fabs(fb_rotate-ROTATE_INIT)<5)
			{
				step_init=2;
			}
			break;
		case 2:
			flag_init=0;
			break;
	}
}



void Start_Task(void)//初始化
{

	switch (step_start)
	{
		case 0://
			des_rotate=ROTATE_BOUNCE;
			desq_catch=CATCH_CYLINDER_CATCH;
			if(fabs(fb_rotate-ROTATE_BOUNCE)<5)
			{
				step_start=1;
			}	
			break;
		case 1:
			des_shot=SHOT_LAY;
			des_pitch=PITCH_LAY;
			if(fabs(fb_shot-SHOT_LAY)<5&&abs(fb_pitch-PITCH_LAY)<5)
			{
				step_start=2;
			}	
			break;
		case 2:
			flag_start=0;
			break;
	}
}


float lefta=-3200;
float righta=3200;
float leftb=3000;
float rightb=-3000;
float t1=0;//松手延时
float t2=10;//反转延时
float t3=80;//停转延时
float t4=100;//夹球延时
float t5=70;//夹球延时
float limit_a=100;//拍球
float maxA=180;

void Bounce_Task(void)
{
		switch (step_bounce)
		{
			case 0://确保拍球位置和发射位置
				state=2;
				des_rotate=ROTATE_BOUNCE;
				desq_catch=CATCH_CYLINDER_CATCH;
				des_shot=SHOT_LAY;
				des_pitch=PITCH_LAY;
				if(fabs(fb_rotate-ROTATE_BOUNCE)<5)
				{
					step_bounce=1;
				}
				break;
			case 1://往下转到特定速度 给延时
				desv_bounce_left=BOUNCE_LEFT_BOUNCE;
				desv_bounce_right=BOUNCE_RIGHT_BOUNCE;
				if(fabs(fbv_bounce_left-BOUNCE_LEFT_BOUNCE)<limit_a&&fabs(fbv_bounce_right-BOUNCE_RIGHT_BOUNCE)<limit_a)
				{
					time2_1++;
				}	
				if(time2_1>t1)
				{
					step_bounce=2;
					time2_1=0;
				}
				break;
			case 2://松手
				desq_catch = CATCH_CYLINDER_UNCATCH;
				if(fabs(fbv_bounce_left-BOUNCE_LEFT_BOUNCE)>maxA&&fabs(fbv_bounce_right-BOUNCE_RIGHT_BOUNCE)>maxA)
				{
				step_bounce=3;			
				}	
				break;
			case 3://开始反转
				time2_2++;
				if(time2_2>t2)
				{
					desv_bounce_left=BOUNCE_LEFT_BOUNCE_U;
					desv_bounce_right=BOUNCE_RIGHT_BOUNCE_U;
					time2_2=0;
					step_bounce=4;
				}
				break;
			case 4://检测向上碰撞
				if(command.air_flag==0)
				{
					step_bounce=5;	
				}
				break;		
			case 5://检测向上碰撞
				if(command.air_flag==1)
				{
					step_bounce=6;	
				}
				break;
			case 6://3号时间到了进行
				desq_catch=1;
				time2_3++;
			if(time2_3>t3)
			{
				desv_bounce_left=0;
				desv_bounce_right=0;
				time2_3=0;
				step_bounce=7;
			}
				break;
			case 7://结束！
				flag_bounce=0;
				break;
		}
}

void Lay_Task(void)
{
	switch(step_lay)
	{
		case 0://把球推进去，然后把云台抬起来
			state=2;
			desq_catch=CATCH_CYLINDER_CATCH;
			des_shot=SHOT_LAY;
			des_pitch=PITCH_LAY;
			if(fabs(fb_shot-SHOT_LAY)<5&&
			abs(fb_pitch-PITCH_LAY)<5)
			{
				step_lay=1;
			}
			break;
		case 1:
			des_rotate=ROTATE_LAY;
			if(fabs(fb_rotate-ROTATE_LAY)<5)
			{
				step_lay=2;
			}
			break;
		case 2://电机开始转
			desv_bounce_left=BOUNCE_LEFT_LAY;
			desv_bounce_right=BOUNCE_RIGHT_LAY;
			if(fabs(fbv_bounce_left-BOUNCE_LEFT_LAY)<10&&fabs(fbv_bounce_right-BOUNCE_RIGHT_LAY)<10)
			{
				step_lay=3;
			}	
			break;
		case 3://开始释放
			time3_1++;
			if(time3_1>20)
			{
				desq_catch = CATCH_CYLINDER_UNCATCH;
				step_lay=4;
				time3_1=0;
			}
			break;
		case 4://检测是否碰到然后停转
			if(fabs(fbv_bounce_left-BOUNCE_LEFT_BOUNCE)>50&&fabs(fbv_bounce_right-BOUNCE_RIGHT_BOUNCE)>50)
			{
				time3_2++;
			}
			if(time3_2>170)
			{
				desv_bounce_left=BOUNCE_LEFT_INIT;
				desv_bounce_right=BOUNCE_RIGHT_INIT;
				time3_2=0;
				step_lay=5;
			}
			break;
		case 5://固定住球
			time3_3++;
			if(time3_3>70)
			{
				time3_3=0;
				desq_catch=CATCH_CYLINDER_CATCH;
				des_shot=SHOT_START;
			}
			if(fabs(fb_shot-SHOT_START)<800)
			{
				step_lay=6;
			}
			break;
		case 6://小延时用来让球落下来
			des_rotate=ROTATE_BOUNCE;
			if(fabs(fb_rotate-ROTATE_BOUNCE)<5)
			{
				flag_lay=0;
				state=0;
			}	
			break;
	}		
}
	

	
void 	Aim_Task(const ST_COMMAND my_command)
{
	switch(step_aim)
	{
		case 0:
			des_pitch=my_command.command_shot.pitch;
			des_rotate=ROTATE_BOUNCE;
			if(fabs(fb_pitch-my_command.command_shot.pitch)<1&&fabs(fb_rotate-ROTATE_BOUNCE)<1)
			{
				step_aim=1;
			}
			break;
		case 1:
			flag_aim=0;
			break;
	}
}




int t_up=15;
int t_uniform=100;
int t_down=80;
void Belt_Shot(const ST_COMMAND my_command)
{
	switch(step_shot)
	{
	case 0:  
			state=0;
			time6_0++;
		if(time6_0>t_up)
		{
			desv_shot=0;
			step_shot=1;
			time6_0=0;
		}
		break;
	case 1:
		desv_shot=-my_command.command_shot.dapao;
		if(fabs(fb_shot-SHOT_SHOT)<20)
		{
			step_shot=2;
		}
		break;
	case 2:
		time6_1++;
		desv_shot=100;
		if(time6_1>t_uniform)
		{
			step_shot=3;
			time6_1=0;
		}
		break;
	case 3:
		desv_shot=0;
		time6_2++;	
		if(time6_2>t_down)
		{
			step_shot=4;
			time6_2=0;
		}
		break;
	case 4:
		state=1;
		des_shot=SHOT_INIT;
		if(fabs(fb_shot-SHOT_INIT)<1)
		{
			step_shot=5;
			desv_shot=0;
		}
		break;
	case 5:
		flag_shot=0;
		break;
	}
}
