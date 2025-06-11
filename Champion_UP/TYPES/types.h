#ifndef __TYPES_H__
#define __TYPES_H__


#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "stm32f4xx.h"


#define REGULATION 1//������
#define TAP 0//������
#define SHOT 0//Ͷ����

/**
 * �����������������
 * -----------------------------------------------------
 * ˵����������һЩ���õĻ����������ͣ����ڼ򻯱�̡�
 * -----------------------------------------------------
 */
typedef unsigned char   u8;       /**< �޷���8λ�������� */
typedef signed char     s8;       /**< �з���8λ�������� */
typedef unsigned short  u16;      /**< �޷���16λ�������� */
typedef signed short    s16;      /**< �з���16λ�������� */
typedef unsigned int    u32;      /**< �޷���32λ�������� */
typedef int             s32;      /**< �з���32λ�������� */
typedef float           FP32;     /**< �����ȸ��������ͣ�32λ�� */
typedef double          DB64;     /**< ˫���ȸ��������ͣ�64λ�� */

/**
 * �궨��
 * -----------------------------------------------------
 * ˵����������һЩ���õĺ곣���ͺ�����
 * -----------------------------------------------------
 */



#define CurrentTime HAL_GetTick()      //���嵱ǰʱ���Ա����ʱ���ж�
#define pi            3.14159265358f  /**< Բ���ʦеĽ���ֵ */
#define PI            3.1415926536f   /**< Բ���ʦе���һ������ֵ */
#define TRUE          (1)             /**< ��ֵ */
#define FALSE         (0)             /**< ��ֵ */
#define DEC           (10)            /**< ʮ���� */
#define HEX           (16)            /**< ʮ������ */
#define DEG           pi / 180.0f     /**< �������֮���ת��ϵ�� */
#define BLUE          1              /**< ��ɫ��ʶ�� */
#define RED           0              /**< ��ɫ��ʶ�� */


/**
 * ���������س�������
 * -----------------------------------------------------
 * ˵���������˲�ͬ����ͺŵ�����ѹֵ�ͳ��ֱȡ�
 * -----------------------------------------------------
 */
#define UMAX_M3508         16000     /**< M3508�������ѹֵ */
#define UMAX_M2006         10000      /**< M2006�������ѹֵ */
#define GM6020_uiGearRatio 1         /**< GM6020���ֱ� */
#define M3508_uiGearRatio  19        /**< M3508���ֱ� */
#define M2006_uiGearRatio  36        /**< M2006���ֱ� */
#define M3508_siNumber     8192      /**< M3508���������� */
#define CANRXVALVEID  0x40                     //��������ID����ŷ���

/** -----------------------------
================================
   ##### Structure #####
================================
 ---------------------------- **/
 /*ͨ�ýṹ��*/
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

/*ϵͳ�Լ�ṹ��*/
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
} CanTxMsg;//���ͽṹ��
/****************************************************************/

/*3508����ṹ��*/

/****************************************************************/
//������̽ṹ��
typedef struct {
    int siRawValue;         /**< ��ǰ������ԭʼֵ */
    int siPreRawValue;      /**< �ϴα�����ԭʼֵ */
    int siDiff;             /**< ��������ֵ */
    int siSumValue;         /**< �������ۼ�ֵ */
    float siGearRatio;       /**< ���ֱ� */
    int siNumber;           /**< ���������� */
    float fpSpeed;           /**< ��������õ��ٶȣ���λ��ת/���� */
		int state;								//��ֵ�Ƿ���0
} ST_ENCODER;
//�ܽṹ��


typedef struct 
{
    ST_ENCODER motor_encoder; /**< �����������Ϣ */
    float EncoderNum;         /**< ����������ֵ */
    float encoder_speed;      /**< ��������õ��ٶ� */
    float angle;              /**< ����Ƕ�λ�� */
    float anglev;             /**< ������ٶ� */
    float motor_current;      /**< ������� */
    
    float outerTarget;        /**< �⻷Ŀ��λ�� */
    float outerFeedback;      /**< �⻷����λ�� */
    float innerFeedback;      /**< �ڻ������ٶ� */
} ST_MOTOR;

/****************************************************************/

/*��ģ����ṹ��*/

/****************************************************************/
typedef struct
{
	int state;
	float motor_current;//���͵ĵ���
	float anglev;//���յ��ĵ�������ٶ�
	float mech_angle;//���յ��ĵ������λ��
	float angle;//�����ĵ��λ��
	float diff_angle;//���λ�ò�
	float u_anglev;//��һ�ε���ٶ�
	float anglew;//��������ĽǼ��ٶ�
}ST_MOTOR_H;

/****************************************************************/

/*PID�ṹ��*/

/****************************************************************/

