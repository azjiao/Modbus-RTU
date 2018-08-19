#ifndef __KEY_H
#define __KEY_H
#include "template.h"

//定义KEY0、KEY1、Wk_UP三个宏为相应key的输入真值。
//此为真值，KEY0和KEY1按下时值为0，WKUP按下时值为1.
#define KEY0   GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4)
#define KEY1   GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3)
#define WKUP  GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)

//定义三个键按下时的独立编码值.可以用于按下释放检测也可以用于键编码值检测。
//注意：键的真值由KEY0/KEY1/WKUP三个宏来定义.
#define KEY0_CODE   !KEY0?  0x01: 0x00
#define KEY1_CODE   !KEY1?  0x02: 0x00
#define WKUP_CODE   WKUP?   0x04: 0x00

//按键防抖动滤波常数：ms
#define KEYFILTIME   150U

//以下是使用键盘扫描功能keypadScan(非单独按键功能)所需键值定义。
//必须和genKeypadVal()所产生的键值一致。
#define KP_KEY0    0x0001
#define KP_KEY1    0x0002
#define KP_WKUP    0x0004

//根据项目需要定义所需按键组合，如果是单个按键就不必另外定义了。
//组合键如果有顺序要求在用于时必须使用Keypad.u16ValOnce而不是Keypad.u16ValSus来判断.
//KEY0-WKUP组合，且后按下WKUP。
#define KEY0_WKUP    (KP_KEY0 + KP_WKUP) && (Keypad.u16DownTrg == KP_WKUP)
//KEY1-KEY0组合，且后按下KEY0.
#define KEY1_KEY0    (KP_KEY1 + KP_KEY0) && (Keypad.u16DownTrg == KP_KEY0)
//KEY0-KEY1组合，无顺序要求。
#define KEY0_KEY1    (KP_KEY0 + KP_KEY1)

//键盘结构
typedef struct
{
    uint16_t u16SusTime;  //组合连续按键的间隔时间，超过此时间认为不是组合。此时间应大于防抖时间。
    uint16_t u16ValSus; //当前键盘状态值(连续状态值)；
    uint16_t u16ValOnce; //不一样时的键盘状态值(连续按下同一组键只产生一个值)；
    uint16_t u16DownTrg;  //键盘按下突变键
    uint16_t u16UpTrg;  //键盘释放突变键

}KeypadType;
extern KeypadType Keypad;

//板载Key初始化。
void key_Init(void);

//Key0扫描程序。
//bSusKey==TRUE:连续按下触发多次。
//bSusKey==FALSE:连续按下当作一次按下处理。
bool key0_Scan( bool bSusKey );
//Key1扫描程序。
bool key1_Scan( bool bSusKey );
//WK_UP扫描程序。
bool WKUP_Scan( bool bSusKey );

//生成键盘组合状态值，当key0按下时KEYPAD第0位为1，当key1按下时KEYPAD第1位为1，同理第3位是WKUP的。
uint16_t genKeypadVal(void);

//键盘按键状态检测。
//替代单独按键检测函数，可以检测多个按键或单个按键。
//具有按键防抖功能。
uint16_t keypadScan(void);

#endif /* ifndef __KEY_H */
