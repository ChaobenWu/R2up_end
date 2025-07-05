#include "types.h"
#include "logic.h"
//fuck the robot

uint8_t usart1_rx_buff[100]={0};
uint8_t usart1_tx_buff[18]={0};

uint16_t des_pitch=7500;//云台俯仰3508
float des_shot=0;//发射3508
float desv_shot=0;//调试模式发射
float des_rotate=0;//前收球3508

/* 
以车头为前，定义左中右
*/
float desv_bounce_left=0;//左
float desv_bounce_right=0;//右

uint8_t desq_catch=1;//收球气缸


int fb_pitch=0;//云台俯仰3508
float fb_shot=0;//发射3508
float fbv_shot=0;//发射3508（初始化用
float fb_rotate=0;//发射3508
float fbv_bounce_left=0;//左
float fbv_bounce_right=0;//右

float distance=0;
float yaw=0;

int state_shot=1;
int state_pitch=0;
//*****************结构体*****************************//


//dji电机初始化赋值
ST_MOTOR motor_bounce_left = {.motor_encoder = {.siNumber = 8192, .siSumValue = 0, .siRawValue = 0,.state=1}};;//201
ST_MOTOR motor_bounce_right = {.motor_encoder = {.siNumber = 8192, .siSumValue = 0, .siRawValue = 0,.state=1}};;//202
ST_MOTOR motor_rotate = {.motor_encoder = {.siNumber = 8192, .siSumValue = 0, .siRawValue = 0,.state=1}};;//202

//航模电机初始化
ST_MOTOR_H motor_shot_up={.state=1};
ST_MOTOR_H motor_shot_down={.state=1};


//td
ST_TD td_pitch = 
{ .x1 = 0, .x2 = 0, .x = 0, .r = 1000, .T = 0.001, .h = 0.002};
ST_TD td_shot = 
{ .x1 = 0, .x2 = 0, .x = 0, .r = 10000, .T = 0.001, .h = 0.002};
ST_TD td_rotate= 
{ .x1 = 0, .x2 = 0, .x = 0, .r = 5000, .T = 0.001, .h = 0.002};


//滤波器
ST_LPF shot_up_filter={.off_freq=200,.samp_tim=0.001};
ST_LPF shot_down_filter={.off_freq=200,.samp_tim=0.001};

//pid
ST_CASCADE_PID pid_shot = {
    .inner = {
        .fpKp = 180.0f,//140
        .fpKi = 1.4f,//0.45//0.8
        .fpKd = 0.0f,
        .fpDes = 0.0f,
        .fpSumEMax = 15000.0f,
        .fpEMax = 1000.0f,
        .fpOMax = 22000.0f,
        .fpEMin = 0.5f,
        .fpFB = 0.0f,
        .fpE = 0.0f,
        .fpPreE = 0.0f,
        .fpSumE = 0.0f,
        .fpU = 0.0f,
        .fpUKp = 0.0f,
        .fpUKi = 0.0f,
        .fpUKd = 0.0f,
        .fpUKdpre = 0.0f
    },
    .outer = {
        .fpKp =0.3f,
        .fpKi = 0.0f,
        .fpKd = 0.0f,
        .fpDes = 0.0f,
        .fpSumEMax = 1000.0f,
        .fpEMax = 30.0f,
        .fpOMax = 250.0f,
        .fpEMin = 0.0f,
        .fpFB = 0.0f,
        .fpE = 0.0f,
        .fpPreE = 0.0f,
        .fpSumE = 0.0f,
        .fpU = 0.0f,
        .fpUKp = 0.0f,
        .fpUKi = 0.0f,
        .fpUKd = 0.0f,
        .fpUKdpre = 0.0f
    },
    .output = 0.0f
};

ST_CASCADE_PID pid_shot_td = {
    .inner = {
        .fpKp = 180.0f,//140
        .fpKi = 1.4f,//0.45//0.8
        .fpKd = 0.0f,
        .fpDes = 0.0f,
        .fpSumEMax = 15000.0f,
        .fpEMax = 1000.0f,
        .fpOMax = 22000.0f,
        .fpEMin = 0.5f,
        .fpFB = 0.0f,
        .fpE = 0.0f,
        .fpPreE = 0.0f,
        .fpSumE = 0.0f,
        .fpU = 0.0f,
        .fpUKp = 0.0f,
        .fpUKi = 0.0f,
        .fpUKd = 0.0f,
        .fpUKdpre = 0.0f
    },
    .outer = {
        .fpKp =0.3f,
        .fpKi = 0.0f,
        .fpKd = 0.0f,
        .fpDes = 0.0f,
        .fpSumEMax = 1000.0f,
        .fpEMax = 30.0f,
        .fpOMax = 250.0f,
        .fpEMin = 0.0f,
        .fpFB = 0.0f,
        .fpE = 0.0f,
        .fpPreE = 0.0f,
        .fpSumE = 0.0f,
        .fpU = 0.0f,
        .fpUKp = 0.0f,
        .fpUKi = 0.0f,
        .fpUKd = 0.0f,
        .fpUKdpre = 0.0f
    },
    .output = 0.0f
};




