#include "types.h"
#include "robot_init.h"
#include "usart.h"
#include "bsp_can.h"
#include "can.h"
#include "tim.h"
#include "logic.h"

void robot_init()
{   
	//	HAL_UARTEx_ReceiveToIdle_DMA(&huart1,usart1_rx_buff,100);	
		HAL_UART_Receive_DMA(&huart1,usart1_rx_buff,30); 
    __HAL_UART_ENABLE_IT(&huart1,UART_IT_IDLE);
		HAL_TIM_Base_Start_IT(&htim2);//定时器2中断使能
		can1_start();
		can2_start();
}
