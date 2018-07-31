#ifndef __KEY_H
#define __KEY_H
#include "template.h"

//定义KEY0、KEY1、Wk_UP三个宏为相应key的输入真值。
//此为真值，KEY0和KEY1按下时值为0，WKUP按下时值为1.
#define KEY0   GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4)
#define KEY1   GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3)
#define WKUP   GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)

//定义三个键按下时的编码值.可以用于按下释放检测也可以用于键编码值检测。
//注意：键的真值由KEY0/KEY1/WKUP三个宏来定义.
#define KEY0_CODE   !KEY0?  0xf0: 0x00
#define KEY1_CODE   !KEY1?  0xf1: 0x00
#define WKUP_CODE   WKUP?   0xf2: 0x00

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

#endif /* ifndef __KEY_H */
