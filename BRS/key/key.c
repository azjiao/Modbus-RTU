/*
 * 开发板板载资源：按键key.
 * KEY0:接到PE4端口，上拉输入；
 * KEY1:接到PE3端口，上拉输入；
 * RESET：接到NRST，TODO...
 * WK_UP:接到PA0端口，下拉输入；
 */
#include "stm32f10x.h"
#include "key.h"
#include "delay.h"

//板载Key初始化。
void key_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct; //声明初始化数据结构。

    //GPIOA和GPIOE时钟使能。
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);

    //KEY0和KEY1设置相同。
    GPIO_InitStruct.GPIO_Pin = (GPIO_Pin_4 | GPIO_Pin_3);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU; //上拉输入模式。
    GPIO_Init(GPIOE, &GPIO_InitStruct);

    //WK_UP设置。
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD; //下拉输入模式。
    GPIO_Init(GPIOA, &GPIO_InitStruct);

}

//key0 扫描程序
//bSusKey==TRUE：连续按下可以扫描到连续多个值。
//bSusKey==FALSE：连续按下只扫描到一个按键值。
bool key0_Scan( bool bSusKey )
{
    static bool bsLastKey = FALSE;  //按键上次状态。
    //经过防抖处理后检测到按键压下。
    bool bKey;
    //防抖动检测定时器。
    static TimerType timer;
    //返回值。
    bool bRet = FALSE;

    assert_param(Is_BOOL(bSusKey));

    // 防抖处理。
    bKey = TimeON(KEY0_CODE, 150U, &timer);
    bRet = bKey & (bKey ^ bsLastKey);  //沿检测。上下沿均为TRUE，非沿时为FALSE.
    //如果是连续按键模式。
    if(bSusKey)
        bsLastKey = FALSE;  //当是连续模式时，把上次按键状态置为FALSE。
    //如果是单按键模式。
    else
        bsLastKey = bKey;


    return bRet;
}

//key1 扫描程序
//bSusKey==TRUE：连续按下可以扫描到连续多个值。
//bSusKey==FALSE：连续按下只扫描到一个按键值。
bool key1_Scan( bool bSusKey )
{
    static bool bsLastKey = FALSE;  //按键上次状态。
    //经过防抖处理后检测到按键压下。
    bool bKey;
    //防抖动检测定时器。
    static TimerType timer;
    //返回值。
    bool bRet = FALSE;

    assert_param(Is_BOOL(bSusKey));

    // 防抖处理。
    bKey = TimeON(KEY1_CODE, 150U, &timer);
    bRet = bKey & (bKey ^ bsLastKey);  //沿检测。上下沿均为TRUE，非沿时为FALSE.
    //如果是连续按键模式。
    if(bSusKey)
        bsLastKey = FALSE;  //当是连续模式时，把上次按键状态置为FALSE。
    //如果是单按键模式。
    else
        bsLastKey = bKey;

    return bRet;
}


//WKUP 扫描程序
//bSusKey==TRUE：连续按下可以扫描到连续多个值。
//bSusKey==FALSE：连续按下只扫描到一个按键值。
bool WKUP_Scan( bool bSusKey )
{
    static bool bsLastKey = FALSE;  //按键上次状态。
    //经过防抖处理后检测到按键压下。
    bool bKey;
    //防抖动检测定时器。
    static TimerType timer;
    //返回值。
    bool bRet = FALSE;

    assert_param(Is_BOOL(bSusKey));

    // 防抖处理。
    bKey = TimeON(WKUP_CODE, 150U, &timer);
    bRet = bKey & (bKey ^ bsLastKey);  //沿检测。上下沿均为TRUE，非沿时为FALSE.
    //如果是连续按键模式。
    if(bSusKey)
        bsLastKey = FALSE;  //当是连续模式时，把上次按键状态置为FALSE。
    //如果是单按键模式。
    else
        bsLastKey = bKey;

    return bRet;
}

