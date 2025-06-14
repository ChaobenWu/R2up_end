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


uint16_t fb_pitch=0;//云台俯仰3508
float fb_shot=0;//发射3508
float fbv_shot=0;//发射3508（初始化用
float fb_rotate=0;//发射3508
float fbv_bounce_left=0;//左
float fbv_bounce_right=0;//右

float distance=0;
float yaw=0;

int state=1;
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


//pid
ST_CASCADE_PID pid_shot = {
    .inner = {
        .fpKp = 170.0f,
        .fpKi = 0.1f,
        .fpKd = 0.0f,
        .fpDes = 0.0f,
        .fpSumEMax = 10000.0f,
        .fpEMax = 100.0f,
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
    },
    .outer = {
        .fpKp =1.0f,
        .fpKi = 0.004f,
        .fpKd = 0.0f,
        .fpDes = 0.0f,
        .fpSumEMax = 1000.0f,
        .fpEMax = 30.0f,
        .fpOMax = 180.0f,
        .fpEMin = 0.8f,
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
        .fpKd = 0.0f,
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
        .fpKp = 80.0f,
        .fpKi = 0.8f,
        .fpKd = 2.0f,
        .fpDes = 0.0f,
        .fpSumEMax = 2000.0f,
        .fpEMax = 1000.0f,
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
        .fpKp = 75.0f,
        .fpKi = 0.025f,
        .fpKd = 2.0f,
        .fpDes = 0.0f,
        .fpSumEMax = 2000.0f,
        .fpEMax = 1000.0f,
        .fpOMax = 2000.0f,
        .fpEMin = 6.0f,
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

ST_COMMAND command;


//备用，当垃圾的时候使用
ST_SHOT shot[10]={
{0},//0,
{.pitch=7000,.dapao=12000},//1
{.pitch=1500,.dapao=3000},//2
{.pitch=1500,.dapao=3000},//3
{.pitch=1500,.dapao=3000},//4
{.pitch=1500,.dapao=3000},//5
{.pitch=1500,.dapao=3000},//6
{.pitch=1500,.dapao=3000},//7
{.pitch=1500,.dapao=3000},//8
{.pitch=1500,.dapao=3000},//9
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
