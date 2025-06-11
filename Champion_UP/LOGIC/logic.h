#ifndef __LOGIC_H__
#define __LOGIC_H__

#include "types.h"


#define PITCH_INIT           0  // ��ʼλ��
#define PITCH_LAY            5000//����λ��
#define PITCH_V_INIT           -400 // ��̨��ʼ����ת��
#define ROTATE_BOUNCE             0    // ��ʼλ�ã�����λ�ã�
#define SHOT_INIT            0 // ��ʼ������λ�ã�
#define SHOT_LAY            9000 // ����
#define SHOT_V_INIT           -400 // �����ʼ���ٶ�
#define ACCEPT_FRONT_INIT        0  // ǰ�����ʼ
#define ACCEPT_BACK_INIT       0  // �������ʼ
#define ACCEPT_FRONT_ACCEPT       -62  // ǰ��������
#define ACCEPT_FRONT_SHOT       -102  // ǰ��������
#define ACCEPT_BACK_ACCEPT       52  // ����������
#define ACCEPT_FRONT_BOUNCE       0  // ǰ������
#define ACCEPT_BACK_BOUNCE       -15  // ��������
#define DRAWER_LEFT_INIT        0  // ������ʼ
#define DRAWER_RIGHT_INIT       0  // �ҳ����ʼ
#define DRAWER_LEFT_BOUNCE        630  // ���������
#define DRAWER_RIGHT_BOUNCE       -630  // �ҳ�������
#define BOUNCE_LEFT_INIT    0     // ��
#define BOUNCE_LEFT_BOUNCE    lefta    // ����
#define BOUNCE_LEFT_BOUNCE_U    leftb     // ��
#define BOUNCE_LEFT_LAY    -800    // ��
#define BOUNCE_RIGHT_INIT    0     // ��
#define BOUNCE_RIGHT_BOUNCE    righta     // �ҷ�
#define BOUNCE_RIGHT_BOUNCE_U    rightb    // ����
#define BOUNCE_RIGHT_LAY    -800    // ��
#define BOUNCE_MIDDLE_INIT   0     // ��
#define BOUNCE_MIDDLE_BOUNCE   middlea    // �з�
#define BOUNCE_MIDDLE_BOUNCE_U   middleb     // ����
#define BOUNCE_MIDDLE_LAY   -800    // ��
#define CATCH_CYLINDER_UNCATCH  0     // ��������
#define CATCH_CYLINDER_CATCH  1     
#define FIX_SERVO_INIT       0     // �̶���Ķ��
#define FIX_SERVO_FIX       140     
#define RELEASE_SERVO_INIT   120    // �ͷ��õĶ��
#define RELEASE_SERVO_RELEASE   80

#define PITCH_MAX           3000  // ��̨����
#define PITCH_MIN            -3000
#define ROW_MAX             -0.6    // ��̨J60
#define ROW_MIN            0.6 // ����
#define SHOT_MAX            50000 // 
#define SHOT_MIN           -50000//
#define ACCEPT_FRONT_MAX       0  // ǰ����
#define ACCEPT_FRONT_MIN       0  // ǰ����
#define ACCEPT_BACK_MAX       0  // ������
#define ACCEPT_BACK_MIN      0  // ������
#define DRAWER_LEFT_MAX        0  // �����
#define DRAWER_LEFT_MIN        0  // �����
#define DRAWER_RIGHT_MAX       0  // �ҳ���
#define DRAWER_RIGHT_MIN       0  // �ҳ���
#define FIX_SERVO_MAX       88     // �̶���Ķ��
#define FIX_SERVO_MIN       89     
#define RELEASE_SERVO_MAX   100    // �ͷ��õĶ��
#define RELEASE_SERVO_MIN   85




void Logic(const ST_COMMAND my_command);
void Init_Task(void);//��ʼ��
void Receive_Task(void);
void Bounce_Inited_Task(void);
void 	Lay_Task(void);
void Aim_Task(const ST_COMMAND my_command);
void Shot_Task(void);
void Force_Task(void);
void Home_Task(void);
void Stretch_Task(void);
#endif //__LOGIC_H__
