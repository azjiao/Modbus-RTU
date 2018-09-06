/*
 * 开发版板载资源：led.
 * LED0: 接在PB5端口，开漏输出。
 * LED1: 接在PE5端口，开漏输出。
 *
 *
 */
#include "stm32f10x.h"
#include "dev.h"


//板载led初始化。
void led_Init(void)
{   
    //GPIOB和GPIOE时钟使能。
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

    //设置LED0。
    My_GPIO_Init(GPIOB, GPIO_Pin_5, GPIO_Mode_Out_OD, GPIO_Speed_10MHz);//开漏输出

    //设置LED1.除了端口组不一样外，其他一样。
    My_GPIO_Init(GPIOE, GPIO_Pin_5, GPIO_Mode_Out_OD, GPIO_Speed_10MHz);//开漏输出
	
	//初始化使led等熄灭。
	//如果不处理则led相应端口初始化输出为0，灯被点亮。
	LED0_OFF;
	LED1_OFF;
}

