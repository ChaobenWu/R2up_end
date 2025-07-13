#include "types.h"
#include "pid.h"
//***********************************************����PID�汾


void PID_Calc(ST_PID *pstPid)
{
    // ���㱾��ƫ��
    pstPid->fpE = pstPid->fpDes - pstPid->fpFB;

    // ��������������Χ�ڣ��������Ϊ0
    if (fabs(pstPid->fpE) <= pstPid->fpEMin)
    {
        pstPid->fpE = 0.0f;
    }

    // ���ַ��룺��ƫ������趨�����ƫ��ֵʱ�������л�������
    if (fabs(pstPid->fpE) < pstPid->fpEMax)
    {
        pstPid->fpSumE += pstPid->fpE; // �ۻ�ƫ��
    }

    // �����޷�
    if (pstPid->fpSumE > pstPid->fpSumEMax)
    {
        pstPid->fpSumE = pstPid->fpSumEMax;
    }
    else if (pstPid->fpSumE < -pstPid->fpSumEMax)
    {
        pstPid->fpSumE = -pstPid->fpSumEMax;
    }
    // ���������
    pstPid->fpUKp = pstPid->fpKp * pstPid->fpE;

    // ��������ע��Ki�Ѿ������ۻ�ƫ���ϣ�
    pstPid->fpUKi = pstPid->fpKi * pstPid->fpSumE;
		
    // ΢�����У����ⷴ��ֵ��������΢�����Ӱ��
    float diffE = (pstPid->fpE - pstPid->fpPreE);
    pstPid->fpUKd = pstPid->fpKd * diffE;

    // ���±���PID������
    pstPid->fpU = pstPid->fpUKp + pstPid->fpUKi + pstPid->fpUKd;

    // ����޷�
    if (pstPid->fpU > pstPid->fpOMax)
    {
        pstPid->fpU = pstPid->fpOMax;
    }
    else if (pstPid->fpU < -pstPid->fpOMax)
    {
        pstPid->fpU = -pstPid->fpOMax;
    }

    // �����ϴ�ƫ����ϴ�΢�������
    pstPid->fpPreE = pstPid->fpE;
    pstPid->fpUKdpre = pstPid->fpUKd;
}


void PID_Calc_DualLoop(ST_CASCADE_PID *pid)
{
    // �����⻷PID
    PID_Calc(&pid->outer);

    // ���⻷�������Ϊ�ڻ���Ŀ��ֵ
    pid->inner.fpDes = pid->outer.fpU;

    // �����ڻ�PID
    PID_Calc(&pid->inner);
    
    // ����������ڻ���PID������
    pid->output = pid->inner.fpU;
}

void CalTD(ST_TD *pStTD)
{
    float d, d0, y, a0, a = 0, fhan;
    pStTD->x = pStTD->x1 - pStTD->aim;
    // ���㵱ǰλ����Ŀ��λ�õ����

    d = pStTD->r * pStTD->h; // ����������� d
    d0 = pStTD->h * d;       // ʱ������� d0
    y = pStTD->x + pStTD->h * pStTD->x2;
    // ��ϵ�ǰλ�������ٶ����

    a0 = sqrt(d * d + 8 * pStTD->r * fabs(y));
    // ������з��������Ե��м��� a0

    if (fabs(y) > d0)
        a = pStTD->x2 + (a0 - d) * Sgn(y) / 2;
    // ���ϴ�ʱ�����÷����Ե���

    else
        a = pStTD->x2 + y / pStTD->h;
    // ����Сʱ���������Ե���

    if (fabs(a) > d)
        fhan = -1 * pStTD->r * Sgn(a);
    else
        fhan = -1 * pStTD->r * a / d;
    // �����ٶȸ��µķ���ͷ���

    pStTD->x1 += pStTD->T * pStTD->x2;
    // ����λ��

    pStTD->x2 += pStTD->T * fhan;
    // �����ٶ�
}



float plan_of_time(float start_pos, float end_pos, float time)
{
    float current_pos;
    float k =0;
    if(time >= 0.0f && time <= 1.0f) k = 6*time*time*time*time*time -15*time*time*time*time + 10*time*time*time;
    else if( time > 1.0f ) k = 1;
    current_pos = start_pos + k*(end_pos - start_pos);
    return current_pos;
}


