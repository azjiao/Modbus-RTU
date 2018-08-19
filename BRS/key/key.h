#ifndef __KEY_H
#define __KEY_H
#include "template.h"

//����KEY0��KEY1��Wk_UP������Ϊ��Ӧkey��������ֵ��
//��Ϊ��ֵ��KEY0��KEY1����ʱֵΪ0��WKUP����ʱֵΪ1.
#define KEY0   GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4)
#define KEY1   GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3)
#define WKUP  GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)

//��������������ʱ�Ķ�������ֵ.�������ڰ����ͷż��Ҳ�������ڼ�����ֵ��⡣
//ע�⣺������ֵ��KEY0/KEY1/WKUP������������.
#define KEY0_CODE   !KEY0?  0x01: 0x00
#define KEY1_CODE   !KEY1?  0x02: 0x00
#define WKUP_CODE   WKUP?   0x04: 0x00

//�����������˲�������ms
#define KEYFILTIME   150U

//������ʹ�ü���ɨ�蹦��keypadScan(�ǵ�����������)�����ֵ���塣
//�����genKeypadVal()�������ļ�ֵһ�¡�
#define KP_KEY0    0x0001
#define KP_KEY1    0x0002
#define KP_WKUP    0x0004

//������Ŀ��Ҫ�������谴����ϣ�����ǵ��������Ͳ������ⶨ���ˡ�
//��ϼ������˳��Ҫ��������ʱ����ʹ��Keypad.u16ValOnce������Keypad.u16ValSus���ж�.
//KEY0-WKUP��ϣ��Һ���WKUP��
#define KEY0_WKUP    (KP_KEY0 + KP_WKUP) && (Keypad.u16DownTrg == KP_WKUP)
//KEY1-KEY0��ϣ��Һ���KEY0.
#define KEY1_KEY0    (KP_KEY1 + KP_KEY0) && (Keypad.u16DownTrg == KP_KEY0)
//KEY0-KEY1��ϣ���˳��Ҫ��
#define KEY0_KEY1    (KP_KEY0 + KP_KEY1)

//���̽ṹ
typedef struct
{
    uint16_t u16SusTime;  //������������ļ��ʱ�䣬������ʱ����Ϊ������ϡ���ʱ��Ӧ���ڷ���ʱ�䡣
    uint16_t u16ValSus; //��ǰ����״ֵ̬(����״ֵ̬)��
    uint16_t u16ValOnce; //��һ��ʱ�ļ���״ֵ̬(��������ͬһ���ֻ����һ��ֵ)��
    uint16_t u16DownTrg;  //���̰���ͻ���
    uint16_t u16UpTrg;  //�����ͷ�ͻ���

}KeypadType;
extern KeypadType Keypad;

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

//���ɼ������״ֵ̬����key0����ʱKEYPAD��0λΪ1����key1����ʱKEYPAD��1λΪ1��ͬ���3λ��WKUP�ġ�
uint16_t genKeypadVal(void);

//���̰���״̬��⡣
//�������������⺯�������Լ���������򵥸�������
//���а����������ܡ�
uint16_t keypadScan(void);

#endif /* ifndef __KEY_H */
