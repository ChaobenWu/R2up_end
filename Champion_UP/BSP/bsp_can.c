#include "bsp_can.h"
#include "can.h"
#include "types.h"
uint8_t aaabaa;
//CAN初始化函数
void can1_start(void) {
    CAN1_FILTER_CONFIG(&hcan1); //滤波器初始化
    HAL_CAN_Start(&hcan1); //启动CAN1
    HAL_CAN_ActivateNotification(&hcan1,CAN_IT_RX_FIFO0_MSG_PENDING); //使能中断
}

/**
 * @brief 配过滤器
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
    CAN2_FILTER_CONFIG(&hcan2); //滤波器初始化
    HAL_CAN_Start(&hcan2); //启动CAN2
    HAL_CAN_ActivateNotification(&hcan2,CAN_IT_RX_FIFO0_MSG_PENDING); //使能中断
}

/**
 * @brief 配过滤器
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
 * @brief 发送标准ID的消息数据
 * @param hcan CAN句柄
 * @param ID 数据帧ID
 * @param pData 数组指针
 * @param Len 字节数(8~8), 可选择性填0x08
 */
void CANX_SendStdData(CAN_HandleTypeDef* hcan, uint16_t ID, uint8_t *pData, uint16_t Len)
{
    static CAN_TxHeaderTypeDef Tx_Header; // 定义静态的Tx Header结构体变量

    Tx_Header.StdId = ID; // 将ID赋值给标准ID域
    Tx_Header.ExtId = CAN_ID_STD; // 设置扩展标识符为标准ID
    Tx_Header.IDE = CAN_RTR_DATA; // 设置IDE为数据帧
    Tx_Header.RTR = 0; // RTR设置为0，表示数据帧
    Tx_Header.DLC = Len; // DLC设置为长度

    // 找空的发送邮箱，将数据发送出去
    if (HAL_CAN_AddTxMessage(hcan, &Tx_Header, pData, (uint32_t*)CAN_TX_MAILBOX0) != HAL_OK) 
    {
        if(HAL_CAN_AddTxMessage(hcan, &Tx_Header, pData, (uint32_t*)CAN_TX_MAILBOX1)!= HAL_OK)
				{
					HAL_CAN_AddTxMessage(hcan, &Tx_Header, pData, (uint32_t*)CAN_TX_MAILBOX2);
				}
    }
}
/**
 * @brief DIJ系列电机电流发送函数
 * @param hcan CAN句柄
 * @param id 电机ID
 * @param current1 电流1
 * @param current2 电流2
 * @param current3 电流3
 * @param current4 电流4
 * @note
 * @author HITCRT
 */
void CAN_SendCurrent(CAN_HandleTypeDef *hcan, uint32_t id,int16_t current1,int16_t current2,int16_t current3,int16_t current4)
{
    uint8_t TxCurrent[8]; // 定义一个8字节的数组用来存放电流数据

    TxCurrent[0] = (current1>>8); // 将电流1的高位放入数组的第一个元素
    TxCurrent[1] = current1; // 将电流1的低位放入数组的第二个元素
    TxCurrent[2] = (current2>>8); // 将电流2的高位放入数组的第三个元素
    TxCurrent[3] = current2; // 将电流2的低位放入数组的第四个元素
    TxCurrent[4] = (current3>>8); // 将电流3的高位放入数组的第五个元素
    TxCurrent[5] = current3; // 将电流3的低位放入数组的第六个元素
    TxCurrent[6] = (current4>>8); // 将电流4的高位放入数组的第七个元素
    TxCurrent[7] = current4; // 将电流4的低位放入数组的第八个元素

    CANX_SendStdData(hcan,id,TxCurrent,8); // 调用CANX_SendStdData函数发送数据
}
//***********************************************************************************************//
//接下来是哥们的can接受函数

//***********************************************************************************************//