ST_CASCADE_PID pid_rotate= {
    .inner = {
        .fpKp = 200.0f,
        .fpKi = 0.001f,
        .fpKd = 200.0f,
        .fpDes = 0.0f,
        .fpSumEMax = 10000.0f,
        .fpEMax = 10000.0f,
        .fpOMax = 10000.0f,
        .fpEMin = 0.0f,
        .fpFB = 0.0f,
        .fpE = 0.0f,
        .fpPreE = 0.0f,
        .fpSumE = 0.0f,
        .fpU = 0.0f,
        .fpUKp = 0.0f,
        .fpUKi = 0.0f,
        .fpUKd = 0.0f,
        .fpUKdpre = 0.0f
    },
    .outer = {
        .fpKp = 4.0f,
        .fpKi = 0.01f,
        .fpKd = 0.0f,
        .fpDes = 0.0f,
        .fpSumEMax = 10000.0f,
        .fpEMax = 10000.0f,
        .fpOMax = 469.0f,
        .fpEMin = 0.0f,
        .fpFB = 0.0f,
        .fpE = 0.0f,
        .fpPreE = 0.0f,
        .fpSumE = 0.0f,
        .fpU = 0.0f,
        .fpUKp = 0.0f,
        .fpUKi = 0.0f,
        .fpUKd = 0.0f,
        .fpUKdpre = 0.0f
    },
    .output = 0.0f
};


ST_PID pid_bounce_left = {
        .fpKp = 45.0f,
        .fpKi = 0.005f,
        .fpKd = 2.0f,
        .fpDes = 0.0f,
        .fpSumEMax = 1000.0f,
        .fpEMax = 10.0f,
        .fpOMax = 12000.0f,
        .fpEMin = 0.0f,
        .fpFB = 0.0f,
        .fpE = 0.0f,
        .fpPreE = 0.0f,
        .fpSumE = 0.0f,
        .fpU = 0.0f,
        .fpUKp = 0.0f,
        .fpUKi = 0.0f,
        .fpUKd = 0.0f,
        .fpUKdpre = 0.0f
    };
		
ST_PID pid_bounce_right = {
        .fpKp = 45.0f,
        .fpKi = 0.005f,
        .fpKd = 2.0f,
        .fpDes = 0.0f,
        .fpSumEMax = 1000.0f,
        .fpEMax = 10.0f,
        .fpOMax = 14000.0f,
        .fpEMin = 0.0f,
        .fpFB = 0.0f,
        .fpE = 0.0f,
        .fpPreE = 0.0f,
        .fpSumE = 0.0f,
        .fpU = 0.0f,
        .fpUKp = 0.0f,
        .fpUKi = 0.0f,
        .fpUKd = 0.0f,
        .fpUKdpre = 0.0f
    };
		

		
ST_PID pid_shot_mod = {
        .fpKp = 195.0f,//140
        .fpKi = 0.041f,//0.45//0.8
        .fpKd = 0.0f,
        .fpDes = 0.0f,
        .fpSumEMax = 15000.0f,
        .fpEMax = 325.0f,
        .fpOMax = 22000.0f,
        .fpEMin = 0.0f,
        .fpFB = 0.0f,
        .fpE = 0.0f,
        .fpPreE = 0.0f,
        .fpSumE = 0.0f,
        .fpU = 0.0f,
        .fpUKp = 0.0f,
        .fpUKi = 0.0f,
        .fpUKd = 0.0f,
        .fpUKdpre = 0.0f
    };
		
ST_PID pid_pitch = {
        .fpKp = 75.0f,//95
        .fpKi = 0.1f,//0.26
        .fpKd = 200.0f,//2
        .fpDes = 0.0f,
        .fpSumEMax = 2000.0f,
        .fpEMax = 10000.0f,
        .fpOMax = 2000.0f,
        .fpEMin = 3.0f,
        .fpFB = 0.0f,
        .fpE = 0.0f,
        .fpPreE = 0.0f,
        .fpSumE = 0.0f,
        .fpU = 0.0f,
        .fpUKp = 0.0f,
        .fpUKi = 0.0f,
        .fpUKd = 0.0f,
        .fpUKdpre = 0.0f
    };
ST_SYSTEM_MONITOR system_monitor={0};

ST_COMMAND command0={0};

//备用，当垃圾的时候使用
ST_SHOT shot[5]={
{0},//0,
{.pitch=7298,.dapao=344.8},//1
{.pitch=7298,.dapao=344.8},//2
{.pitch=7298,.dapao=344.8},//3
{.pitch=7298,.dapao=344.8},//4
};


