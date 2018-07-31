/*
 * �����������Դ������key.
 * KEY0:�ӵ�PE4�˿ڣ��������룻
 * KEY1:�ӵ�PE3�˿ڣ��������룻
 * RESET���ӵ�NRST��TODO...
 * WK_UP:�ӵ�PA0�˿ڣ��������룻
 */
#include "stm32f10x.h"
#include "key.h"
#include "delay.h"

//����Key��ʼ����
void key_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct; //������ʼ�����ݽṹ��

    //GPIOA��GPIOEʱ��ʹ�ܡ�
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);

    //KEY0��KEY1������ͬ��
    GPIO_InitStruct.GPIO_Pin = (GPIO_Pin_4 | GPIO_Pin_3);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU; //��������ģʽ��
    GPIO_Init(GPIOE, &GPIO_InitStruct);

    //WK_UP���á�
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD; //��������ģʽ��
    GPIO_Init(GPIOA, &GPIO_InitStruct);

}

//key0 ɨ�����
//bSusKey==TRUE���������¿���ɨ�赽�������ֵ��
//bSusKey==FALSE����������ֻɨ�赽һ������ֵ��
bool key0_Scan( bool bSusKey )
{
    static bool bsLastKey = FALSE;  //�����ϴ�״̬��
    //��������������⵽����ѹ�¡�
    bool bKey;
    //��������ⶨʱ����
    static TimerType timer;
    //����ֵ��
    bool bRet = FALSE;

    assert_param(Is_BOOL(bSusKey));

    // ��������
    bKey = TimeON(KEY0_CODE, 150U, &timer);
    bRet = bKey & (bKey ^ bsLastKey);  //�ؼ�⡣�����ؾ�ΪTRUE������ʱΪFALSE.
    //�������������ģʽ��
    if(bSusKey)
        bsLastKey = FALSE;  //��������ģʽʱ�����ϴΰ���״̬��ΪFALSE��
    //����ǵ�����ģʽ��
    else
        bsLastKey = bKey;


    return bRet;
}

//key1 ɨ�����
//bSusKey==TRUE���������¿���ɨ�赽�������ֵ��
//bSusKey==FALSE����������ֻɨ�赽һ������ֵ��
bool key1_Scan( bool bSusKey )
{
    static bool bsLastKey = FALSE;  //�����ϴ�״̬��
    //��������������⵽����ѹ�¡�
    bool bKey;
    //��������ⶨʱ����
    static TimerType timer;
    //����ֵ��
    bool bRet = FALSE;

    assert_param(Is_BOOL(bSusKey));

    // ��������
    bKey = TimeON(KEY1_CODE, 150U, &timer);
    bRet = bKey & (bKey ^ bsLastKey);  //�ؼ�⡣�����ؾ�ΪTRUE������ʱΪFALSE.
    //�������������ģʽ��
    if(bSusKey)
        bsLastKey = FALSE;  //��������ģʽʱ�����ϴΰ���״̬��ΪFALSE��
    //����ǵ�����ģʽ��
    else
        bsLastKey = bKey;

    return bRet;
}


//WKUP ɨ�����
//bSusKey==TRUE���������¿���ɨ�赽�������ֵ��
//bSusKey==FALSE����������ֻɨ�赽һ������ֵ��
bool WKUP_Scan( bool bSusKey )
{
    static bool bsLastKey = FALSE;  //�����ϴ�״̬��
    //��������������⵽����ѹ�¡�
    bool bKey;
    //��������ⶨʱ����
    static TimerType timer;
    //����ֵ��
    bool bRet = FALSE;

    assert_param(Is_BOOL(bSusKey));

    // ��������
    bKey = TimeON(WKUP_CODE, 150U, &timer);
    bRet = bKey & (bKey ^ bsLastKey);  //�ؼ�⡣�����ؾ�ΪTRUE������ʱΪFALSE.
    //�������������ģʽ��
    if(bSusKey)
        bsLastKey = FALSE;  //��������ģʽʱ�����ϴΰ���״̬��ΪFALSE��
    //����ǵ�����ģʽ��
    else
        bsLastKey = bKey;

    return bRet;
}

