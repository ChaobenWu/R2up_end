#ifndef __BSP_CAN_H__
#define __BSP_CAN_H__

#include "types.h"
#include "stm32f4xx_hal_can.h"


#define POSITION_MIN -40.0f
#define POSITION_MAX 40.0f
#define VELOCITY_MIN -40.0f
#define VELOCITY_MAX 40.0f
#define KP_MIN 0.0f
#define KP_MAX 1023.0f
#define KD_MIN 0.0f
#define KD_MAX 51.0f
#define TORQUE_MIN -40.0f
#define TORQUE_MAX 40.0f

#define SEND_POSITION_LENGTH 16
#define SEND_VELOCITY_LENGTH 14
#define SEND_KP_LENGTH 10
#define SEND_KD_LENGTH 8
#define SEND_TORQUE_LENGTH 16
#define SEND_GEAR_RATIO_LENGTH 16
#define SEND_LIMIT_CURRENT_LENGTH 16

#define RECEIVE_POSITION_LENGTH 20
#define RECEIVE_VELOCITY_LENGTH 20
#define RECEIVE_TORQUE_LENGTH 16
#define RECEIVE_TEMP_FLAG_LENGTH 1
#define RECEIVE_TEMP_LENGTH 7

#define MOTOR_TEMP_MIN -20.0f
#define MOTOR_TEMP_MAX 200.0f
#define DRIVER_TEMP_MIN -20.0f
#define DRIVER_TEMP_MAX 200.0f
#define CURRENT_MIN 0.0f
#define CURRENT_MAX 40.0f

enum Temp_Flag{
    kDriverTempFlag=0,
    kMotorTempFlag=1
};

void can1_start(void);
void CAN1_FILTER_CONFIG(CAN_HandleTypeDef* hcan);
void can2_start(void);
void CAN2_FILTER_CONFIG(CAN_HandleTypeDef* hcan);
void CANX_SendStdData(CAN_HandleTypeDef* hcan, uint16_t ID, uint8_t *pData, uint16_t Len);
void CAN_SendCurrent(CAN_HandleTypeDef *hcan, uint32_t id,int16_t current1,int16_t current2,int16_t current3,int16_t current4);
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
int16_t GetEncoderNumber(ST_MOTOR* motor,uint8_t msg[8]);
float GetSpeed(CAN_RxHeaderTypeDef* pCanRxMsg,uint8_t msg[8]);
void Abs_Encoder_Process(ST_ENCODER* pEncoder, uint32_t value);
void SendSwitchValue(uint8_t Switch1,uint8_t Switch2,uint8_t Switch3);
void Cacul_w(ST_MOTOR_H* motor);
#endif // CAN1_H
