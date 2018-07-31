#include <stm32f10x.h>
#include <core_cm3.h>
#include "delay.h"

#define APP1MSPLUS
#ifdef  APP1MSPLUS
// �������1ms���巽ʽ������500us����SysTick.
void delay_Init(void)
{
    // SysTick���ã��ȴ���ɡ�
    if (SysTick_Config(RELOAD_500US))
    {
        while(1);
    }
}

// SysTick��ʱ���ж�
void SysTick_Handler(void)
{
    // 500us��ʱ�жϵ���ʹTimer.bPlus_ms��תһ�Ρ�
    Timer.bPlus_ms = !Timer.bPlus_ms;
}

#else
// �������1ms��ѯ��ʽ������1ms����SysTick��
// ����SysTick�Ķ�ʱ����ʼ��
void delay_Init(void)
{
    // SysTick���ã��ȴ���ɡ�
    if (SysTick_Config(RELOAD_1MS))
    {
        while(1);
    }
}

// SysTick��ʱ�ж�
void SysTick_Handler(void)
{
    // 1ms��ʱ����ʹTimer.uTimer_ms��1.
    // ʹ��ǰ����ʹTimer.uTimer_ms��ֵΪ�趨ʱ��ms������
    // Timer�����ظ�ʹ�ã�ֻ����һ���ط�ʹ�á�
    if( Timer.uTimer_ms != 0U )
    {
        Timer.uTimer_ms--;
    }
}
#endif

// ms��delay.
void delay_ms(u32 utime_ms)
{

    Timer.uTimer_ms = utime_ms;
    // ��ѯ��ʱ�Ƿ񵽣������������ѯ��
    while( Timer.uTimer_ms != 0U )
        ;
}

// s��delay.
void delay_s(u32 utime_s)
{
    // ��ʱʱ��Ϊutime_s����1000ms��
    for (int i = 0; i < utime_s; ++i)
    {
        delay_ms(1000U);
    }
}

// ��ʱ��ͨ��ʱ��
// ��bEnbΪTRUEʱ��ʼ��ʱ����ʱ��λΪ1ms��
// ��bEnbΪFALSEʱ��λ��ʱ����
// ����ʱ��������û�и�λ��ʱ����ʱ����ǰ����ֵuEt���ֲ��䡣
bool TimeON(bool bEnb, u32 uPt, TimerType *timer)
{
    if(!bEnb){
        timer->uEt = 0U;
        timer->bTemp = FALSE;
        timer->bQ = FALSE;
        return FALSE;
    }
    else{
        if((timer->uEt < uPt) && (Timer.bPlus_ms) && (!timer->bTemp))
            timer->uEt = timer->uEt + 1;
        timer->bTemp = Timer.bPlus_ms;

        if((timer->uEt >= uPt)){
            timer->bQ = TRUE;
            return TRUE;
        }
        else{
            timer->bQ = FALSE;
            return FALSE;
        }
    }
}