ST_SMC g_stFrictionSMC={
	.fpUMax=22000,
	.b=0.5,
	.eps=200,
	.gain=300,
	.dead=10,
	.TD={0,0,0,800,50,2,0}
};

ST_LESO_1order order1=
{
  .Beta01=60,//100
  .Beta02=900,//2500
  .b0=600,//300
	.fpUMax=22000,
	.h=0.001
};

ST_LESO_1order order2=
{
  .Beta01=60,//100
  .Beta02=900,//2500
  .b0=600,//300
	.fpUMax=22000,
	.h=0.001
};

ST_LESO_1order order3=
{
  .Beta01=60,//100
  .Beta02=900,//2500
  .b0=600,//300
	.fpUMax=22000,
	.h=0.001
};
/*******************************************************************************************
函数名称：LimitMax()
函数功能：限制输入值的范围，将其约束在指定的最小值和最大值之间
输入：   1. fpValue 浮点数类型的输入值
          2. fpMin   浮点数类型的最小允许值
          3. fpMax   浮点数类型的最大允许值
输出：   1. 限制后的浮点数值：
             - 若输入值小于最小值，则返回最小值 fpMin
             - 若输入值大于最大值，则返回最大值 fpMax
             - 若输入值在范围内，则返回原值 fpValue
备注：   用于限制PID计算的误差项、比例项、微分项
*******************************************************************************************/
float LimitMax(float fpValue, float fpMin, float fpMax)
{
    float temp;
    if (fpValue < fpMin)
    {
        temp = fpMin;
    }
    else if (fpValue > fpMax)
    {
        temp = fpMax;
    }
    else
    {
        temp = fpValue;
    }
    return temp;
}


/*******************************************************************************************
函数名称：Sgn()
函数功能：判断输入值的符号，并返回对应的结果
输入：   1. y 浮点数类型的输入值
输出：   1. 符号函数结果：
             - 返回 1 表示输入值为正
             - 返回 -1 表示输入值为负
             - 返回 0 表示输入值为零
*******************************************************************************************/
int Sgn(float y)
{
    if (y > 0)
    {
        return 1;
    }
    else if (y < 0)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}



/*-------------------------------------------------------------------------------------------------
函数功能：一二阶LESO算法，通过状态观测器来观测扰动，在输出电流量中补偿掉
-------------------------------------------------------------------------------------------------*/
void LESO_Order1(ST_LESO_1order * leso_1order, float y,float U0)
{

  leso_1order->U0 = U0;

  leso_1order->E = y - leso_1order->Z1;
	
	leso_1order->Z1 += leso_1order->h*(leso_1order->b0 * leso_1order->U0+ leso_1order->Z2+leso_1order->Beta01 * leso_1order->E);

	leso_1order->Z2 += leso_1order->h*leso_1order->Beta02 * leso_1order->E;
	
	leso_1order->U = leso_1order->U0 - leso_1order->Z2/leso_1order->b0;

	if(leso_1order->U>=leso_1order->fpUMax) leso_1order->U = leso_1order->fpUMax;
	if(leso_1order->U<=-leso_1order->fpUMax) leso_1order->U = -leso_1order->fpUMax;
}



/*******************************************************************************************
函数名称：LpFilter()
函数功能：一阶低通滤波器，用于平滑输入信号，去除高频噪声
输入：   1. lpf 指向低通滤波器结构体的指针，结构体成员包括：
             - off_freq：截止频率
             - samp_tim：采样时间
             - in：当前输入信号
             - preout：上一次输出信号
输出：   1. lpf->out：滤波后的输出信号
原理：   1. 滤波器通过加权平均当前输入信号和上一次输出信号来平滑信号：
             - fir_a = 1 / (1 + off_freq * samp_tim)
             - out = fir_a * preout + (1 - fir_a) * in
          2. `fir_a` 控制滤波器对历史数据和当前数据的加权：
             - `fir_a` 越小，表示滤波器对历史数据的加权越大，输出更平滑但响应较慢。
             - `fir_a` 越大，表示滤波器对当前数据的加权越大，响应更快但平滑效果减弱。
          3. 本函数常用于传感器数据的滤波处理，尤其适用于需要去除高频噪声的信号，如加速度计、陀螺仪等。
*******************************************************************************************/
void LpFilter(ST_LPF *lpf)
{
    // 计算滤波器系数 fir_a，基于截止频率和采样时间的典型公式：
    // fir_a = 1 / (1 + 截止频率 * 采样时间)
    float fir_a = 1 / (1 + lpf->off_freq * lpf->samp_tim);

    // 根据当前输入信号和上一次的输出信号计算新的输出值：
    // 输出是当前输入信号和历史输出信号的加权平均，权重由 fir_a 决定
    lpf->out = fir_a * lpf->preout + (1 - fir_a) * lpf->in;

    // 更新历史输出值，为下次调用提供参考
    lpf->preout = lpf->out;
}
