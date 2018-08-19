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
#include "stdio.h"

//定义键盘结构变量。
KeypadType Keypad;

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

//生成键盘组合状态值，当key0按下时u16Keypad第0位为1，当key1按下时u16keypad第1位为1，同理第2位是WKUP的。
uint16_t genKeypadVal(void)
{
    uint16_t u16Keypad = 0;
    if(KEY0_CODE)
        u16Keypad |= 0x0001;
    if(KEY1_CODE)
        u16Keypad |= 0x0002;
    if(WKUP_CODE)
        u16Keypad |= 0x0004;

    return u16Keypad;
}

//键盘按键状态扫描。
//替代单独按键检测函数，可以检测多个按键或单个按键。
//具有按键防抖功能。
//支持连续按下同一组按键产生连续多个同一个键盘状态值，存放在Keypad.u16ValSus中。
//支持连续按下同一组按键只产生一个键盘状态值，存放在Keypad.u16ValOnce中。
//函数返回值为Keypad.u16DownTrg，是按下突变键,供简单独立按键使用。
uint16_t keypadScan(void)
{
    static uint16_t u16KeypadRTTmp = 0x0000; //上一次实时键盘状态暂存值。
    uint16_t u16KeypadRT;  //本次实时按键状态值。
    uint16_t u16KeypadRTTrg;   //键实时发生沿变化。
    
    static uint16_t u16KeypadStabValTemp = 0x0000; //上一次检出的键盘状态值。
    uint16_t u16KeypadStabTrg;  //键检出发生按下或释放标识：检测到沿变化。
        
    //防抖动检测定时器。
    static TimerType timer;

    //检测键盘状态值
    u16KeypadRT = genKeypadVal();
    
    //防抖处理：当前后两次检测到的键盘状态值相同时才会使定时器继续工作。
    //并持续150ms后检出。
    //当按键两次检测不一样时检出值为上一次的检出值. 
    //这样，当键按下和释放时会有150ms的延时。
    u16KeypadRTTrg = u16KeypadRT ^ u16KeypadRTTmp;
    TimeON(!u16KeypadRTTrg, KEYFILTIME, &timer);
    u16KeypadRTTmp = u16KeypadRT;
    if(timer.bQ)
    {
        Keypad.u16ValSus = u16KeypadRT;        
    }
    else        
        Keypad.u16ValSus = u16KeypadStabValTemp;    
    
    //沿检测：发生了突变。
    u16KeypadStabTrg = (Keypad.u16ValSus ^ u16KeypadStabValTemp); 
    //按下突变键检测。    
    Keypad.u16DownTrg = Keypad.u16ValSus & u16KeypadStabTrg;
    //释放突变键检测。
    Keypad.u16UpTrg = ~Keypad.u16ValSus & u16KeypadStabTrg;
    //只触发一次检测,检出和上次不一样的状态值。用来实现连续按下同一组键时只能检出一个状态值。。
    if(u16KeypadStabTrg)
        Keypad.u16ValOnce = Keypad.u16ValSus;
    else 
        Keypad.u16ValOnce = 0;
    
    u16KeypadStabValTemp = Keypad.u16ValSus;    
    
    return Keypad.u16DownTrg;
}
