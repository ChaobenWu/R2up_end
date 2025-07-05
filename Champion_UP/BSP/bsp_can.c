#include "bsp_can.h"
#include "can.h"
#include "types.h"
uint8_t aaabaa;
//CAN��ʼ������
void can1_start(void) {
    CAN1_FILTER_CONFIG(&hcan1); //�˲�����ʼ��
    HAL_CAN_Start(&hcan1); //����CAN1
    HAL_CAN_ActivateNotification(&hcan1,CAN_IT_RX_FIFO0_MSG_PENDING); //ʹ���ж�
}

/**
 * @brief �������
 * @param CAN1 or CAN2
 * @retval None
 */
void CAN1_FILTER_CONFIG(CAN_HandleTypeDef* hcan) {
		CAN_FilterTypeDef CAN_FilterConfigStructure;
    CAN_FilterConfigStructure.FilterBank = 0;
    CAN_FilterConfigStructure.FilterMode = CAN_FILTERMODE_IDMASK;
    CAN_FilterConfigStructure.FilterScale = CAN_FILTERSCALE_32BIT;
    CAN_FilterConfigStructure.FilterIdHigh = 0x7FFE;
    CAN_FilterConfigStructure.FilterMaskIdHigh = 0x0000;
    CAN_FilterConfigStructure.FilterIdLow = 0x0000;
    CAN_FilterConfigStructure.FilterMaskIdLow = 0x0000;
    CAN_FilterConfigStructure.FilterFIFOAssignment = CAN_FilterFIFO0;
    CAN_FilterConfigStructure.SlaveStartFilterBank = 14;
    CAN_FilterConfigStructure.FilterActivation = ENABLE;
		
    HAL_CAN_ConfigFilter(&hcan1, &CAN_FilterConfigStructure);
    HAL_CAN_Start(&hcan1);
    HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
}

void can2_start(void) {
    CAN2_FILTER_CONFIG(&hcan2); //�˲�����ʼ��
    HAL_CAN_Start(&hcan2); //����CAN2
    HAL_CAN_ActivateNotification(&hcan2,CAN_IT_RX_FIFO0_MSG_PENDING); //ʹ���ж�
}

/**
 * @brief �������
 * @param CAN1 or CAN2
 * @retval None
 */
void CAN2_FILTER_CONFIG(CAN_HandleTypeDef* hcan) {
		CAN_FilterTypeDef CAN_FilterConfigStructure;
    CAN_FilterConfigStructure.FilterBank = 14;
    CAN_FilterConfigStructure.FilterMode = CAN_FILTERMODE_IDMASK;
    CAN_FilterConfigStructure.FilterScale = CAN_FILTERSCALE_32BIT;
    CAN_FilterConfigStructure.FilterIdHigh = 0x7FFE;
    CAN_FilterConfigStructure.FilterMaskIdHigh = 0x0000;
    CAN_FilterConfigStructure.FilterIdLow = 0x0000;
    CAN_FilterConfigStructure.FilterMaskIdLow = 0x0000;
    CAN_FilterConfigStructure.FilterFIFOAssignment = CAN_FilterFIFO0;
    CAN_FilterConfigStructure.SlaveStartFilterBank = 28;
    CAN_FilterConfigStructure.FilterActivation = ENABLE;
		
    HAL_CAN_ConfigFilter(&hcan2, &CAN_FilterConfigStructure);
    HAL_CAN_Start(&hcan2);
    HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);
}
/**
 * @brief ���ͱ�׼ID����Ϣ����
 * @param hcan CAN���
 * @param ID ����֡ID
 * @param pData ����ָ��
 * @param Len �ֽ���(8~8), ��ѡ������0x08
 */
