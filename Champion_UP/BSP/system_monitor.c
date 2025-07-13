#include "types.h"
#include "system_monitor.h"
#include <stdlib.h>
#include <math.h>


void System_Monitor(ST_SYSTEM_MONITOR *monitor)
{
	Monitor(monitor->rate_fps.air_receive,&monitor->system_error.air_receive,1000);
	Monitor(monitor->rate_fps.freertos_control,&monitor->system_error.freertos_control,1000);		
	Monitor(monitor->rate_fps.freertos_logic,&monitor->system_error.freertos_logic,1000);		
	Monitor(monitor->rate_fps.freertos_rx,&monitor->system_error.freertos_rx,200);		
	Monitor(monitor->rate_fps.freertos_tx,&monitor->system_error.freertos_tx,1000);
	Monitor(monitor->rate_fps.motor_bounce_left,&monitor->system_error.motor_bounce_left,1000);
	Monitor(monitor->rate_fps.motor_bounce_right,&monitor->system_error.motor_bounce_right,1000);
	Monitor(monitor->rate_fps.motor_pitch,&monitor->system_error.motor_pitch,1000);
	Monitor(monitor->rate_fps.motor_rotate,&monitor->system_error.motor_rotate,1000);
	Monitor(monitor->rate_fps.motor_shot_down,&monitor->system_error.motor_shot_down,1000);
	Monitor(monitor->rate_fps.motor_shot_up,&monitor->system_error.motor_shot_up,1000);	
	Monitor(monitor->rate_fps.usart1,&monitor->system_error.usart1,200);
}	
		
		
void Monitor(uint16_t rps,uint16_t *error,uint16_t decision)
{
	int16_t diff=(int16_t)rps-(int16_t)decision;
	if(abs(diff)<50)
	{
		*error=1;
	}
	else
	{
		*error=0;
	}
}

