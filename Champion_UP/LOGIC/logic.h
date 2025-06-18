#ifndef __LOGIC_H__
#define __LOGIC_H__

#include "types.h"


#define PITCH_INIT           7400  // ��ʼλ��
#define PITCH_LAY            5000//����λ��
#define ROTATE_BOUNCE             0    // ����
#define ROTATE_INIT               450    // ��ʼ
#define ROTATE_LAY                570    // ����
#define SHOT_INIT            -700 // ��ʼ
#define SHOT_START            -50 // ����������
#define SHOT_SHOT            -2200// �����
#define SHOT_LAY            -950 // ����
#define BOUNCE_LEFT_INIT    0     // ��
#define BOUNCE_LEFT_BOUNCE    lefta    // ����
#define BOUNCE_LEFT_BOUNCE_U    leftb     // ��
#define BOUNCE_LEFT_LAY    -800    // ��
#define BOUNCE_RIGHT_INIT    0     // ��
#define BOUNCE_RIGHT_BOUNCE    righta     // �ҷ�
#define BOUNCE_RIGHT_BOUNCE_U    rightb    // ����
#define BOUNCE_RIGHT_LAY    900    // ��
#define CATCH_CYLINDER_UNCATCH  0     // ��������
#define CATCH_CYLINDER_CATCH  1     



#define PITCH_MAX           3000  // ��̨����
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
void Init_Task(void);//��ʼ��
void Start_Task(void);
void Receive_Task(void);
void Bounce_Task(const ST_COMMAND my_command);
void 	Lay_Task(void);
void Aim_Task(const ST_COMMAND my_command);
void Belt_Shot(const ST_COMMAND my_command);
#endif //__LOGIC_H__
