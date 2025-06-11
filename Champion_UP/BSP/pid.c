#include "types.h"
#include "pid.h"
//***********************************************单环PID版本
//PID参数初始化
void PID_Init(ST_PID *pid, float p, float i, float d, float maxErr, float maxI, float maxO)
{
    pid->fpKp = p;
    pid->fpKi = i;
    pid->fpKd = d;
    pid->fpEMax = maxErr;
    pid->fpIMax = maxI;
    pid->fpOMax = maxO;
}


void PID_Calc(ST_PID *pstPid)
{
    // 计算本次偏差
    pstPid->fpE = pstPid->fpDes - pstPid->fpFB;

    // 如果误差在死区范围内，则将误差视为0
    if (fabs(pstPid->fpE) <= pstPid->fpEMin)
    {
        pstPid->fpE = 0.0f;
    }

    // 积分分离：当偏差大于设定的最大偏差值时，不进行积分作用
    if (fabs(pstPid->fpE) < pstPid->fpEMax)
    {
        pstPid->fpSumE += pstPid->fpE; // 累积偏差
    }

    // 积分限幅
    if (pstPid->fpSumE > pstPid->fpIMax)
    {
        pstPid->fpSumE = pstPid->fpIMax;
    }
    else if (pstPid->fpSumE < -pstPid->fpIMax)
    {
        pstPid->fpSumE = -pstPid->fpIMax;
    }
    // 计算比例项
    pstPid->fpUKp = pstPid->fpKp * pstPid->fpE;

    // 计算积分项（注意Ki已经乘在累积偏差上）
    pstPid->fpUKi = pstPid->fpKi * pstPid->fpSumE;
		
    // 微分先行，避免反馈值的噪声对微分项的影响
    float diffE = (pstPid->fpE - pstPid->fpPreE);
    pstPid->fpUKd = pstPid->fpKd * diffE;

    // 更新本次PID运算结果
    pstPid->fpU = pstPid->fpUKp + pstPid->fpUKi + pstPid->fpUKd;

    // 输出限幅
    if (pstPid->fpU > pstPid->fpOMax)
    {
        pstPid->fpU = pstPid->fpOMax;
    }
    else if (pstPid->fpU < -pstPid->fpOMax)
    {
        pstPid->fpU = -pstPid->fpOMax;
    }

    // 更新上次偏差和上次微分项输出
    pstPid->fpPreE = pstPid->fpE;
    pstPid->fpUKdpre = pstPid->fpUKd;
}


void PID_Calc_DualLoop(ST_CASCADE_PID *pid)
{
    // 更新外环PID
    PID_Calc(&pid->outer);

    // 将外环的输出设为内环的目标值
    pid->inner.fpDes = pid->outer.fpU;

    // 更新内环PID
    PID_Calc(&pid->inner);
    
    // 最终输出是内环的PID运算结果
    pid->output = pid->inner.fpU;
}

void CalTD(ST_TD *pStTD)
{
    float d, d0, y, a0, a = 0, fhan;
    pStTD->x = pStTD->x1 - pStTD->aim;
    // 计算当前位置与目标位置的误差

    d = pStTD->r * pStTD->h; // 计算调节因子 d
    d0 = pStTD->h * d;       // 时间相关项 d0
    y = pStTD->x + pStTD->h * pStTD->x2;
    // 组合当前位置误差和速度误差

    a0 = sqrt(d * d + 8 * pStTD->r * fabs(y));
    // 计算带有非线性特性的中间量 a0

    if (fabs(y) > d0)
        a = pStTD->x2 + (a0 - d) * Sgn(y) / 2;
    // 误差较大时，采用非线性调节

    else
        a = pStTD->x2 + y / pStTD->h;
    // 误差较小时，采用线性调节

    if (fabs(a) > d)
        fhan = -1 * pStTD->r * Sgn(a);
    else
        fhan = -1 * pStTD->r * a / d;
    // 控制速度更新的方向和幅度

    pStTD->x1 += pStTD->T * pStTD->x2;
    // 更新位置

    pStTD->x2 += pStTD->T * fhan;
    // 更新速度
}



float plan_of_time(float start_pos, float end_pos, float time)
{
    float current_pos;
    float k =0;
    if(time >= 0.0f && time <= 1.0f) k = 6*time*time*time*time*time -15*time*time*time*time + 10*time*time*time;
    else if( time > 1.0f ) k = 1;
    current_pos = start_pos + k*(end_pos - start_pos);
    return current_pos;
}


float TD_END(ST_TD *pStTD,float end_pos)
{
	pStTD->aim=end_pos;
	CalTD(pStTD);
	return pStTD->x1;
}
