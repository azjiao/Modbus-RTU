#ifndef __KEY_H
#define __KEY_H
#include "template.h"

//����KEY0��KEY1��Wk_UP������Ϊ��Ӧkey��������ֵ��
//��Ϊ��ֵ��KEY0��KEY1����ʱֵΪ0��WKUP����ʱֵΪ1.
#define KEY0   GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4)
#define KEY1   GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3)
#define WKUP   GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)

//��������������ʱ�ı���ֵ.�������ڰ����ͷż��Ҳ�������ڼ�����ֵ��⡣
//ע�⣺������ֵ��KEY0/KEY1/WKUP������������.
#define KEY0_CODE   !KEY0?  0xf0: 0x00
#define KEY1_CODE   !KEY1?  0xf1: 0x00
#define WKUP_CODE   WKUP?   0xf2: 0x00

//����Key��ʼ����
void key_Init(void);

//Key0ɨ�����
//bSusKey==TRUE:�������´�����Ρ�
//bSusKey==FALSE:�������µ���һ�ΰ��´���
bool key0_Scan( bool bSusKey );
//Key1ɨ�����
bool key1_Scan( bool bSusKey );
//WK_UPɨ�����
bool WKUP_Scan( bool bSusKey );

#endif /* ifndef __KEY_H */
