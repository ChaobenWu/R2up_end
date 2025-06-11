#include "types.h"
#include "pid.h"
//***********************************************����PID�汾
//PID������ʼ��
void PID_Init(ST_PID *pid, float p, float i, float d, float maxErr, float maxI, float maxO)
{
    pid->fpKp = p;
    pid->fpKi = i;
    pid->fpKd = d;
    pid->fpEMax = maxErr;
    pid->fpIMax = maxI;
    pid->fpOMax = maxO;
}


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
    if (pstPid->fpSumE > pstPid->fpIMax)
    {
        pstPid->fpSumE = pstPid->fpIMax;
    }
    else if (pstPid->fpSumE < -pstPid->fpIMax)
    {
        pstPid->fpSumE = -pstPid->fpIMax;
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