void CANX_SendStdData(CAN_HandleTypeDef* hcan, uint16_t ID, uint8_t *pData, uint16_t Len)
{
    static CAN_TxHeaderTypeDef Tx_Header; // ���徲̬��Tx Header�ṹ�����

    Tx_Header.StdId = ID; // ��ID��ֵ����׼ID��
    Tx_Header.ExtId = CAN_ID_STD; // ������չ��ʶ��Ϊ��׼ID
    Tx_Header.IDE = CAN_RTR_DATA; // ����IDEΪ����֡
    Tx_Header.RTR = 0; // RTR����Ϊ0����ʾ����֡
    Tx_Header.DLC = Len; // DLC����Ϊ����

    // �ҿյķ������䣬�����ݷ��ͳ�ȥ
    if (HAL_CAN_AddTxMessage(hcan, &Tx_Header, pData, (uint32_t*)CAN_TX_MAILBOX0) != HAL_OK) 
    {
        if(HAL_CAN_AddTxMessage(hcan, &Tx_Header, pData, (uint32_t*)CAN_TX_MAILBOX1)!= HAL_OK)
				{
					HAL_CAN_AddTxMessage(hcan, &Tx_Header, pData, (uint32_t*)CAN_TX_MAILBOX2);
				}
    }
}
/**
 * @brief DIJϵ�е���������ͺ���
 * @param hcan CAN���
 * @param id ���ID
 * @param current1 ����1
 * @param current2 ����2
 * @param current3 ����3
 * @param current4 ����4
 * @note
 * @author HITCRT
 */
void CAN_SendCurrent(CAN_HandleTypeDef *hcan, uint32_t id,int16_t current1,int16_t current2,int16_t current3,int16_t current4)
{
    uint8_t TxCurrent[8]; // ����һ��8�ֽڵ�����������ŵ�������

    TxCurrent[0] = (current1>>8); // ������1�ĸ�λ��������ĵ�һ��Ԫ��
    TxCurrent[1] = current1; // ������1�ĵ�λ��������ĵڶ���Ԫ��
    TxCurrent[2] = (current2>>8); // ������2�ĸ�λ��������ĵ�����Ԫ��
    TxCurrent[3] = current2; // ������2�ĵ�λ��������ĵ��ĸ�Ԫ��
    TxCurrent[4] = (current3>>8); // ������3�ĸ�λ��������ĵ����Ԫ��
    TxCurrent[5] = current3; // ������3�ĵ�λ��������ĵ�����Ԫ��
    TxCurrent[6] = (current4>>8); // ������4�ĸ�λ��������ĵ��߸�Ԫ��
    TxCurrent[7] = current4; // ������4�ĵ�λ��������ĵڰ˸�Ԫ��

    CANX_SendStdData(hcan,id,TxCurrent,8); // ����CANX_SendStdData������������
}
//***********************************************************************************************//
//�������Ǹ��ǵ�can���ܺ���

//***********************************************************************************************//


