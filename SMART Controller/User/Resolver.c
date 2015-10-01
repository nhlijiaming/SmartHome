#include <stm32f10x.h>
#include <stdio.h>
#include "Resolver.h"
#include "DHT11.h"
#include "MQ5.h"
#include "BH1750.h"

extern vu16 AD_Value;
extern u32 Value;

extern uint8_t DataString[20];

void Measure_GI(void)
{
    int i;
    sprintf((char *)DataString,"$GI%02d@",Value);
    i=0;
    while (DataString[i])
    {
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
        USART_SendData(USART1, DataString[i]);
        i++;
    }
}

void Measure_TH(void)
{
    int i;
    ReadDHT11();
    ReadDHT11();
    sprintf((char *)DataString,"$TH%02d,%02d@", tdata[2], tdata[0]);
    i=0;
    while (DataString[i])
    {
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
        USART_SendData(USART1, DataString[i]);
        i++;
    }
}

void Measure_LI(void)
{
    int i;
    Read_BH1750();
    i=0;
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    USART_SendData(USART1, '$');
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    USART_SendData(USART1, 'L');
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    USART_SendData(USART1, 'I');
    while (DataString[i])
    {
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
        USART_SendData(USART1, DataString[i]);
        i++;
    }
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    USART_SendData(USART1, '@');
}

void Measure_SS(void)
{
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    USART_SendData(USART1, '$');
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    USART_SendData(USART1, 'S');
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    USART_SendData(USART1, 'S');
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    USART_SendData(USART1, '0' + GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13));
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    USART_SendData(USART1, '0' + GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9));
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    USART_SendData(USART1, '@');
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    USART_SendData(USART1, 0x0d);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    USART_SendData(USART1, 0x0a);
}

void Resolver(volatile unsigned char Comm[])
{
    if (Comm[1]=='L' && Comm[3]=='O' && Comm[4]=='n')                              //开电器
    {
        switch (Comm[2])
        {
            case '1': Relay1_Set; break;
            case '2': Relay2_Set; break;
            case '3': Relay3_Set; break;
            default : break;
        }
    }
    if (Comm[1]=='L' && Comm[3]=='O' && Comm[4]=='f' && Comm[5]=='f')        //关电器
    {
        switch (Comm[2])
        {
            case '1': Relay1_Reset; break;
            case '2': Relay2_Reset; break;
            case '3': Relay3_Reset; break;
            default : break;
        }
    }
    if (Comm[1]=='G' && Comm[2]=='e' && Comm[3]=='t' && Comm[4]=='T' && Comm[5]=='H')     //测量温度
    {
        Measure_TH();
    }
    if (Comm[1]=='G' && Comm[2]=='e' && Comm[3]=='t' && Comm[4]=='L' && Comm[5]=='I')     //测量光强
    {
        Measure_LI();
    }
    if (Comm[1]=='G' && Comm[2]=='e' && Comm[3]=='t' && Comm[4]=='G' && Comm[5]=='I')    //测量可燃气体浓度
    {
        Measure_GI();                
    }
    if (Comm[1]=='G' && Comm[2]=='e' && Comm[3]=='t' && Comm[4]=='S' && Comm[5]=='e')    //测量门窗传感器数据
    {
        Measure_SS();
    }
}