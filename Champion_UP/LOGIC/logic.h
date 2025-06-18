#ifndef __LOGIC_H__
#define __LOGIC_H__

#include "types.h"


#define PITCH_INIT           7400  // 初始位置
#define PITCH_LAY            5000//放球位置
#define ROTATE_BOUNCE             0    // 拍球
#define ROTATE_INIT               450    // 初始
#define ROTATE_LAY                570    // 放球
#define SHOT_INIT            -700 // 初始
#define SHOT_START            -50 // 发射启动点
#define SHOT_SHOT            -2200// 发射点
#define SHOT_LAY            -950 // 放置
#define BOUNCE_LEFT_INIT    0     // 左
#define BOUNCE_LEFT_BOUNCE    lefta    // 左正
#define BOUNCE_LEFT_BOUNCE_U    leftb     // 左反
#define BOUNCE_LEFT_LAY    -800    // 左
#define BOUNCE_RIGHT_INIT    0     // 右
#define BOUNCE_RIGHT_BOUNCE    righta     // 右反
#define BOUNCE_RIGHT_BOUNCE_U    rightb    // 右正
#define BOUNCE_RIGHT_LAY    900    // 右
#define CATCH_CYLINDER_UNCATCH  0     // 夹球气缸
#define CATCH_CYLINDER_CATCH  1     



#define PITCH_MAX           3000  // 云台俯仰
#define PITCH_MIN            -3000

#define SHOT_MAX            50000 // 
#define SHOT_MIN           -50000//

extern uint8_t flag_bounce;
extern uint8_t flag_lay;
extern uint8_t flag_aim;
extern uint8_t flag_shot;
extern uint8_t flag_init;
extern uint8_t flag_start;


void Logic(const ST_COMMAND my_command);
void Init_Task(void);//初始化
void Start_Task(void);
void Receive_Task(void);
void Bounce_Task(const ST_COMMAND my_command);
void 	Lay_Task(void);
void Aim_Task(const ST_COMMAND my_command);
void Belt_Shot(const ST_COMMAND my_command);
#endif //__LOGIC_H__
