#include "types.h"
#include "bsp_usart.h"

void Usart_Control(ST_COMMAND *my_command,uint8_t rx[])
{
	float pitch=0;
	float dapao=0;

		system_monitor.rate_cnt.usart1++;
		
		my_command->init=rx[1];
		 
	//	my_command->receive=rx[2];
		
		my_command->bounce=rx[3];
		
		my_command->lay=rx[4];
		
		
		my_command->aim=rx[6];
		
		my_command->shot=rx[7];
		
				
		my_command->flag_aim=rx[10];
		
		my_command->number_aim=rx[11];
		
		memcpy(&pitch,&rx[12],4);
		
		memcpy(&dapao,&rx[16],4);
		
//		memcpy(&yaw,&rx[20],4);
//		
//		memcpy(&distance,&rx[24],4);		
	
	//	my_command->air_flag=rx[2];	
		
		if(my_command->flag_aim==1)
		{
			my_command->command_shot.pitch=pitch;
			my_command->command_shot.dapao=dapao;		
		}
		if(my_command->flag_aim==2)
		
		{
			my_command->command_shot.pitch=shot[my_command->number_aim].pitch;
			my_command->command_shot.dapao=shot[my_command->number_aim].dapao;		
		}
		
		//оч╥Ы
		LimitMax(my_command->command_shot.pitch, 0, 6000);
		LimitMax(my_command->command_shot.dapao, 0, 18000);
}	


void DC_Montor(uint16_t des,uint16_t fb)
{
	if(des>fb)//DOWN
	{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_RESET);
	}
	else if(fb>des)//UP
	{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_RESET);
	}
}

