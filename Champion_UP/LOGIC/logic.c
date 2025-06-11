//ע�⣬��������Ƕ����ġ�
#include "types.h"
#include "logic.h"
int aim_choice=0;
int shot_choice=0;
int8_t step_init=0;
int8_t step_receive=0;
int8_t step_bounce=0;
int8_t step_lay=0;
int8_t step_aim=0;
int8_t step_shot=0;
int8_t step_home=0;
int8_t step_force=0;
int8_t step_stretch=0;
int8_t step_shot_extra=0;//˿����



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


int time6_1=0;
int time6_2=0;

int time7_1=0;
int time7_2=0;

uint8_t flag_bounce=0;
uint8_t flag_lay=0;
uint8_t flag_aim=0;
uint8_t flag_shot=0;
uint8_t flag_init=0;
uint8_t flag_home=0;
uint8_t flag_force=0;
uint8_t flag_stretch=0;
void Logic(const ST_COMMAND my_command)
{
		//��ʼ������
			if(my_command.init)
			{
				flag_init=1;
				step_init=0;
			}
			if(flag_init)
			{
				Init_Task();
			}

		//��������
			if(my_command.bounce)
			{
				flag_bounce=1;
				step_bounce=0;
			}
			if(flag_bounce)
			{
				Bounce_Inited_Task();
			}
		//��������
			if(my_command.lay)
			{
				flag_lay=1;
				step_lay=0;
			}
			if(flag_lay)
			{
				Lay_Task();
			}
			if(my_command.force)
			{
				flag_force=1;
				step_force=0;
			}
			if(flag_force)
			{
				Force_Task();
			}
		//��׼����
			if(my_command.aim)
			{
				flag_aim=1;
				step_aim=0;
			}
			if(flag_aim)
			{
				Aim_Task(my_command);
			}
		//��������
			if(my_command.shot)
			{
				flag_shot=1;
				step_shot=0;
			}
			if(flag_shot)
			{
				Shot_Task();
			}
		//��������
			if(my_command.stretch)
			{
				flag_stretch=1;
				step_stretch=0;
			}
			if(flag_stretch)
			{
				Stretch_Task();
			}
			//��λ����
			if(my_command.home)
			{
				flag_home=1;
				step_home=0;
			}
			if(flag_home)
			{
				Home_Task();
			}
			usart1_tx_buff[4]=0;
			usart1_tx_buff[5]=0;
			usart1_tx_buff[6]=0;
			usart1_tx_buff[7]=0;
			usart1_tx_buff[8]=0;
			usart1_tx_buff[9]=0;
			usart1_tx_buff[10]=0;   // �� 4 λ	
			usart1_tx_buff[11]=0; 
			usart1_tx_buff[12]=0; 	
			usart1_tx_buff[4]= flag_init; // �� 0 λ
			usart1_tx_buff[6]= flag_bounce;   // �� 1 λ
			usart1_tx_buff[7]= flag_lay;     
			usart1_tx_buff[8]= flag_force;     
			usart1_tx_buff[9]= flag_aim;    
			usart1_tx_buff[10]= flag_shot;   
			usart1_tx_buff[11]= flag_home; 
			usart1_tx_buff[12]= flag_stretch; 			
}


void Init_Task(void)//��ʼ��
{

	switch (step_init)
	{

	}
}



float lefta=-5000;
float righta=5000;
float leftb=3000;
float rightb=-3000;
float t1=0;//������ʱ
float t2=90;//��ת��ʱ
float t3=80;//ͣת��ʱ
float t4=100;//������ʱ
float t5=70;//������ʱ
float aaaaa=100;//����
float aaa=2000;
float maxA=180;
float maxU=500;//Ҫ�Ĵ�1000��500