float TD_END(ST_TD *pStTD,float end_pos)
{
	pStTD->aim=end_pos;
	CalTD(pStTD);
	return pStTD->x1;
}




/*******************************************************************
�������ƣ�CalSMC(ST_SMC *pStSMC)
�������ܣ���ģ�����㷨
��    ע��
********************************************************************/
void CalSMC(ST_SMC *pStSMC)
{
         pStSMC->TD.aim = pStSMC->fpDes;        
         CalTD(&pStSMC->TD);
         pStSMC->fpE = pStSMC->TD.x1 - pStSMC->fpFB;
				 pStSMC->fpU = 1 / pStSMC->b * (pStSMC->TD.x2+ pStSMC->eps * SMC_SatFunc(pStSMC->fpE, pStSMC->dead)+ pStSMC->gain * pStSMC->fpE);
				pStSMC->fpU = Clip(pStSMC->fpU, -pStSMC->fpUMax, pStSMC->fpUMax);
}
float SMC_SatFunc(float in, float d)
{
                if(fabs(in) >= d)
                        return Sign_Judge(in);
                else
                        return in / d;
}
//���ｫ���ź���sgn()�����˱��ͺ���sat()��Ŀ���Ƿ�ֹ����
/*******************************************************************
�������ƣ�Sign_Judge(FP32 fp_Any_Number)
�������ܣ��ж�����
��    ע������ֵΪ1��-1�����ı����ķ���
********************************************************************/
int Sign_Judge(FP32 fp_Judge_Number)
{
        if(fp_Judge_Number >= 0)
        {
                return 1;
        }
        else 
        {
                return -1;
        }
}
float Clip(float in,float min ,float max )
{
	if(in>max)
		in=max;
	if(in<min)
		in=min;
	return in;
}



//void Pi_Caculate(float *p,float *i,float v)
//{
//	if(v>250&&v<=260)
//	{
//		p=0;
//		i=0;
//	}
//}


void PID_Calc_Pitch(ST_PID *pstPid)
{
    // ���㱾��ƫ��
    pstPid->fpE = pstPid->fpDes - pstPid->fpFB;

    // ��������������Χ�ڣ��������Ϊ0
    if (fabs(pstPid->fpE) <= pstPid->fpEMin)
    {
        pstPid->fpE = 0.0f;
    }

    // ���ַ��룺��ƫ������趨�����ƫ��ֵʱ�������л�������
    if (fabs(pstPid->fpE) < pstPid->fpEMax)
    {
        pstPid->fpSumE += pstPid->fpE; // �ۻ�ƫ��
    }

    pstPid->fpSumE *= 0.99f;
    // �����޷�
    if (pstPid->fpSumE > pstPid->fpSumEMax)
    {
        pstPid->fpSumE = pstPid->fpSumEMax;
    }
    else if (pstPid->fpSumE < -pstPid->fpSumEMax)
    {
        pstPid->fpSumE = -pstPid->fpSumEMax;
    }
    // ���������
    pstPid->fpUKp = pstPid->fpKp * pstPid->fpE;

    // ��������ע��Ki�Ѿ������ۻ�ƫ���ϣ�
    pstPid->fpUKi = pstPid->fpKi * pstPid->fpSumE;
		
    // ΢�����У����ⷴ��ֵ��������΢�����Ӱ��
    float diffE = (pstPid->fpE - pstPid->fpPreE);
    pstPid->fpUKd = pstPid->fpKd * diffE;

    // ���±���PID������
    pstPid->fpU = pstPid->fpUKp + pstPid->fpUKi + pstPid->fpUKd;

    // ����޷�
    if (pstPid->fpU > pstPid->fpOMax)
    {
        pstPid->fpU = pstPid->fpOMax;
    }
    else if (pstPid->fpU < -pstPid->fpOMax)
    {
        pstPid->fpU = -pstPid->fpOMax;
    }

    // �����ϴ�ƫ����ϴ�΢�������
    pstPid->fpPreE = pstPid->fpE;
    pstPid->fpUKdpre = pstPid->fpUKd;
}
