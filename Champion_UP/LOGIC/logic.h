#ifndef __LOGIC_H__
#define __LOGIC_H__

#include "types.h"


#define PITCH_INIT           0  // 初始位置
#define PITCH_LAY            5000//放球位置
#define PITCH_V_INIT           -400 // 云台初始化的转速
#define ROTATE_BOUNCE             0    // 初始位置（放球位置）
#define SHOT_INIT            0 // 初始（上膛位置）
#define SHOT_LAY            9000 // 放置
#define SHOT_V_INIT           -400 // 发射初始化速度
#define ACCEPT_FRONT_INIT        0  // 前收球初始
#define ACCEPT_BACK_INIT       0  // 后收球初始
#define ACCEPT_FRONT_ACCEPT       -62  // 前收球收球
#define ACCEPT_FRONT_SHOT       -102  // 前收球收球
#define ACCEPT_BACK_ACCEPT       52  // 后收球收球
#define ACCEPT_FRONT_BOUNCE       0  // 前收球挡球
#define ACCEPT_BACK_BOUNCE       -15  // 后收球挡球
#define DRAWER_LEFT_INIT        0  // 左抽屉初始
#define DRAWER_RIGHT_INIT       0  // 右抽屉初始
#define DRAWER_LEFT_BOUNCE        630  // 左抽屉拍球
#define DRAWER_RIGHT_BOUNCE       -630  // 右抽屉拍球
#define BOUNCE_LEFT_INIT    0     // 左
#define BOUNCE_LEFT_BOUNCE    lefta    // 左正
#define BOUNCE_LEFT_BOUNCE_U    leftb     // 左反
#define BOUNCE_LEFT_LAY    -800    // 左
#define BOUNCE_RIGHT_INIT    0     // 右
#define BOUNCE_RIGHT_BOUNCE    righta     // 右反
#define BOUNCE_RIGHT_BOUNCE_U    rightb    // 右正
#define BOUNCE_RIGHT_LAY    -800    // 右
#define BOUNCE_MIDDLE_INIT   0     // 中
#define BOUNCE_MIDDLE_BOUNCE   middlea    // 中反
#define BOUNCE_MIDDLE_BOUNCE_U   middleb     // 中正
#define BOUNCE_MIDDLE_LAY   -800    // 中
#define CATCH_CYLINDER_UNCATCH  0     // 夹球气缸
#define CATCH_CYLINDER_CATCH  1     
#define FIX_SERVO_INIT       0     // 固定球的舵机
#define FIX_SERVO_FIX       140     
#define RELEASE_SERVO_INIT   120    // 释放用的舵机
#define RELEASE_SERVO_RELEASE   80

#define PITCH_MAX           3000  // 云台俯仰
#define PITCH_MIN            -3000
#define ROW_MAX             -0.6    // 云台J60
#define ROW_MIN            0.6 // 发射
#define SHOT_MAX            50000 // 
#define SHOT_MIN           -50000//
#define ACCEPT_FRONT_MAX       0  // 前收球
#define ACCEPT_FRONT_MIN       0  // 前收球
#define ACCEPT_BACK_MAX       0  // 后收球
#define ACCEPT_BACK_MIN      0  // 后收球
#define DRAWER_LEFT_MAX        0  // 左抽屉
#define DRAWER_LEFT_MIN        0  // 左抽屉
#define DRAWER_RIGHT_MAX       0  // 右抽屉
#define DRAWER_RIGHT_MIN       0  // 右抽屉
#define FIX_SERVO_MAX       88     // 固定球的舵机
#define FIX_SERVO_MIN       89     
#define RELEASE_SERVO_MAX   100    // 释放用的舵机
#define RELEASE_SERVO_MIN   85




void Logic(const ST_COMMAND my_command);
void Init_Task(void);//初始化
void Receive_Task(void);
void Bounce_Inited_Task(void);
void 	Lay_Task(void);
void Aim_Task(const ST_COMMAND my_command);
void Shot_Task(void);
void Force_Task(void);
void Home_Task(void);
void Stretch_Task(void);
#endif //__LOGIC_H__