void Bounce_Inited_Task(void)
{
		switch (step_bounce)
		{
			case 0://ȷ������λ�úͷ���λ��
				des_rotate=ROTATE_BOUNCE;
				desq_catch=CATCH_CYLINDER_CATCH;
				des_shot=SHOT_LAY;
				des_pitch=PITCH_LAY;
				if(fabs(des_rotate-ROTATE_BOUNCE)<5)
				{
					step_bounce=1;
				}
				break;
			case 1://����ת���ض��ٶ� ����ʱ
				desv_bounce_left=BOUNCE_LEFT_BOUNCE;
				desv_bounce_right=BOUNCE_RIGHT_BOUNCE;
				if(fabs(fbv_bounce_left-BOUNCE_LEFT_BOUNCE)<aaaaa&&fabs(fbv_bounce_right-BOUNCE_RIGHT_BOUNCE)<aaaaa)
				{
					time2_1++;
				}	
				if(time2_1>t1)
				{
					step_bounce=2;
					time2_1=0;
				}
				break;
			case 2://����
				desq_catch = CATCH_CYLINDER_UNCATCH;
				if(fabs(fbv_bounce_left-BOUNCE_LEFT_BOUNCE)>maxA&&fabs(fbv_bounce_right-BOUNCE_RIGHT_BOUNCE)>maxA)
				{
				step_bounce=3;			
				}	
				break;
			case 3://��ʼ��ת
				time2_2++;
				if(time2_2>t2)
				{
					desv_bounce_left=BOUNCE_LEFT_BOUNCE_U;
					desv_bounce_right=BOUNCE_RIGHT_BOUNCE_U;
					time2_2=0;
					step_bounce=4;
				}
				break;
			case 4://���������ײ
				if(command.air_flag==1)
				{
					step_bounce=6;	
				}
				break;
			case 6://3��ʱ�䵽�˽���
			time2_3++;
			if(time2_3>t3)
			{
				desq_catch=1;
				desv_bounce_left=0;
				desv_bounce_right=0;
				time2_3=0;
				step_bounce=7;
			}
				break;
			case 7://������
				flag_bounce=0;
				break;
		}
}

void Lay_Task(void)
{
//	switch(step_lay)
//	{
//		case 0://�����ƽ�ȥ��Ȼ�����̨̧����
//			des_accept_back=ACCEPT_BACK_INIT;
//			des_accept_front=ACCEPT_FRONT_INIT;
//			des_drawer_left=DRAWER_LEFT_INIT;
//			des_drawer_right=DRAWER_RIGHT_INIT;
//			desq_catch=CATCH_CYLINDER_CATCH;
//			
//			if(fabs(DRAWER_LEFT_INIT-fb_drawer_left)<1&&
//			fabs(DRAWER_RIGHT_INIT-fb_drawer_right)<1&&
//			fabs(fb_accept_back-ACCEPT_BACK_INIT)<1&&
//			fabs(fb_accept_front-ACCEPT_FRONT_INIT)<1
//			)
//			{
//				step_lay=1;
//			}
//			break;
//		case 1://�����ʼת
//			desv_bounce_left=BOUNCE_LEFT_LAY;
//			desv_bounce_right=BOUNCE_RIGHT_LAY;
//			desv_bounce_middle=BOUNCE_MIDDLE_LAY;
//			if(fabs(fbv_bounce_left-BOUNCE_LEFT_LAY)<10&&fabs(fbv_bounce_right-BOUNCE_RIGHT_LAY)<10&&fabs(fbv_bounce_middle-BOUNCE_MIDDLE_LAY)<10)
//			{
//				step_lay=2;
//			}	
//			break;
//		case 2://��ʼ�ͷ�
//			time3_1++;
//			if(time3_1>500)
//			{
//				desq_catch = CATCH_CYLINDER_UNCATCH;
//				step_lay=3;
//				time3_1=0;
//			}
//			break;
//		case 3://����Ƿ�����Ȼ��ͣת
//			if((fabs(fbv_bounce_left-BOUNCE_LEFT_BOUNCE)>50&&fabs(fbv_bounce_right-BOUNCE_RIGHT_BOUNCE)>50&&fabs(fbv_bounce_middle-BOUNCE_MIDDLE_BOUNCE)>50))
//			{
//				time3_2++;
//			}
//			if(time3_2>500)
//			{
//				desv_bounce_left=BOUNCE_LEFT_INIT;
//				desv_bounce_right=BOUNCE_RIGHT_INIT;
//				desv_bounce_middle=BOUNCE_MIDDLE_INIT;	
//				time3_2=0;
//				step_lay=4;
//			}
//			break;
//		case 4://�̶�ס��
//			time3_3++;
//			if(time3_3>70)
//			{
//				time3_3=0;
//				desd_fix=FIX_SERVO_FIX;
//				desd_release=RELEASE_SERVO_INIT;
//				step_lay=5;
//			}
//			break;
//		case 5://С��ʱ��������������
//			time3_4++;
//			if(time3_4>70)
//			{
//				time3_4=0;
//				flag_lay=0;
//			}	
//			break;
//	}		
}
	

	
void 	Aim_Task(const ST_COMMAND my_command)
{
//	switch(step_aim)
//	{
//		case 0:
//			des_pitch=my_command.command_shot.pitch;
//			des_row=my_command.command_shot.row;
//			des_shot=my_command.command_shot.dapao;
//			des_drawer_left=DRAWER_LEFT_BOUNCE;
//			des_drawer_right=DRAWER_RIGHT_BOUNCE;
//			des_accept_back=ACCEPT_BACK_ACCEPT;
//			des_accept_front=ACCEPT_FRONT_SHOT;
//			if(fabs(fb_pitch-my_command.command_shot.pitch)<1&&fabs(fb_row-my_command.command_shot.row)<1&&fabs(fb_shot-my_command.command_shot.dapao)<1)
//			{
//				step_aim=1;
//			}
//			break;
//		case 1:
//			flag_aim=0;
//			break;
//	}
}

