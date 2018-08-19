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
#include "stdio.h"

//������̽ṹ������
KeypadType Keypad;

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

//���ɼ������״ֵ̬����key0����ʱu16Keypad��0λΪ1����key1����ʱu16keypad��1λΪ1��ͬ���2λ��WKUP�ġ�
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

//���̰���״̬ɨ�衣
//�������������⺯�������Լ���������򵥸�������
//���а����������ܡ�
//֧����������ͬһ�鰴�������������ͬһ������״ֵ̬�������Keypad.u16ValSus�С�
//֧����������ͬһ�鰴��ֻ����һ������״ֵ̬�������Keypad.u16ValOnce�С�
//��������ֵΪKeypad.u16DownTrg���ǰ���ͻ���,���򵥶�������ʹ�á�
uint16_t keypadScan(void)
{
    static uint16_t u16KeypadRTTmp = 0x0000; //��һ��ʵʱ����״̬�ݴ�ֵ��
    uint16_t u16KeypadRT;  //����ʵʱ����״ֵ̬��
    uint16_t u16KeypadRTTrg;   //��ʵʱ�����ر仯��
    
    static uint16_t u16KeypadStabValTemp = 0x0000; //��һ�μ���ļ���״ֵ̬��
    uint16_t u16KeypadStabTrg;  //������������»��ͷű�ʶ����⵽�ر仯��
        
    //��������ⶨʱ����
    static TimerType timer;

    //������״ֵ̬
    u16KeypadRT = genKeypadVal();
    
    //����������ǰ�����μ�⵽�ļ���״ֵ̬��ͬʱ�Ż�ʹ��ʱ������������
    //������150ms������
    //���������μ�ⲻһ��ʱ���ֵΪ��һ�εļ��ֵ. 
    //�������������º��ͷ�ʱ����150ms����ʱ��
    u16KeypadRTTrg = u16KeypadRT ^ u16KeypadRTTmp;
    TimeON(!u16KeypadRTTrg, KEYFILTIME, &timer);
    u16KeypadRTTmp = u16KeypadRT;
    if(timer.bQ)
    {
        Keypad.u16ValSus = u16KeypadRT;        
    }
    else        
        Keypad.u16ValSus = u16KeypadStabValTemp;    
    
    //�ؼ�⣺������ͻ�䡣
    u16KeypadStabTrg = (Keypad.u16ValSus ^ u16KeypadStabValTemp); 
    //����ͻ�����⡣    
    Keypad.u16DownTrg = Keypad.u16ValSus & u16KeypadStabTrg;
    //�ͷ�ͻ�����⡣
    Keypad.u16UpTrg = ~Keypad.u16ValSus & u16KeypadStabTrg;
    //ֻ����һ�μ��,������ϴβ�һ����״ֵ̬������ʵ����������ͬһ���ʱֻ�ܼ��һ��״ֵ̬����
    if(u16KeypadStabTrg)
        Keypad.u16ValOnce = Keypad.u16ValSus;
    else 
        Keypad.u16ValOnce = 0;
    
    u16KeypadStabValTemp = Keypad.u16ValSus;    
    
    return Keypad.u16DownTrg;
}
