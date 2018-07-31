#include <stm32f10x.h>
#include <core_cm3.h>
#include "delay.h"

#define APP1MSPLUS
#ifdef  APP1MSPLUS
// 如果采用1ms脉冲方式，则以500us设置SysTick.
void delay_Init(void)
{
    // SysTick配置，等待完成。
    if (SysTick_Config(RELOAD_500US))
    {
        while(1);
    }
}

// SysTick定时器中断
void SysTick_Handler(void)
{
    // 500us定时中断到，使Timer.bPlus_ms翻转一次。
    Timer.bPlus_ms = !Timer.bPlus_ms;
}

#else
// 如果采用1ms查询方式，则以1ms设置SysTick。
// 采用SysTick的定时器初始化
void delay_Init(void)
{
    // SysTick配置，等待完成。
    if (SysTick_Config(RELOAD_1MS))
    {
        while(1);
    }
}

// SysTick定时中断
void SysTick_Handler(void)
{
    // 1ms定时到，使Timer.uTimer_ms减1.
    // 使用前必须使Timer.uTimer_ms赋值为需定时的ms倍数。
    // Timer不能重复使用，只能在一个地方使用。
    if( Timer.uTimer_ms != 0U )
    {
        Timer.uTimer_ms--;
    }
}
#endif

// ms级delay.
void delay_ms(u32 utime_ms)
{

    Timer.uTimer_ms = utime_ms;
    // 查询延时是否到，不到则继续查询。
    while( Timer.uTimer_ms != 0U )
        ;
}

// s级delay.
void delay_s(u32 utime_s)
{
    // 延时时间为utime_s倍的1000ms。
    for (int i = 0; i < utime_s; ++i)
    {
        delay_ms(1000U);
    }
}

// 延时接通定时器
// 当bEnb为TRUE时开始定时，定时单位为1ms。
// 当bEnb为FALSE时复位定时器。
// 当定时到达后如果没有复位定时器则定时器当前计数值uEt保持不变。
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

