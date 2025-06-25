#include "VOFA.h"

VOFA_DATA vofa_data;

const uint8_t FRAME_TAIL[4] = {0x00, 0x00, 0x80, 0x7F}; // �̶���ֹ��

//ʹ��justfloatЭ�齫���ݷ�����vofa+��λ��
void VOFA_transmit_data(float *data, uint8_t num)
{
	memcpy(vofa_data.ch_data, data, num * sizeof(float));
	
	
	// ��ȷ���ʣ��ͨ��Ϊ0
    for(int i=num; i<CH_COUNT; i++) 
	{
        vofa_data.ch_data[i] = 0.0f; 
    }
	
	memcpy(vofa_data.tail, FRAME_TAIL, sizeof(FRAME_TAIL));
	
	
	HAL_UART_Transmit_DMA(&huart2,(uint8_t*)&vofa_data,sizeof(VOFA_DATA));//����
	
}
	

	