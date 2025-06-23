#ifndef __PID_H__
#define __PID_H__

#include "types.h"
#include "stm32f4xx_hal_can.h"
#include "main.h"
void PID_Calc(ST_PID *pstPid);
void PID_Calc_DualLoop(ST_CASCADE_PID *cascadePid);
void CalTD(ST_TD *pStTD);
float plan_of_time(float start_pos, float end_pos, float time);
float TD_END(ST_TD *pStTD,float end_pos);
void CalSMC(ST_SMC *pStSMC);
float SMC_SatFunc(float in, float d);
int Sign_Judge(FP32 fp_Judge_Number);
float Clip(float in,float min ,float max );



#endif //
