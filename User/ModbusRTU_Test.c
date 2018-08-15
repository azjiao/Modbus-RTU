/*********************************************************
  ���ܣ� ����Modbus-RTUͨѶ����
  ������ ��Ӣ��485�ӿ���USART2�ڣ�ʹ�ô���2����Modbus-RTU���顣
  ��ƣ� azjiao
  �汾�� 0.1
  ���ڣ� 2018��07��18��
 *********************************************************/

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "template.h"
#include "dev.h"
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
    delay_Init(); 
    beep_Init();
    Usart1_Init(19200);
    //ModbusͨѶ��ʼ��.
    Modbus_Init();
    Iwdg_Init(4, 625);  //�������Ź���ʼ����Ԥ��Ƶϵ��4��Ӧ64��RLRֵΪ625���������Ź���ʱ1s��
   
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
        Modbus_Slave();
        if(Modbus_Status_Struct.bBusy){
            LED1_OFF;
            LED0_ON;            
        }
        else{
            LED0_OFF;
            LED1_ON;
        }
       Iwdg_Feed();
    }

}





