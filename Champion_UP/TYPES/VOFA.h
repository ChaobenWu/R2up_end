#include "types.h"
#include "usart.h"

#define CH_COUNT 25		//ͨ���� ���������ݵĸ���
#pragma pack(push, 1) // ǿ��1�ֽڶ���
typedef struct
{
    float ch_data[CH_COUNT];
    unsigned char tail[4];
}VOFA_DATA;
#pragma pack(pop) // �ָ�Ĭ�϶���

	
extern VOFA_DATA vofa_data;
void VOFA_transmit_data(float *data, uint8_t num);


