#ifndef __TYPES_H__
#define __TYPES_H__


#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "stm32f4xx.h"


#define REGULATION 1//常规赛
#define TAP 0//拍球赛
#define SHOT 0//投篮赛

/**
 * 定义基本的数据类型
 * -----------------------------------------------------
 * 说明：定义了一些常用的基本数据类型，用于简化编程。
 * -----------------------------------------------------
 */
typedef unsigned char   u8;       /**< 无符号8位整数类型 */
typedef signed char     s8;       /**< 有符号8位整数类型 */
typedef unsigned short  u16;      /**< 无符号16位整数类型 */
typedef signed short    s16;      /**< 有符号16位整数类型 */
typedef unsigned int    u32;      /**< 无符号32位整数类型 */
typedef int             s32;      /**< 有符号32位整数类型 */
typedef float           FP32;     /**< 单精度浮点数类型（32位） */
typedef double          DB64;     /**< 双精度浮点数类型（64位） */

/**
 * 宏定义
 * -----------------------------------------------------
 * 说明：定义了一些常用的宏常量和函数。
 * -----------------------------------------------------
 */



#define CurrentTime HAL_GetTick()      //定义当前时间以便进行时间判断
#define pi            3.14159265358f  /**< 圆周率π的近似值 */
#define PI            3.1415926536f   /**< 圆周率π的另一个近似值 */
#define TRUE          (1)             /**< 真值 */
#define FALSE         (0)             /**< 假值 */
#define DEC           (10)            /**< 十进制 */
#define HEX           (16)            /**< 十六进制 */
#define DEG           pi / 180.0f     /**< 弧度与度之间的转换系数 */
#define BLUE          1              /**< 蓝色标识符 */
#define RED           0              /**< 红色标识符 */


/**
 * 电机控制相关常量定义
 * -----------------------------------------------------
 * 说明：定义了不同电机型号的最大电压值和齿轮比。
 * -----------------------------------------------------
 */
#define UMAX_M3508         16000     /**< M3508电机最大电压值 */
#define UMAX_M2006         10000      /**< M2006电机最大电压值 */
#define GM6020_uiGearRatio 1         /**< GM6020齿轮比 */
#define M3508_uiGearRatio  19        /**< M3508齿轮比 */
#define M2006_uiGearRatio  36        /**< M2006齿轮比 */
#define M3508_siNumber     8192      /**< M3508编码器线数 */
#define CANRXVALVEID  0x40                     //气动控制ID（电磁阀）

/** -----------------------------
================================
   ##### Structure #####
================================
 ---------------------------- **/
 /*通用结构体*/
typedef struct {
	uint16_t motor_pitch;//0
	uint16_t motor_shot_up;
	uint16_t motor_shot_down;
	uint16_t motor_rotate;
	uint16_t motor_bounce_left;//3
	uint16_t motor_bounce_right;
	uint16_t usart1;
	uint16_t freertos_logic;
	uint16_t freertos_control;//12
	uint16_t freertos_tx;
	uint16_t freertos_rx;
	uint16_t air_receive;
} MONITOR;

/*系统自检结构体*/
typedef struct
{
	MONITOR rate_cnt;
	MONITOR rate_fps;
	MONITOR system_error;
} ST_SYSTEM_MONITOR;


typedef struct
{
  uint32_t StdId;
  uint32_t ExtId;
  uint8_t IDE;
  uint8_t RTR;
  uint8_t DLC;
  uint8_t Data[8];
} CanTxMsg;//发送结构体
/****************************************************************/

/*3508电机结构体*/

/****************************************************************/
//电机码盘结构体
typedef struct {
    int siRawValue;         /**< 当前编码器原始值 */
    int siPreRawValue;      /**< 上次编码器原始值 */
    int siDiff;             /**< 编码器差值 */
    int siSumValue;         /**< 编码器累计值 */
    float siGearRatio;       /**< 齿轮比 */
    int siNumber;           /**< 编码器线数 */
    float fpSpeed;           /**< 编码器测得的速度，单位：转/分钟 */
		int state;								//初值是否置0
} ST_ENCODER;
//总结构体


typedef struct 
{
    ST_ENCODER motor_encoder; /**< 电机编码器信息 */
    float EncoderNum;         /**< 编码器计数值 */
    float encoder_speed;      /**< 编码器测得的速度 */
    float angle;              /**< 电机角度位置 */
    float anglev;             /**< 电机角速度 */
    float motor_current;      /**< 电机电流 */
    
    float outerTarget;        /**< 外环目标位置 */
    float outerFeedback;      /**< 外环反馈位置 */
    float innerFeedback;      /**< 内环反馈速度 */
} ST_MOTOR;

/****************************************************************/

/*航模电机结构体*/

