/*********************************************************
  功能： 串口Modbus-RTU通讯试验
  描述： 精英版485接口是USART2口，使用串口2进行Modbus-RTU试验。
  设计： azjiao
  版本： 0.1
  日期： 2018年07月18日
 *********************************************************/

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stm32f10x.h>
#include "template.h"
#include "key.h"
#include "led.h"
#include "beep.h"
#include "delay.h"
#include "extix.h"
#include "bitBand.h"
#include "usart1.h"
#include "baseTime.h"
#include "modRTU.h"
#include "modMaster.h"
#include "modSlave.h"

void assert_failed(uint8_t* file, uint32_t line)
{
    //printf("Param Error! in file name: xxx, in line %d\n",line);
    //while(1);
}



int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    led_Init();
    key_Init();
    delay_Init(); //采用查询方式延时，应在文件delay.c中注释掉#define APP1MSPLUS宏定义。
    Usart1_Init(19200);
    //Modbus通讯初始化.
    Modbus_Init();
    ModRTU_Buffer Buffer_Temp; //数据暂存，用于比较。
   
 /*  
    while(1)
    {               
        if(key0_Scan(TRUE))
  
   //  作为主站
        {
        //读取HoldReg F0x03
        Modbus_Master(3, FALSE, 40001, 5, Data_Struct.Buffer); 
        
        //把读回的4xxxx值反送入从站。F0x010
        if(Modbus_Status_Struct.bDone)
        {
            //写HoldReg F0x10
            Modbus_Master(3, TRUE, 40021, 5, Data_Struct.Buffer);
            
        }
        
        //读取AI 3xxxx F0x04
        Modbus_Master(3, FALSE, 30000, 5, Data_Struct.Buffer);
        //把读取的3xxxx值反送入4xxxx。F0x10
        if(Modbus_Status_Struct.bDone)
            //写HoldReg F0x10
            Modbus_Master(3, TRUE, 40011, 5, Data_Struct.Buffer);
        
        //读取1xxxx:DI:F0x02
        Modbus_Master(3, FALSE, 10000, 5, Data_Struct.Buffer);
        if(Modbus_Status_Struct.bDone)
        {
            //暂存读取的DI数据。结构赋值是传值，不是传址。
            Buffer_Temp = Data_Struct;
            //把读取的1xxxxDI值反送入0xxxx.F0x0F
            //元件基址0xxxx不能使用0开头，0开头为8进制。
            Modbus_Master(3, TRUE, 10, 5, Data_Struct.Buffer);
        }
        
        
        //读取0xxxx DQ:F0x01,和DI 1xxxx 值比较。
        Modbus_Master(3, FALSE, 10, 5, Data_Struct.Buffer);        
            
        //5个DQ，只有1个字节数据。无法直接比较数组，只能使用库函数或for。
        if(Buffer_Temp.Buffer[0] != Data_Struct.Buffer[0])
        {
            printf("DQ返回数据和强制的不一致！\n");
        }
        
       //delay_ms(300);   

    } 

    
    }
*/
    ReceiveFrame(&RX_Struct);  
 
    while(1)
    {

        
    //if(!KEY0)
    if(!KEY0)  //由于采用扫描方式不是中断，可能会由于通讯耗时出现错过防抖到的时刻。
        DI_1xxxx.u8Data[0] |= 0x10;
    else
        DI_1xxxx.u8Data[0] &= 0xEF;
        
    if(!KEY1)
        DI_1xxxx.u8Data[0] |= 0x20;
    else
        DI_1xxxx.u8Data[0] &= 0xDF;
    if(WKUP)
        DI_1xxxx.u8Data[0] |= 0x40;
    else
        DI_1xxxx.u8Data[0] &= 0xBF;

        //AI
        AI_3xxxx.u16Data[0] = 0x1234;
        AI_3xxxx.u16Data[1] = 0x5678;
        AI_3xxxx.u16Data[2] = 0x9ABC;
        AI_3xxxx.u16Data[3] = 0xDEF0;
        AI_3xxxx.u16Data[4] = 0xEFCD;
        AI_3xxxx.u16Data[5] = 0xF234;
        Modbus_Slave();

       
    }

}