uint8_t RxMsg_CAN1[8]; // ����CAN1ͨ�Ž��ջ�����
uint8_t RxMsg_CAN2[8]; // ����CAN2ͨ�Ž��ջ�����

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
		if(hcan->Instance == CAN1) // �ж��Ƿ�ΪCAN1�ӿ�
		{
        CAN_RxHeaderTypeDef RxHeader;
        HAL_CAN_GetRxMessage(hcan,CAN_RX_FIFO0,&RxHeader,RxMsg_CAN1); // ��ȡ���յ�����Ϣͷ��Ϣ������

        switch(RxHeader.StdId) // ���ݽ��յ��ı�׼ID���д����õ�
        {

					  case 0x310: // ���Ƹ�
							fb_pitch = ((RxMsg_CAN1[1]<<8)&0xFF00)|(RxMsg_CAN1[0]&0x00FF);
							system_monitor.rate_cnt.motor_pitch++;
							
							
							if((RxMsg_CAN1[2] & 0x08) == 0)
							{
								command0.air_flag=0;
							}
							if((RxMsg_CAN1[2] & 0x08) == 0x08)
							{
								command0.air_flag=1;
							}							
							system_monitor.rate_cnt.air_receive++;							
							break;
//					  case 0x30: // �г̿���
//							if((RxMsg_CAN1[1] & 0x08) == 0)
//							{
//								command0.air_flag=0;
//							}
//							if((RxMsg_CAN1[1] & 0x08) == 0x08)
//							{
//								command0.air_flag=1;
//							}
//							system_monitor.rate_cnt.air_receive++;
//							break;			
						case 0x201://�º�ģ
							if(motor_shot_down.state==1)
							{		
								memcpy(&motor_shot_down.diff_angle, &RxMsg_CAN1, 4);  // λ��
								motor_shot_down.state=0;
							}
							else
							{
								memcpy(&motor_shot_down.mech_angle, &RxMsg_CAN1, 4);  // λ��
								motor_shot_down.angle=motor_shot_down.mech_angle-motor_shot_down.diff_angle;
							}
							memcpy(&motor_shot_down.mech_anglev, &RxMsg_CAN1[4], 4);  // �ٶ�
							shot_down_filter.in=motor_shot_down.mech_anglev;
							LpFilter(&shot_down_filter);
							motor_shot_down.anglev=shot_down_filter.out;
							Cacul_w(&motor_shot_down);
							system_monitor.rate_cnt.motor_shot_down++;
							break;
						case 0x202://�Ϻ�ģ
							if(motor_shot_up.state==1)
							{		
								memcpy(&motor_shot_up.diff_angle, &RxMsg_CAN1, 4);  // λ��
								motor_shot_up.state=0;
							}
							else
							{
								memcpy(&motor_shot_up.mech_angle, &RxMsg_CAN1, 4);  // λ��
								motor_shot_up.angle=motor_shot_up.mech_angle-motor_shot_up.diff_angle;
							}
							memcpy(&motor_shot_up.mech_anglev, &RxMsg_CAN1[4], 4);  // �ٶ�
							shot_up_filter.in=motor_shot_up.mech_anglev;
							LpFilter(&shot_up_filter);
							motor_shot_up.anglev=shot_up_filter.out;
							Cacul_w(&motor_shot_up);
							fbv_shot=motor_shot_up.anglev;
							fb_shot=motor_shot_up.angle;
							system_monitor.rate_cnt.motor_shot_up++;
							break;
            default:
              break;

        }
    }
		if(hcan->Instance == CAN2) // �ж��Ƿ�ΪCAN2�ӿ�
		{
        CAN_RxHeaderTypeDef RxHeader2;
        HAL_CAN_GetRxMessage(hcan,CAN_RX_FIFO0,&RxHeader2,RxMsg_CAN2); // ��ȡ���յ�����Ϣͷ��Ϣ������

        switch(RxHeader2.StdId) // ���ݽ��յ��ı�׼ID���д����õ�
        {	
            case 0x201: // 2006���
                motor_rotate.EncoderNum = GetEncoderNumber(&motor_rotate,RxMsg_CAN2);
                motor_rotate.encoder_speed = GetSpeed(&RxHeader2,RxMsg_CAN2);
                Abs_Encoder_Process(&motor_rotate.motor_encoder,motor_rotate.EncoderNum);
                motor_rotate.angle=motor_rotate.motor_encoder.siSumValue/(float)M3508_siNumber*360.f/M2006_uiGearRatio; // �Ƕȼ���
                motor_rotate.anglev=motor_rotate.encoder_speed/(float)M3508_uiGearRatio; // ���ٶȼ���
								fb_rotate=motor_rotate.angle;
								system_monitor.rate_cnt.motor_rotate++;
                break;					
            case 0x202 :// 3508���
                motor_bounce_right.EncoderNum = GetEncoderNumber(&motor_bounce_right,RxMsg_CAN2);
                motor_bounce_right.encoder_speed = GetSpeed(&RxHeader2,RxMsg_CAN2);
                Abs_Encoder_Process(&motor_bounce_right.motor_encoder,motor_bounce_right.EncoderNum);
                motor_bounce_right.angle=motor_bounce_right.motor_encoder.siSumValue/(float)M3508_siNumber*360.f; // �Ƕȼ���
                motor_bounce_right.anglev=motor_bounce_right.encoder_speed; // ���ٶȼ���
								fbv_bounce_right=motor_bounce_right.anglev;
								system_monitor.rate_cnt.motor_bounce_right++;
                break;	
            case 0x203: // 3508���
                motor_bounce_left.EncoderNum = GetEncoderNumber(&motor_bounce_left,RxMsg_CAN2);
                motor_bounce_left.encoder_speed = GetSpeed(&RxHeader2,RxMsg_CAN2);
                Abs_Encoder_Process(&motor_bounce_left.motor_encoder,motor_bounce_left.EncoderNum);
                motor_bounce_left.angle=motor_bounce_left.motor_encoder.siSumValue/(float)M3508_siNumber*360.f; // �Ƕȼ���
                motor_bounce_left.anglev=motor_bounce_left.encoder_speed; // ���ٶȼ���
								fbv_bounce_left=motor_bounce_left.anglev;
								system_monitor.rate_cnt.motor_bounce_left++;	
								break;	
            default:
                break;
        }
    }
}
// �������ƣ�GetEncoderNumber
// ���ܣ���CAN��Ϣ����ȡ��������ֵ
// ������motor ָ��MOTOR���͵�ָ�룬msg ���յ�CAN��Ϣ
int16_t GetEncoderNumber(ST_MOTOR* motor,uint8_t msg[8])
{
    motor->EncoderNum = (msg[0]<<8 | msg[1]);
    return motor->EncoderNum;
}