/****************************************************************/
typedef struct
{
	int state;
	float motor_current;//发送的电流
	float anglev;//接收到的电机返回速度
	float mech_angle;//接收到的电机返回位置
	float angle;//处理后的电机位置
	float diff_angle;//电机位置差
	float u_anglev;//上一次电机速度
	float anglew;//计算出来的角加速度
}ST_MOTOR_H;

/****************************************************************/

/*PID结构体*/

/****************************************************************/

//PID结构体
typedef struct
{
    float fpDes; // 控制变量目标值        
    float fpFB;  // 控制变量反馈值       

    float fpKp; // 比例系数Kp          
    float fpKi; // 积分系数Ki        
    float fpKd; // 微分系数Kd      

    float fpE;    // 本次偏差       
    float fpPreE; // 上次偏差      

    float fpSumE; // 总偏差           

    float fpU; // 本次PID运算结果       
    float fpIMax;
    float fpEMax; // 做积分分离运算时偏差的最大值                                             
    float fpOMax; // 输出限幅
    float fpEMin; //偏差死区

    float fpUKp;    // 比例项输出
    float fpUKi;    // 积分项输出
    float fpUKd;    // 微分项输出
    float fpUKdpre; // 上次的微分项输出
} ST_PID;

//双环PID结构体
typedef struct 
{
    ST_PID inner;            //内环PID控制器
    ST_PID outer;            //外环PID控制器
    float output;            //级联PID控制器的最终输出
} ST_CASCADE_PID;

/*TD（微分跟踪器）结构体*/
typedef struct
{
    float x1;  // TD 生成的目标位置（平滑位置）
    float x2;  // TD 生成的目标速度（平滑速度）
    float x;   // 当前目标与实际目标的偏差，用于计算平滑控制
    float r;   // TD 阻尼因子，影响响应速度，越大响应越快
    float h;   // TD 滤波因子，影响系统平滑度，越大越平滑
    float T;   // TD 时间步长，影响更新频率，通常与系统采样时间相同
    float aim; // 系统期望达到的目标位置
} ST_TD;

//滑模控制
typedef struct
{
		FP32 fpDes;        //控制变量目标值
		FP32 fpFB;        //控制变量反馈值
		FP32 fpE;        //本次偏差
		FP32 fpU;        //本次运算结果
		FP32 fpUMax;        //输出上限
		
		//SMC参数
		FP32 b;//转动惯量倒数
		FP32 eps;//饱和函数增益项，作用是扰动补偿
		FP32 gain;//比例增益项
		FP32 dead;//饱和函数死区，在死区内为放大环节，死区外为固定值
		ST_TD TD; //使用TD来获取微分信号
}ST_SMC;

typedef struct
{
	float pitch;
	float dapao;
}ST_SHOT;

typedef struct
{
	int init;
	int receive;
	int bounce;
	int lay;
	int aim;
	int shot;
	int home;
	int flag_aim;
	int number_aim;
	int force;
	int stretch;
	int air_flag;	
 ST_SHOT command_shot;
}ST_COMMAND;


//下面这些玩意儿是声明，只能放需要在多个文件中使用的全局变量

extern uint8_t usart1_rx_buff[100];
extern uint8_t usart1_tx_buff[18];

extern uint16_t des_pitch;//云台俯仰3508
extern float des_shot;//发射3508
extern float desv_shot;//调试模式发射
extern float des_rotate;//前收球3508


extern float desv_bounce_left;//左
extern float desv_bounce_right;//右

extern uint8_t desq_catch;//收球气缸


extern uint16_t fb_pitch;//云台俯仰3508
extern float fb_shot;//发射3508
extern float fbv_shot;//发射3508（初始化用
extern float fb_rotate;//发射3508（初始化用
extern float fbv_bounce_left;//左
extern float fbv_bounce_right;//右
//****************结构体********************//

// DJI电机初始化结构体
extern ST_MOTOR motor_bounce_left;//202
extern ST_MOTOR motor_bounce_right;//203
extern ST_MOTOR motor_rotate; 
extern ST_MOTOR_H motor_shot_up;
extern ST_MOTOR_H motor_shot_down;
// TD跟踪微分器结构体
extern ST_TD td_pitch;
extern ST_TD td_shot;
extern ST_TD td_rotate;
// 级联PID控制器
extern ST_CASCADE_PID pid_pitch;
extern ST_CASCADE_PID pid_shot;
extern ST_CASCADE_PID pid_rotate;
// 单环PID控制器
extern ST_PID pid_bounce_left;
extern ST_PID pid_bounce_right;


// 系统监控和命令结构
extern ST_SYSTEM_MONITOR system_monitor;
extern ST_COMMAND command;
extern ST_SHOT shot[10];


extern float distance;
extern float yaw;
//下面这些玩意儿是一些通用函数;
float LimitMax(float fpValue, float fpMin, float fpMax);
int Sgn(float y);


extern uint16_t DC_motor_fpU;//电推杆反馈位置
extern uint16_t DC_motor_Des;//电推杆目标位置


#endif // __TYPES_H__