uint8_t RxMsg_CAN1[8]; // 定义CAN1通信接收缓冲区
uint8_t RxMsg_CAN2[8]; // 定义CAN2通信接收缓冲区

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
		if(hcan->Instance == CAN1) // 判断是否为CAN1接口
		{
        CAN_RxHeaderTypeDef RxHeader;
        HAL_CAN_GetRxMessage(hcan,CAN_RX_FIFO0,&RxHeader,RxMsg_CAN1); // 获取接收到的消息头信息及数据

        switch(RxHeader.StdId) // 根据接收到的标准ID进行处理，得到
        {

					  case 0x310: // 电推杆
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
//					  case 0x30: // 行程开关
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
						case 0x201://下航模
							if(motor_shot_down.state==1)
							{		
								memcpy(&motor_shot_down.diff_angle, &RxMsg_CAN1, 4);  // 位置
								motor_shot_down.state=0;
							}
							else
							{
								memcpy(&motor_shot_down.mech_angle, &RxMsg_CAN1, 4);  // 位置
								motor_shot_down.angle=motor_shot_down.mech_angle-motor_shot_down.diff_angle;
							}
							memcpy(&motor_shot_down.mech_anglev, &RxMsg_CAN1[4], 4);  // 速度
							shot_down_filter.in=motor_shot_down.mech_anglev;
							LpFilter(&shot_down_filter);
							motor_shot_down.anglev=shot_down_filter.out;
							Cacul_w(&motor_shot_down);
							system_monitor.rate_cnt.motor_shot_down++;
							break;
						case 0x202://上航模
							if(motor_shot_up.state==1)
							{		
								memcpy(&motor_shot_up.diff_angle, &RxMsg_CAN1, 4);  // 位置
								motor_shot_up.state=0;
							}
							else
							{
								memcpy(&motor_shot_up.mech_angle, &RxMsg_CAN1, 4);  // 位置
								motor_shot_up.angle=motor_shot_up.mech_angle-motor_shot_up.diff_angle;
							}
							memcpy(&motor_shot_up.mech_anglev, &RxMsg_CAN1[4], 4);  // 速度
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
		if(hcan->Instance == CAN2) // 判断是否为CAN2接口
		{
        CAN_RxHeaderTypeDef RxHeader2;
        HAL_CAN_GetRxMessage(hcan,CAN_RX_FIFO0,&RxHeader2,RxMsg_CAN2); // 获取接收到的消息头信息及数据

        switch(RxHeader2.StdId) // 根据接收到的标准ID进行处理，得到
        {	
            case 0x201: // 2006电机
                motor_rotate.EncoderNum = GetEncoderNumber(&motor_rotate,RxMsg_CAN2);
                motor_rotate.encoder_speed = GetSpeed(&RxHeader2,RxMsg_CAN2);
                Abs_Encoder_Process(&motor_rotate.motor_encoder,motor_rotate.EncoderNum);
                motor_rotate.angle=motor_rotate.motor_encoder.siSumValue/(float)M3508_siNumber*360.f/M2006_uiGearRatio; // 角度计算
                motor_rotate.anglev=motor_rotate.encoder_speed/(float)M3508_uiGearRatio; // 角速度计算
								fb_rotate=motor_rotate.angle;
								system_monitor.rate_cnt.motor_rotate++;
                break;					
            case 0x202 :// 3508电机
                motor_bounce_right.EncoderNum = GetEncoderNumber(&motor_bounce_right,RxMsg_CAN2);
                motor_bounce_right.encoder_speed = GetSpeed(&RxHeader2,RxMsg_CAN2);
                Abs_Encoder_Process(&motor_bounce_right.motor_encoder,motor_bounce_right.EncoderNum);
                motor_bounce_right.angle=motor_bounce_right.motor_encoder.siSumValue/(float)M3508_siNumber*360.f; // 角度计算
                motor_bounce_right.anglev=motor_bounce_right.encoder_speed; // 角速度计算
								fbv_bounce_right=motor_bounce_right.anglev;
								system_monitor.rate_cnt.motor_bounce_right++;
                break;	
            case 0x203: // 3508电机
                motor_bounce_left.EncoderNum = GetEncoderNumber(&motor_bounce_left,RxMsg_CAN2);
                motor_bounce_left.encoder_speed = GetSpeed(&RxHeader2,RxMsg_CAN2);
                Abs_Encoder_Process(&motor_bounce_left.motor_encoder,motor_bounce_left.EncoderNum);
                motor_bounce_left.angle=motor_bounce_left.motor_encoder.siSumValue/(float)M3508_siNumber*360.f; // 角度计算
                motor_bounce_left.anglev=motor_bounce_left.encoder_speed; // 角速度计算
								fbv_bounce_left=motor_bounce_left.anglev;
								system_monitor.rate_cnt.motor_bounce_left++;	
								break;	
            default:
                break;
        }
    }
}
// 函数名称：GetEncoderNumber
// 功能：从CAN消息中提取编码器数值
// 参数：motor 指向MOTOR类型的指针，msg 接收的CAN消息
int16_t GetEncoderNumber(ST_MOTOR* motor,uint8_t msg[8])
{
    motor->EncoderNum = (msg[0]<<8 | msg[1]);
    return motor->EncoderNum;
}

// 函数名称：GetSpeed
// 功能：从CAN消息中提取速度信息
// 参数：pCanRxMsg 指向CAN_RxHeaderTypeDef类型的指针，msg 接收的CAN消息
float GetSpeed(CAN_RxHeaderTypeDef* pCanRxMsg,uint8_t msg[8])
{
    int32_t speed_temp;
    int32_t base_value = 0xFFFFF;
    
    if(msg[2] & 0x01<<7) // 判断最高位是否为1
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
	if(pEncoder->state == 1)//在这里清零
	{
		pEncoder->siSumValue = 0;
		pEncoder->state = 0;
	}
}



//气缸
void SendSwitchValue(uint8_t pusSwitch)
{
    CanTxMsg TxMessage;  // 定义发送消息的结构体

    // 设置CAN消息的标准ID和消息类型
    TxMessage.StdId = CANRXVALVEID;  // 设置消息ID，可以根据实际情况修改
    TxMessage.RTR = CAN_RTR_DATA;    // 数据帧
    TxMessage.IDE = CAN_ID_STD;     // 标准ID
    TxMessage.DLC = 1;              // 数据长度：2字节（16位）

    // 将pusSwitch的值打包成数据
    TxMessage.Data[0] = pusSwitch;       // 低字节

    // 发送CAN消息
   CANX_SendStdData(&hcan1, CANRXVALVEID, TxMessage.Data, 1);
	
}

void Cacul_w(ST_MOTOR_H* motor)
{
	motor->anglew=motor->anglev-motor->u_anglev;
	motor->u_anglev=motor->anglev;
}
