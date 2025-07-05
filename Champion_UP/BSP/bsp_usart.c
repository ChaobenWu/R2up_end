#include "types.h"
#include "bsp_usart.h"
#include "pid.h"
#include "tim.h"
void Usart_Control(ST_COMMAND *my_command,uint8_t rx[])
{
	float pitch=0;
	float dapao=0;
	
if(rx[0]==0x88&&rx[29]==0xFF)
{
    uint16_t received_crc = (rx[28] << 8) | rx[27]; // 小端模式
		
    uint16_t calculated_crc = crc16(&rx[1], 26);    // 计算1-26字节的CRC
		
		if(received_crc == calculated_crc)
		{

		system_monitor.rate_cnt.usart1++;
		
		my_command->init=rx[1];
		 
		my_command->start=rx[2];
		
		my_command->bounce=rx[3];
		
		my_command->lay=rx[4];	
		
		my_command->aim=rx[5];
		
		my_command->shot=rx[6];
		
		my_command->out=rx[25];
		
		my_command->danger=rx[26];
				
		my_command->flag_aim=rx[7];
		
		my_command->number_aim=rx[8];
		
		memcpy(&pitch,&rx[17],4);
		
		memcpy(&dapao,&rx[21],4);
		
		memcpy(&yaw,&rx[13],4);
//		
		memcpy(&distance,&rx[9],4);		
		
	
//		
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
		}
}
}	


void DC_Montor(uint16_t des,uint16_t fb,ST_PID *pid)//4900-9500
{
	float fpdes=0;
	int count1=0;
	int count2=0;
	if(des>8800)
	{
		fpdes=8800;
	}
	else if(des<5200)
	{
		fpdes=5200;
	}
	else
	{
		fpdes=(float)des;
	}
	pid->fpDes=fpdes;
	pid->fpFB=fb;
	PID_Calc_Pitch(pid);
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




// CRC
uint16_t crc16(uint8_t *data, uint16_t length) {
    uint16_t crc = 0xFFFF;  // 初始值
    
    for (uint16_t i = 0; i < length; i++) {
        crc ^= data[i];
        
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 0x0001) {
                crc = (crc >> 1) ^ 0xA001;  // 多项式反转值
            } else {
                crc = crc >> 1;
            }
        }
    }
    return crc;
}