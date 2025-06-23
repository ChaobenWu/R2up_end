/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
#include "types.h"
#include "bsp_can.h"
#include "bsp_usart.h"
#include "can.h"
#include "pid.h"
#include "tim.h"
#include "logic.h"
#include "system_monitor.h"
#include "VOFA.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
int last_state=-1;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId logicHandle;
osThreadId controlHandle;
osThreadId monitorHandle;
osThreadId usartRXHandle;
osThreadId usartTXHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void Logic_Task(void const * argument);
void Control_Task(void const * argument);
void Monitor_Task(void const * argument);
void Rx_Task(void const * argument);
void Tx_Task(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of logic */
  osThreadDef(logic, Logic_Task, osPriorityIdle, 0, 128);
  logicHandle = osThreadCreate(osThread(logic), NULL);

  /* definition and creation of control */
  osThreadDef(control, Control_Task, osPriorityIdle, 0, 128);
  controlHandle = osThreadCreate(osThread(control), NULL);

  /* definition and creation of monitor */
  osThreadDef(monitor, Monitor_Task, osPriorityIdle, 0, 128);
  monitorHandle = osThreadCreate(osThread(monitor), NULL);

  /* definition and creation of usartRX */
  osThreadDef(usartRX, Rx_Task, osPriorityIdle, 0, 128);
  usartRXHandle = osThreadCreate(osThread(usartRX), NULL);

  /* definition and creation of usartTX */
  osThreadDef(usartTX, Tx_Task, osPriorityIdle, 0, 128);
  usartTXHandle = osThreadCreate(osThread(usartTX), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    vTaskDelay(pdMS_TO_TICKS(1));
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_Logic_Task */
/**
* @brief Function implementing the logic thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Logic_Task */
void Logic_Task(void const * argument)
{
  /* USER CODE BEGIN Logic_Task */
  /* Infinite loop */
  for(;;)
  {
		Logic(command0);
		system_monitor.rate_cnt.freertos_logic++;
		vTaskDelay(pdMS_TO_TICKS(1));
  }
  /* USER CODE END Logic_Task */
}

/* USER CODE BEGIN Header_Control_Task */
/**
* @brief Function implementing the control thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Control_Task */
void Control_Task(void const * argument)
{
  /* USER CODE BEGIN Control_Task */
  /* Infinite loop */
  for(;;)
  {
		pid_bounce_left.fpDes=desv_bounce_left;
		pid_bounce_left.fpFB=motor_bounce_left.anglev;
		PID_Calc(&pid_bounce_left);
		
		pid_bounce_right.fpDes=desv_bounce_right;
		pid_bounce_right.fpFB=motor_bounce_right.anglev;
		PID_Calc(&pid_bounce_right);

		pid_rotate.outer.fpDes=TD_END(&td_rotate,des_rotate);
		pid_rotate.outer.fpFB=motor_rotate.angle;
		pid_rotate.inner.fpFB=motor_rotate.anglev;
		PID_Calc_DualLoop(&pid_rotate);		
		
		td_shot.aim=des_shot;
		CalTD(&td_shot);	
//		g_stFrictionSMC.fpDes=desv_shot;
//		g_stFrictionSMC.fpFB=motor_shot_up.anglev;
//		CalSMC(&g_stFrictionSMC);
//		CAN_SendCurrent(&hcan1,0x200,g_stFrictionSMC.fpU,g_stFrictionSMC.fpU,0,0);	
		switch(state_shot)
		{
			case 0:	
			if(last_state!=0)
			{
				pid_shot_mod.fpSumE=0;
			}
				pid_shot_mod.fpDes=desv_shot;
				pid_shot_mod.fpFB=motor_shot_up.anglev;
				PID_Calc(&pid_shot_mod);
				CAN_SendCurrent(&hcan1,0x200,pid_shot_mod.fpU,pid_shot_mod.fpU,0,0);		
				last_state=0;
				break;
			case 1:
			if(last_state!=1)
			{
				pid_shot.outer.fpSumE=0;
				pid_shot.inner.fpSumE=0;		
			}
				pid_shot.outer.fpDes=des_shot;
				pid_shot.outer.fpFB=motor_shot_up.angle;
				pid_shot.inner.fpFB=motor_shot_up.anglev;
				PID_Calc_DualLoop(&pid_shot);
				CAN_SendCurrent(&hcan1,0x200,pid_shot.output,pid_shot.output,0,0);		
				last_state=1;
				break;
			case 2:
				if(last_state!=2)
				{
				pid_shot.outer.fpSumE=0;
				pid_shot.inner.fpSumE=0;		
				}					
				pid_shot.outer.fpDes=td_shot.x1;
				pid_shot.outer.fpFB=motor_shot_up.angle;
				pid_shot.inner.fpFB=motor_shot_up.anglev;
				PID_Calc_DualLoop(&pid_shot);
				CAN_SendCurrent(&hcan1,0x200,pid_shot.output,pid_shot.output,0,0);
				last_state=2;
				break;
			default:
				break;
		}
		CAN_SendCurrent(&hcan2,0x200,pid_rotate.output,pid_bounce_right.fpU,pid_bounce_left.fpU,0);
		SendSwitchValue(desq_catch);//气缸
		system_monitor.rate_cnt.freertos_control++;
		vTaskDelay(pdMS_TO_TICKS(1));
  }
  /* USER CODE END Control_Task */
}

/* USER CODE BEGIN Header_Monitor_Task */
/**
* @brief Function implementing the monitor thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Monitor_Task */
void Monitor_Task(void const * argument)
{
  /* USER CODE BEGIN Monitor_Task */
  /* Infinite loop */
  for(;;)
  {
		System_Monitor(&system_monitor);
		vTaskDelay(pdMS_TO_TICKS(1));
  }
  /* USER CODE END Monitor_Task */
}

/* USER CODE BEGIN Header_Rx_Task */
/**
* @brief Function implementing the usartRX thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Rx_Task */
void Rx_Task(void const * argument)
{
  /* USER CODE BEGIN Rx_Task */
  /* Infinite loop */
  for(;;)
  {
		Usart_Control(&command0,usart1_rx_buff);
		system_monitor.rate_cnt.freertos_rx++;
		vTaskDelay(pdMS_TO_TICKS(1));
  }
  /* USER CODE END Rx_Task */
}

/* USER CODE BEGIN Header_Tx_Task */
/**
* @brief Function implementing the usartTX thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Tx_Task */
void Tx_Task(void const * argument)
{
  /* USER CODE BEGIN Tx_Task */
  /* Infinite loop */
  for(;;)
  {
		usart1_tx_buff[0]=0x66;
		usart1_tx_buff[2]=0;
		usart1_tx_buff[3]=0;		
		for (int i = 0; i < 8; i++) {
        usart1_tx_buff[2] |= (*(&system_monitor.system_error.motor_pitch+i) << (7 - i)); 
    }
    for (int i = 8; i < 12; i++) {
        usart1_tx_buff[3] |= (*(&system_monitor.system_error.motor_pitch+i) << (15 - i));  
    }		
		usart1_tx_buff[4]=0;
		usart1_tx_buff[5]=0;
		usart1_tx_buff[6]=0;
		usart1_tx_buff[7]=0;
		usart1_tx_buff[8]=0;
		usart1_tx_buff[9]=0;
		usart1_tx_buff[4]= flag_init;
		usart1_tx_buff[5]=flag_start;
		usart1_tx_buff[6]= flag_bounce;   
		usart1_tx_buff[7]= flag_lay;          
		usart1_tx_buff[8]= flag_aim;    
		usart1_tx_buff[9]= flag_shot;  
		HAL_UART_Transmit(&huart1,usart1_tx_buff,sizeof(usart1_tx_buff),1);
		DC_Montor(des_pitch,fb_pitch,&pid_pitch);
		
		vofa_data.ch_data[0] = des_pitch;
		vofa_data.ch_data[1] = fb_pitch;	
		vofa_data.ch_data[2] = pid_shot.outer.fpDes;
		vofa_data.ch_data[3] = pid_shot.outer.fpFB;	
		vofa_data.ch_data[4] = pid_shot.inner.fpDes;
		vofa_data.ch_data[5] = pid_shot.inner.fpFB;	
		vofa_data.ch_data[6] = pid_shot_mod.fpDes;
		vofa_data.ch_data[7] = pid_shot_mod.fpFB;	
		vofa_data.ch_data[8]= flag_init;
		vofa_data.ch_data[9]=flag_start;
		vofa_data.ch_data[10]= flag_bounce;   
		vofa_data.ch_data[11]= flag_lay;          
		vofa_data.ch_data[12]= flag_aim;    
		vofa_data.ch_data[13]= flag_shot;  
		vofa_data.ch_data[14]= step_init;
		vofa_data.ch_data[15]=step_start;
		vofa_data.ch_data[16]= step_bounce;   
		vofa_data.ch_data[17]= step_lay;          
		vofa_data.ch_data[18]= step_aim;    
		vofa_data.ch_data[19]= step_shot; 
				
	  VOFA_transmit_data(vofa_data.ch_data , 15);
		system_monitor.rate_cnt.freertos_tx++;
		vTaskDelay(pdMS_TO_TICKS(1));
  }
  /* USER CODE END Tx_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
