#include <stm32f10x.h>
#include "DHT11.h"
#include "delay.h"
#include <stdio.h>
#include "MQ5.h"
#include "BH1750.h"
#include "Resolver.h"
#include "GSM.h"

#define Threshold 15                //可燃气体警告阈值
#define Send_Data_Int 10            //传感器数据发送间隔

vu16 AD_Value;
u32 Value;
volatile unsigned char SS_Flag;
volatile unsigned char Hr=0, Min=0, Sec=0;
volatile unsigned char INT = 0;
volatile unsigned char USART_Refresh;
volatile unsigned char USART_Buff[40];
volatile unsigned char GSM_Refresh;
volatile unsigned char GSM_BuffCount;
volatile unsigned char GSM_Buff[10][100];

int T;
int index=0;

uint8_t DataString[20];

void GPIO_Config()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;                   //MQ-2输入
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init( GPIOB, &GPIO_InitStructure );
    
    Relay1_Reset;
    Relay2_Reset;
    Relay3_Reset;
    Relay4_Reset;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;                   //继电器输出
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init( GPIOB, &GPIO_InitStructure );
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_4 | GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init( GPIOA, &GPIO_InitStructure );
    
    //    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;                   //蜂鸣器输出
    //    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    //    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    //    GPIO_Init( GPIOA, &GPIO_InitStructure );
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;    //GY-30光强模块
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init( GPIOB, &GPIO_InitStructure );
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;                   //DHT-11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOA, &GPIO_InitStructure );
    

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;                   //LC-05 EN
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init( GPIOA, &GPIO_InitStructure );
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                   //USART1 TX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init( GPIOA, &GPIO_InitStructure );
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                   //USART1 RX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init( GPIOA, &GPIO_InitStructure );
    
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;                   //USART2 TX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init( GPIOA, &GPIO_InitStructure );
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;                   //USART2 RX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init( GPIOA, &GPIO_InitStructure );
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;                   //外部中断源1
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init( GPIOC, &GPIO_InitStructure );
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                   //外部中断源2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init( GPIOB, &GPIO_InitStructure );
}

void TIM_Config()//fTIM2 = 1Hz
{
    
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    
    TIM_Cmd(TIM2, DISABLE);//先停止TIM2时钟，以准备下面的设置
    
    TIM_TimeBaseStructure.TIM_Period = 7200 - 1; //APR寄存器
    TIM_TimeBaseStructure.TIM_Prescaler = 10000 - 1; //预分频值，用来调整频率，分频系数=1000khz/(prescaler+1)
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
    TIM_ClearFlag(TIM2,TIM_FLAG_Update);   //必须先清除配置时候产生的更新标志
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);   //使能中断，中断事件为定时器工薪事件
    TIM_Cmd(TIM2, ENABLE);
}

void NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
#ifdef  VECT_TAB_RAM
    /* Set the Vector Table base location at 0x20000000 */
    NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
    /* Set the Vector Table base location at 0x08000000 */
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif    
    
    //    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    //	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    //	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    //	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    //	NVIC_Init(&NVIC_InitStructure);
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);     //NVIC_PriorityGroup_2, NVIC_IRQChannelPreemptionPriority 0~3, NVIC_IRQChannelSubPriority 0~3
    
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
}

void EXTI_Config(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_ClearITPendingBit(EXTI_Line9);
    EXTI_ClearITPendingBit(EXTI_Line13);
    
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource9);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13);
    
    EXTI_InitStructure.EXTI_Line = EXTI_Line9 | EXTI_Line13;        //选择中断线路9 13
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;             //设置为中断请求，非事件请求
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  //设置中断触发方式为上下降沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;                       //外部中断使能
    EXTI_Init(&EXTI_InitStructure);
}

void USART_Config(void)
{
    USART_InitTypeDef USART_InitStructure;
    
    USART_InitStructure.USART_BaudRate = 115200;								 //波特率9600
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;				 //数据位:8bits
    USART_InitStructure.USART_StopBits = USART_StopBits_1;					 //停止位:1bit
    USART_InitStructure.USART_Parity = USART_Parity_No;						 //无极偶校验
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	 //无流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;			 //接收发送使能  Tx=:PA9  Rx:PA10
    USART_Init(USART1, &USART_InitStructure);								 //初始化
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);							 //开启接收中断
    USART_Cmd(USART1, ENABLE);												 //启动USART1                                                      
    
    USART_InitStructure.USART_BaudRate = 9600;								 //波特率9600
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;				 //数据位:8bits
    USART_InitStructure.USART_StopBits = USART_StopBits_1;					 //停止位:1bit
    USART_InitStructure.USART_Parity = USART_Parity_No;						 //无极偶校验
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	 //无流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;			 //接收发送使能  Tx:PA2  Rx:PA3
    USART_Init(USART2, &USART_InitStructure);								 //初始化
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);							 //开启接收中断
    USART_Cmd(USART2, ENABLE);												 //启动USART2
}

u16 Get_GasConcentration(u16 advalue)
{
    if (advalue <= 120) return 0;
    advalue -= 120;
    return (u16)(advalue * (2700-120) / 4096 / 30);
}

void main()
{
    unsigned char lastSec;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    
    GSM_BuffCount = 0;
    
    GPIO_Config();
    TIM_Config();
    MQ5_Init();
    USART_Config();
    NVIC_Config();
    BH1750_Init();
    EXTI_Config();
    if (SysTick_Config(SystemCoreClock / 1000000)) //1us
    { 
        /* Capture error */ 
        while (1);
    }
    
    delay_ms(1000);
    delay_ms(1000);
    delay_ms(1000);
    delay_ms(1000);
    delay_ms(1000);
    delay_ms(1000);
    //send "AT"
    while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
    USART_SendData(USART2, 0x41);
    while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
    USART_SendData(USART2, 0x54);
    while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
    USART_SendData(USART2, 0x0d);
    while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
    USART_SendData(USART2, 0x0a);
    
    
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    ReadDHT11();
    while (1)
    {
        
        Value = Get_GasConcentration(AD_Value);
        
        if (USART_Refresh==1 && USART_Buff[0]=='$')
        {
            USART_Refresh = 0;
            Resolver((unsigned char *)USART_Buff);
        }
        
        if (INT >= Send_Data_Int)       //每 Send_Data_Int 秒检测一次数据
        {
            INT -= Send_Data_Int;
            Measure_TH();
            Measure_LI();
            Measure_GI();  
            Measure_SS();
        }
        
        if (Value > Threshold)          //可燃气体浓度过大
        {
            Relay4_Set;
            //Beep_Set;
        }
        else
        {
            Relay4_Reset;
            //Beep_Reset;
        }
        
        if (SS_Flag)                    //门窗传感器有反应
        {
            SS_Flag = 0;
            Measure_SS();
        }
        
        //GSM_Refresh = 0;
        if (GSM_Refresh)
        {
            GSM_Refresh = 0;
            ReceiveSMS();
        }
            
        lastSec = Sec;                  //等下一秒
        while (lastSec==Sec && !USART_Refresh && !GSM_Refresh) ;
    }
    
}