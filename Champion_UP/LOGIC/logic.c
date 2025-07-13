#include "types.h"
#include "logic.h"
#include <stdlib.h>
#include <math.h>
float Shot_Shot=-2000;
uint8_t step_init=0;
uint8_t step_start=0;
uint8_t step_bounce=0;
uint8_t step_lay=0;
uint8_t step_aim=0;
uint8_t step_shot=0;
uint8_t step_out=0;
uint8_t step_danger=0;
uint8_t step_protect=0;

int number_protect=0;

float fb_max=0;

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

int time7_0=0;
int time7_1=0;
int time7_2=0;
int time7_3=0;


int time8_1=0;

uint8_t flag_bounce=0;
uint8_t flag_lay=0;
uint8_t flag_aim=0;
uint8_t flag_shot=0;
uint8_t flag_init=0;
uint8_t flag_start=0;
uint8_t flag_out=0;
uint8_t flag_danger=0;
uint8_t flag_protect=0;

volatile uint8_t task_running = 0;  // 0: 无任务, 1: 有任务

// 检查并设置任务状态
uint8_t start_task(uint8_t *flag, uint8_t *step) {
    // 如果没有任务运行，启动新任务
    if (!task_running) {
        task_running = 1;  // 设置任务运行标志
        *flag = 1;         // 设置任务标志
        *step = 0;         // 重置任务步骤
        return 1;          // 成功启动任务
    }
    return 0;  // 无法启动任务
}

// 任务完成时调用
void end_task(void) {
    task_running = 0;  // 清除任务运行标志
}


void Logic(const ST_COMMAND my_command)
{
//    // 初始化任务
//    if (my_command.init) {
//        if (start_task(&flag_init, &step_init)) {
//            // 任务已启动
//        }
//    }
//    if (flag_init) {
//        Init_Task();
//    }
    
    // 启动任务
    if (my_command.start) {
        if (start_task(&flag_start, &step_start)) {
            // 任务已启动
        }
    }
    if (flag_start) {
        Start_Task();
    }

    // 运球任务
    if (my_command.bounce) {
        if (start_task(&flag_bounce, &step_bounce)) {
            // 任务已启动
        }
    }
    if (flag_bounce) {
        Bounce_Task(my_command);
    }
    
    // 放球任务
    if (my_command.lay) {
        if (start_task(&flag_lay, &step_lay)) {
            // 任务已启动
        }
    }
    if (flag_lay) {
        Lay_Task();
    }

    // 瞄准任务
    if (my_command.aim) {
        if (start_task(&flag_aim, &step_aim)) {
            // 任务已启动
        }
    }
    if (flag_aim) {
        Aim_Task(my_command);
    }
    
    // 发射任务
    if (my_command.shot) {
        if (start_task(&flag_shot, &step_shot)) {
            // 任务已启动
        }
    }
    if (flag_shot) {
        Belt_Shot(my_command);
    }
    
    // 吸球任务
    if (my_command.out) {
			flag_out=1;
			step_out=0;
    }
    if (flag_out) {
        Bounce_Out(my_command);
    }


    // 保护任务
    if (my_command.protect) {
			flag_protect=1;
			step_protect=0;
    }
    if (flag_protect) {
        Protect();
    }
    
    // 危险任务
    if (my_command.danger) {
        if (start_task(&flag_danger, &step_danger)) {
            // 任务已启动
        }
    }
    if (flag_danger) {
        Danger();
    }
}


//void Init_Task(void)//初始化
//{

//	switch (step_init)
//	{
//		case 0://
//			state_pitch=0;
//			state_shot=2;
//			desq_catch=CATCH_CYLINDER_CATCH;
//			des_shot=SHOT_INIT;
//			des_pitch=PITCH_INIT;
//			if(fabs(fb_shot-SHOT_INIT)<5&&abs(fb_pitch-PITCH_INIT)<5)
//			{
//				step_init=1;
//			}		
//			break;
//		case 1:
//			des_rotate=ROTATE_INIT;
//			if(fabs(fb_rotate-ROTATE_INIT)<5)
//			{
//				step_init=2;
//			}
//			break;
//		case 2:
//			flag_init=0;
//			end_task();
//			break;
//	}
//}

void Danger(void)//收起来
{
	switch(step_danger)
	{
		case 0://把球推进去，然后把云台抬起来
			state_pitch=0;
			state_shot=2;
			desq_catch=CATCH_CYLINDER_CATCH;
			des_shot=SHOT_LAY;
			des_pitch=PITCH_LAY;
			if(fabs(fb_shot-SHOT_LAY)<5&&
			abs(fb_pitch-PITCH_LAY)<10)
			{
				step_danger=1;
			}
			break;
		case 1:
			des_rotate=ROTATE_LAY;
			if(fabs(fb_rotate-ROTATE_LAY)<5)
			{
				step_danger=2;
			}
			break;
		case 2:
			flag_danger=0;
			end_task();
			break;
	}	
}

