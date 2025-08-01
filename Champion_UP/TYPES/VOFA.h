#include "types.h"
#include "usart.h"

#define CH_COUNT 25		//通道数 即发送数据的个数
#pragma pack(push, 1) // 强制1字节对齐
typedef struct
{
    float ch_data[CH_COUNT];
    unsigned char tail[4];
}VOFA_DATA;
#pragma pack(pop) // 恢复默认对齐

	
extern VOFA_DATA vofa_data;
void VOFA_transmit_data(float *data, uint8_t num);