void Shot_Task(void)
{
//	switch(step_shot)
//	{
//		case 0:
//			des_drawer_left=DRAWER_LEFT_BOUNCE;
//			des_drawer_right=DRAWER_RIGHT_BOUNCE;
//			des_accept_back=ACCEPT_BACK_ACCEPT;
//			des_accept_front=ACCEPT_FRONT_SHOT;
//			if(fabs(DRAWER_LEFT_BOUNCE-fb_drawer_left)<1&&fabs(DRAWER_RIGHT_BOUNCE-fb_drawer_right)<1&&fabs(fb_accept_back-ACCEPT_BACK_ACCEPT)<5&&fabs(fb_accept_front-ACCEPT_FRONT_SHOT)<5)
//				{
//					step_shot=1;
//				}
//			break;
//		case 1:
//			desd_release=RELEASE_SERVO_RELEASE;
//			time5_1++;
//			if(time5_1>70)
//			{
//				time5_1=0;
//				flag_shot=0;
//			}
//			break;
//	}
}

void Home_Task(void)
{
//	switch(step_home)
//	{
//		case 0:
//			desd_fix=FIX_SERVO_FIX;
//			desd_release=RELEASE_SERVO_RELEASE;
//			step_home=1;
//			break;
//		case 1://С��ʱ
//			time6_1++;
//			if(time6_1>70)
//			{
//				time6_1=0;
//				step_home=2;
//			}	
//			break;
//		case 2:
//			des_shot=0;
//			if(fabs(fb_shot-SHOT_INIT)<1)
//			{
//				step_home=3;
//			}
//			break;
//		case 3:
//			desd_release=RELEASE_SERVO_INIT;
//			time6_2++;
//			if(time6_2>70)
//			{
//				time6_2=0;
//				flag_home=0;
//			}	
//			break;
//	}		
}	
	
	
	
void Force_Task(void)
	{
//		switch(step_force)
//		{
//			case 0:
//				des_pitch=PITCH_LAY;
//				des_row=ROW_INIT;
//				des_shot=SHOT_LAY;
//				if(fabs(fb_pitch-PITCH_LAY)<1&&
//				fabs(fb_row-ROW_INIT)<1&&
//				fabs(fb_shot-SHOT_LAY)<1)
//					{
//						step_force=1;
//					}
//					break;
//			case 1:
//				flag_force=0;
//		}
}
	

void Stretch_Task(void)
{
//	switch(step_stretch)
//	{
//		case 0://shoujinqu
//			des_accept_back=ACCEPT_BACK_INIT;
//			des_accept_front=ACCEPT_FRONT_INIT;
//			des_drawer_left=DRAWER_LEFT_INIT;
//			des_drawer_right=DRAWER_RIGHT_INIT;
//			desq_catch=CATCH_CYLINDER_CATCH;
//			
//			if(fabs(DRAWER_LEFT_INIT-fb_drawer_left)<1&&
//			fabs(DRAWER_RIGHT_INIT-fb_drawer_right)<1&&
//			fabs(fb_accept_back-ACCEPT_BACK_INIT)<1&&
//			fabs(fb_accept_front-ACCEPT_FRONT_INIT)<1
//			)
//			{
//				step_stretch=1;
//			}
//			break;
//		case 1:
//			flag_stretch=0;
//			break;
//	}
}