// �������ƣ�GetSpeed
// ���ܣ���CAN��Ϣ����ȡ�ٶ���Ϣ
// ������pCanRxMsg ָ��CAN_RxHeaderTypeDef���͵�ָ�룬msg ���յ�CAN��Ϣ
float GetSpeed(CAN_RxHeaderTypeDef* pCanRxMsg,uint8_t msg[8])
{
    int32_t speed_temp;
    int32_t base_value = 0xFFFFF;
    
    if(msg[2] & 0x01<<7) // �ж����λ�Ƿ�Ϊ1
    {
        speed_temp = (base_value<<16 | msg[2]<<8 | msg[3]);
    }
    else
    {
        speed_temp = (msg[2]<<8 | msg[3]);
    }

    return speed_temp;
}



void Abs_Encoder_Process(ST_ENCODER* pEncoder, uint32_t value)
{
	pEncoder->siPreRawValue =pEncoder->siRawValue;
	pEncoder->siRawValue =value;
	pEncoder->siDiff = pEncoder->siRawValue - pEncoder->siPreRawValue;
	if(pEncoder->siDiff>(pEncoder->siNumber)/2)
	{
		pEncoder->siDiff -= pEncoder->siNumber;
	}
	else if(pEncoder->siDiff<-(pEncoder->siNumber)/2)
	{
		pEncoder->siDiff += pEncoder->siNumber;
	}
	pEncoder->siSumValue +=pEncoder->siDiff;
	if(pEncoder->state == 1)//����������
	{
		pEncoder->siSumValue = 0;
		pEncoder->state = 0;
	}
}



//����
void SendSwitchValue(uint8_t pusSwitch)
{
    CanTxMsg TxMessage;  // ���巢����Ϣ�Ľṹ��

    // ����CAN��Ϣ�ı�׼ID����Ϣ����
    TxMessage.StdId = CANRXVALVEID;  // ������ϢID�����Ը���ʵ������޸�
    TxMessage.RTR = CAN_RTR_DATA;    // ����֡
    TxMessage.IDE = CAN_ID_STD;     // ��׼ID
    TxMessage.DLC = 1;              // ���ݳ��ȣ�2�ֽڣ�16λ��

    // ��pusSwitch��ֵ���������
    TxMessage.Data[0] = pusSwitch;       // ���ֽ�

    // ����CAN��Ϣ
   CANX_SendStdData(&hcan1, CANRXVALVEID, TxMessage.Data, 1);
	
}

void Cacul_w(ST_MOTOR_H* motor)
{
	motor->anglew=motor->anglev-motor->u_anglev;
	motor->u_anglev=motor->anglev;
}
