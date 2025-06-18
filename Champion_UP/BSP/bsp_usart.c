#include "types.h"
#include "bsp_usart.h"
#include "pid.h"
#include "tim.h"
void Usart_Control(ST_COMMAND *my_command,uint8_t rx[])
{
	float pitch=0;
	float dapao=0;

		system_monitor.rate_cnt.usart1++;
		
		my_command->init=rx[1];
		 
		my_command->start=rx[2];
		
		my_command->bounce=rx[3];
		
		my_command->lay=rx[4];	
		
		my_command->aim=rx[5];
		
		my_command->shot=rx[6];
				
		my_command->flag_aim=rx[7];
		
		my_command->number_aim=rx[8];
		
		memcpy(&pitch,&rx[9],4);
		
		memcpy(&dapao,&rx[13],4);
		
//		memcpy(&yaw,&rx[20],4);
//		
//		memcpy(&distance,&rx[24],4);		
	
		
//		if(my_command->flag_aim==1)
//		{
//			my_command->command_shot.pitch=pitch;
//			my_command->command_shot.dapao=dapao;		
//		}
//		if(my_command->flag_aim==2)
//		
//		{
//			my_command->command_shot.pitch=shot[my_command->number_aim].pitch;
//			my_command->command_shot.dapao=shot[my_command->number_aim].dapao;		
//		}
		
}	


void DC_Montor(uint16_t des,uint16_t fb,ST_PID *pid)//4900-9500
{
	float fpdes=0;
	int count1=0;
	int count2=0;
	if(des>9500)
	{
		fpdes=9500;
	}
	else if(des<4900)
	{
		fpdes=4900;
	}
	else
	{
		fpdes=(float)des;
	}
	pid->fpDes=fpdes;
	pid->fpFB=fb;
	PID_Calc(pid);
	count1=(int)round(pid->fpU)+2000;
	count2=-(int)round(pid->fpU)+2000;
	if(state_pitch==0)
	{
	__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_2,count2);	
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,count1);
	}
	if(state_pitch==1)
	{
	__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_2,2000);	
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,2000);
	}
}