void Start_Task(void)//展开
{

	switch (step_start)
	{

		case 0:
			state_shot=2;
			des_shot=SHOT_START;
			des_pitch=PITCH_START;
			if(fabs(fb_shot-SHOT_START)<5&&abs(fb_pitch-PITCH_START)<5)
			{
				step_start=1;
			}	
			break;
		case 1://
			state_pitch=0;
			des_rotate=ROTATE_BOUNCE;
			desq_catch=CATCH_CYLINDER_CATCH;
			if(fabs(fb_rotate-ROTATE_BOUNCE)<5)
			{
				step_start=2;
			}	
			break;
		case 2:
			flag_start=0;
			end_task();
			break;
	}
}


float lefta=-2600;
float righta=2600;
float leftb=3600;
float rightb=-3600;
float t1=0;//松手延时
float t2=10;//反转延时
float t3=40;//停转延时
float t4=150;//夹球延时
float t5=70;//夹球延时
float limit_a=100;//拍球
float maxA=180;
float current_a=8000;
float current_b=8000;
void Bounce_Task(const ST_COMMAND my_command)
{
		switch (step_bounce)
		{
			case 0://确保拍球位置和发射位置
				state_shot=2;
				state_pitch=0;
				des_rotate=ROTATE_BOUNCE;
				desq_catch=CATCH_CYLINDER_CATCH;
				des_shot=SHOT_LAY;
				des_pitch=PITCH_LAY;
				if(fabs(fb_rotate-ROTATE_BOUNCE)<5)
				{
					step_bounce=1;
					pid_bounce_left.fpOMax=current_a;
					pid_bounce_right.fpOMax=current_b;
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
				pid_bounce_left.fpOMax=14000;
				pid_bounce_right.fpOMax=14000;
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
				if(my_command.air_flag==0)
				{
					step_bounce=5;	
				}
				break;		
			case 5://检测向上碰撞
				if(my_command.air_flag==1)
				{
					step_bounce=6;	
				}
				break;
			case 6://3号时间到了进行
				time2_3++;
				if(time2_3>t3)
				{
				desq_catch=1;
				time2_3=0;
				}
				time2_4++;
			if(time2_4>t4)
			{
				desv_bounce_left=0;
				desv_bounce_right=0;
				time2_4=0;
				step_bounce=7;
			}
				break;
			case 7://结束！
				flag_bounce=0;
				end_task();
				break;
		}
}

//void Bounce_Task(const ST_COMMAND my_command)
//{
//		switch (step_bounce)
//		{
//			case 0://确保拍球位置和发射位置
//				state_shot=2;
//				state_pitch=0;
//				des_rotate=ROTATE_BOUNCE;
//				desq_catch=CATCH_CYLINDER_CATCH;
//				des_shot=SHOT_LAY;
//				des_pitch=PITCH_LAY;
//				if(fabs(fb_rotate-ROTATE_BOUNCE)<5)
//				{
//					step_bounce=1;
//					pid_bounce_left.fpOMax=12000;
//					pid_bounce_right.fpOMax=12000;
//				}
//				break;
//			case 1://往下转到特定速度 给延时
//				desv_bounce_left=BOUNCE_LEFT_BOUNCE;
//				desv_bounce_right=BOUNCE_RIGHT_BOUNCE;
//				if(fabs(fbv_bounce_left-BOUNCE_LEFT_BOUNCE)<limit_a&&fabs(fbv_bounce_right-BOUNCE_RIGHT_BOUNCE)<limit_a)
//				{
//					time2_1++;
//				}	
//				if(time2_1>t1)
//				{
//					step_bounce=2;
//					time2_1=0;
//				}
//				break;
//			case 2://松手
//				desq_catch = CATCH_CYLINDER_UNCATCH;
//				if(fabs(fbv_bounce_left-BOUNCE_LEFT_BOUNCE)>maxA&&fabs(fbv_bounce_right-BOUNCE_RIGHT_BOUNCE)>maxA)
//				{
//				step_bounce=3;
//					pid_bounce_left.fpOMax=16000;
//					pid_bounce_right.fpOMax=16000;			
//				}	
//				break;
//			case 3://开始反转
//				time2_2++;
//				if(time2_2>t2)
//				{
//					desv_bounce_left=BOUNCE_LEFT_BOUNCE_U;
//					desv_bounce_right=BOUNCE_RIGHT_BOUNCE_U;
//					time2_2=0;
//					step_bounce=4;
//				}
//				break;
//			case 4://检测向上碰撞
//				if(my_command.air_flag==0)
//				{
//					step_bounce=5;	
//				}
//				break;		
//			case 5://检测向上碰撞
//				if(my_command.air_flag==1)
//				{
//					step_bounce=6;	
//				}
//				break;
//			case 6://3号时间到了进行
//				desq_catch=1;
//				time2_3++;
//			if(time2_3>t3)
//			{
//				desv_bounce_left=0;
//				desv_bounce_right=0;
//				time2_3=0;
//				step_bounce=7;
//			}
//				break;
//			case 7://结束！
//				flag_bounce=0;
//				break;
//		}
//}


void Lay_Task(void)
{
	switch(step_lay)
	{
		case 0://把球推进去，然后把云台抬起来
			state_pitch=0;
			state_shot=2;
			desq_catch=CATCH_CYLINDER_CATCH;
			des_shot=SHOT_LAY;
			des_pitch=PITCH_LAY;
			if(fabs(fb_shot-SHOT_LAY)<5&&
			abs(fb_pitch-PITCH_LAY)<10)
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
			if(time3_2>210)
			{
				desv_bounce_left=BOUNCE_LEFT_INIT;
				desv_bounce_right=BOUNCE_RIGHT_INIT;
				time3_2=0;
				step_lay=5;
			}
			break;
		case 5://固定住球
			time3_3++;
			if(time3_3>300)
			{
				time3_3=0;
				desq_catch=CATCH_CYLINDER_CATCH;
				step_lay=6;
			}
			break;
		case 6://小延时用来让球落下来
				flag_lay=0;
				end_task();
			break;
	}		
}



	
void 	Aim_Task(const ST_COMMAND my_command)
{
	switch(step_aim)
	{
		case 0:
			state_shot=2;
			state_pitch=0;
			des_shot=SHOT_START;
			des_pitch=my_command.command_shot.pitch;
			des_rotate=ROTATE_BOUNCE;
			
			if(fabs(fb_pitch-my_command.command_shot.pitch)<10&&fabs(fb_rotate-ROTATE_BOUNCE)<1&&fabs(fb_shot-SHOT_START)<5)
			{
				step_aim=1;
			}
			break;
		case 1:
			flag_aim=0;
			end_task();
			break;
	}
}




int t_up=100;
int t_uniform=100;
int t_down=500;
float un_shotv=100;

void Belt_Shot(const ST_COMMAND my_command)
{
	switch(step_shot)
	{
	case 0:
		state_shot=2;
		state_pitch=0;//0
		fb_max=0;
		des_shot=SHOT_START;
		des_pitch=my_command.command_shot.pitch;
		des_rotate=ROTATE_BOUNCE;		
		if(fabs(fb_pitch-my_command.command_shot.pitch)<100&&fabs(fb_rotate-ROTATE_BOUNCE)<1&&fabs(fb_shot-SHOT_START)<5)
			{
				step_shot=1;
			}	
		break;
	case 1:  
		state_shot=0;
		state_pitch=1;
		time6_0++;
		if(time6_0>t_up)
		{
			desv_shot=0;
			step_shot=2;
			time6_0=0;
		}
		break;
	case 2:
		desv_shot=-my_command.command_shot.dapao;
		
		if(Shot_Shot-fb_shot>0)
		{
			step_shot=3;
		}
		break;
	case 3:
		time6_1++;
		state_shot=3;
		desv_shot=un_shotv;
		if(fb_shot>fb_max)
		{
			fb_max=fb_shot;
		}
		if(time6_1>t_uniform)
		{
			step_shot=4;
			time6_1=0;
		}
		break;
	case 4:
		desv_shot=0;
		time6_2++;	
		if(time6_2>t_down)
		{
			step_shot=5;
			time6_2=0;
		}
		break;
	case 5:
		state_shot=2;
		des_shot=SHOT_START;
		if(fabs(fb_shot-SHOT_START)<1)
		{
			step_shot=6;
			desv_shot=0;
		}
		break;
	case 6:
		state_pitch=0;
		flag_shot=0;
		end_task();
		break;
	}
}
//-2931


void Bounce_Out(const ST_COMMAND my_command)
{
	switch(step_out)
	{
		case 0:
			desq_catch=0;
			desv_bounce_left=BOUNCE_LEFT_BOUNCE_U;
			desv_bounce_right=BOUNCE_RIGHT_BOUNCE_U;
			step_out=1;
			break;	
		case 1://检测向上碰撞
				if(my_command.air_flag==1)
				{
					step_out=2;	
				}
				break;
		case 2://3号时间到了进行
				time7_1++;
				if(time7_1>t3)
				{
				desq_catch=1;
				time7_1=0;
				}
				time7_2++;
			if(time7_2>t4)
			{
				desv_bounce_left=0;
				desv_bounce_right=0;
				time7_2=0;
				step_out=3;
			}
				break;
			case 3://结束！
				flag_out=0;
				break;
		}
		
}	



void Protect(void)
{
	switch(step_protect)
	{
	case 0:

		desq_danger=1-desq_danger;
		step_protect=1;

	break;
	case 1:
	time8_1++;
	if(time8_1>200)
	{
		step_protect=2;
	}
	break;
	case 2:
	flag_protect=0;
	break;
	}
}