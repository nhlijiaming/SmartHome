#include <stm32f10x.h>
#include "BH1750.h"
#include "hard_i2c_driver.h"
#include <stdio.h>
#include "delay.h"

uint8_t I2C_Data[10],ret;

extern uint8_t DataString[20];

void Read_BH1750(void)
{
    I2C_Data[0]=0x11;
    ret = VT_I2C_HardWriteNByte(I2C2,0,I2C_Data,1);//���Ͳ�������
    if(ret){
        //printf("I2Cд���ݴ��� %d\n\r",ret);
        sprintf((char *)DataString,"r err");
        //while (1);
    }
    delay_ms(200);//�ȴ�����120ms������������ʱ�����򴫸���û׼�������ݻ���ֶ����ݴ���
    ret = VT_I2C_HardReadNByte(I2C2,0,I2C_Data,2);//��ȡ����ǿ��ֵ
    if(ret)
    {
        //printf("I2C�����ݴ��� %d\n\r",ret);
        sprintf((char *)DataString,"w err");
        //while (1);
    }
    else
    {
        //printf("��ǿ�ȣ�%f lx\n\r",((I2C_Data[0]<<8)+I2C_Data[1])/(1.2*2));
        sprintf((char *)DataString,"%0.2f",((I2C_Data[0]<<8)+I2C_Data[1])/(1.2*2));
    }
}

void BH1750_Init(void)
{
    extern I2C_INIT_INFO I2C_Info;	//I2C�����������Ϣ
    I2C_Info.controlMode = 1;           //Ӳ������
    I2C_Info.speed = 10000;              //10K
    I2C_Info.subAddrMode = 2;           //���ӵ�ַģʽ
    I2C_Info.subAddrWidth = 0;          //�ӵ�ַΪ0�ֽڿ��
    I2C_Info.slaveAddr = 0x46;          //BH1750FVI������I2C��ַ
    I2C_Info.channel = 1;               //ʹ��I2C2
    VT_I2C_HardInit( &I2C_Info);
    I2C_Data[0] = 0x01;
    VT_I2C_HardWriteNByte(I2C2, 0, I2C_Data, 1);    //������оƬ�ϵ�����
}