//PID�ṹ��
typedef struct
{
    float fpDes; // ���Ʊ���Ŀ��ֵ        
    float fpFB;  // ���Ʊ�������ֵ       

    float fpKp; // ����ϵ��Kp          
    float fpKi; // ����ϵ��Ki        
    float fpKd; // ΢��ϵ��Kd      

    float fpE;    // ����ƫ��       
    float fpPreE; // �ϴ�ƫ��      

    float fpSumE; // ��ƫ��           

    float fpU; // ����PID������       
    float fpIMax;
    float fpEMax; // �����ַ�������ʱƫ������ֵ                                             
    float fpOMax; // ����޷�
    float fpEMin; //ƫ������

    float fpUKp;    // ���������
    float fpUKi;    // ���������
    float fpUKd;    // ΢�������
    float fpUKdpre; // �ϴε�΢�������
} ST_PID;

//˫��PID�ṹ��
typedef struct 
{
    ST_PID inner;            //�ڻ�PID������
    ST_PID outer;            //�⻷PID������
    float output;            //����PID���������������
} ST_CASCADE_PID;

/*TD��΢�ָ��������ṹ��*/
typedef struct
{
    float x1;  // TD ���ɵ�Ŀ��λ�ã�ƽ��λ�ã�
    float x2;  // TD ���ɵ�Ŀ���ٶȣ�ƽ���ٶȣ�
    float x;   // ��ǰĿ����ʵ��Ŀ���ƫ����ڼ���ƽ������
    float r;   // TD �������ӣ�Ӱ����Ӧ�ٶȣ�Խ����ӦԽ��
    float h;   // TD �˲����ӣ�Ӱ��ϵͳƽ���ȣ�Խ��Խƽ��
    float T;   // TD ʱ�䲽����Ӱ�����Ƶ�ʣ�ͨ����ϵͳ����ʱ����ͬ
    float aim; // ϵͳ�����ﵽ��Ŀ��λ��
} ST_TD;

//��ģ����
typedef struct
{
		FP32 fpDes;        //���Ʊ���Ŀ��ֵ
		FP32 fpFB;        //���Ʊ�������ֵ
		FP32 fpE;        //����ƫ��
		FP32 fpU;        //����������
		FP32 fpUMax;        //�������
		
		//SMC����
		FP32 b;//ת����������
		FP32 eps;//���ͺ���������������Ŷ�����
		FP32 gain;//����������
		FP32 dead;//���ͺ�����������������Ϊ�Ŵ󻷽ڣ�������Ϊ�̶�ֵ
		ST_TD TD; //ʹ��TD����ȡ΢���ź�
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


//������Щ�������������ֻ�ܷ���Ҫ�ڶ���ļ���ʹ�õ�ȫ�ֱ���

extern uint8_t usart1_rx_buff[100];
extern uint8_t usart1_tx_buff[18];

extern uint16_t des_pitch;//��̨����3508
extern float des_shot;//����3508
extern float desv_shot;//����ģʽ����
extern float des_rotate;//ǰ����3508


extern float desv_bounce_left;//��
extern float desv_bounce_right;//��

extern uint8_t desq_catch;//��������


extern uint16_t fb_pitch;//��̨����3508
extern float fb_shot;//����3508
extern float fbv_shot;//����3508����ʼ����
extern float fb_rotate;//����3508����ʼ����
extern float fbv_bounce_left;//��
extern float fbv_bounce_right;//��
//****************�ṹ��********************//

// DJI�����ʼ���ṹ��
extern ST_MOTOR motor_bounce_left;//202
extern ST_MOTOR motor_bounce_right;//203
extern ST_MOTOR motor_rotate; 
extern ST_MOTOR_H motor_shot_up;
extern ST_MOTOR_H motor_shot_down;
// TD����΢�����ṹ��
extern ST_TD td_pitch;
extern ST_TD td_shot;
extern ST_TD td_rotate;
// ����PID������
extern ST_CASCADE_PID pid_pitch;
extern ST_CASCADE_PID pid_shot;
extern ST_CASCADE_PID pid_rotate;
// ����PID������
extern ST_PID pid_bounce_left;
extern ST_PID pid_bounce_right;


// ϵͳ��غ�����ṹ
extern ST_SYSTEM_MONITOR system_monitor;
extern ST_COMMAND command;
extern ST_SHOT shot[10];


extern float distance;
extern float yaw;
//������Щ�������һЩͨ�ú���;
float LimitMax(float fpValue, float fpMin, float fpMax);
int Sgn(float y);


extern uint16_t DC_motor_fpU;//���Ƹ˷���λ��
extern uint16_t DC_motor_Des;//���Ƹ�Ŀ��λ��


#endif // __TYPES_H__
