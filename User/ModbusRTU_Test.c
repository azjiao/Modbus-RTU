/*********************************************************
  ���ܣ� ����Modbus-RTUͨѶ����
  ������ ��Ӣ��485�ӿ���USART2�ڣ�ʹ�ô���2����Modbus-RTU���顣
  ��ƣ� azjiao
  �汾�� 0.1
  ���ڣ� 2018��07��18��
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
    delay_Init(); //���ò�ѯ��ʽ��ʱ��Ӧ���ļ�delay.c��ע�͵�#define APP1MSPLUS�궨�塣
    Usart1_Init(19200);
    //ModbusͨѶ��ʼ��.
    Modbus_Init();
    ModRTU_Buffer Buffer_Temp; //�����ݴ棬���ڱȽϡ�
   
 /*  
    while(1)
    {               
        if(key0_Scan(TRUE))
  
   //  ��Ϊ��վ
        {
        //��ȡHoldReg F0x03
        Modbus_Master(3, FALSE, 40001, 5, Data_Struct.Buffer); 
        
        //�Ѷ��ص�4xxxxֵ�������վ��F0x010
        if(Modbus_Status_Struct.bDone)
        {
            //дHoldReg F0x10
            Modbus_Master(3, TRUE, 40021, 5, Data_Struct.Buffer);
            
        }
        
        //��ȡAI 3xxxx F0x04
        Modbus_Master(3, FALSE, 30000, 5, Data_Struct.Buffer);
        //�Ѷ�ȡ��3xxxxֵ������4xxxx��F0x10
        if(Modbus_Status_Struct.bDone)
            //дHoldReg F0x10
            Modbus_Master(3, TRUE, 40011, 5, Data_Struct.Buffer);
        
        //��ȡ1xxxx:DI:F0x02
        Modbus_Master(3, FALSE, 10000, 5, Data_Struct.Buffer);
        if(Modbus_Status_Struct.bDone)
        {
            //�ݴ��ȡ��DI���ݡ��ṹ��ֵ�Ǵ�ֵ�����Ǵ�ַ��
            Buffer_Temp = Data_Struct;
            //�Ѷ�ȡ��1xxxxDIֵ������0xxxx.F0x0F
            //Ԫ����ַ0xxxx����ʹ��0��ͷ��0��ͷΪ8���ơ�
            Modbus_Master(3, TRUE, 10, 5, Data_Struct.Buffer);
        }
        
        
        //��ȡ0xxxx DQ:F0x01,��DI 1xxxx ֵ�Ƚϡ�
        Modbus_Master(3, FALSE, 10, 5, Data_Struct.Buffer);        
            
        //5��DQ��ֻ��1���ֽ����ݡ��޷�ֱ�ӱȽ����飬ֻ��ʹ�ÿ⺯����for��
        if(Buffer_Temp.Buffer[0] != Data_Struct.Buffer[0])
        {
            printf("DQ�������ݺ�ǿ�ƵĲ�һ�£�\n");
        }
        
       //delay_ms(300);   

    } 

    
    }
*/
    ReceiveFrame(&RX_Struct);  
 
    while(1)
    {

        
    //if(!KEY0)
    if(!KEY0)  //���ڲ���ɨ�跽ʽ�����жϣ����ܻ�����ͨѶ��ʱ���ִ����������ʱ�̡�
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





