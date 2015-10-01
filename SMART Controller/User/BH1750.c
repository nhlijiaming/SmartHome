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
    ret = VT_I2C_HardWriteNByte(I2C2,0,I2C_Data,1);//发送测量命令
    if(ret){
        //printf("I2C写数据错误 %d\n\r",ret);
        sprintf((char *)DataString,"r err");
        //while (1);
    }
    delay_ms(200);//等待大于120ms，必须加这个延时，否则传感器没准备好数据会出现读数据错误
    ret = VT_I2C_HardReadNByte(I2C2,0,I2C_Data,2);//读取光照强度值
    if(ret)
    {
        //printf("I2C读数据错误 %d\n\r",ret);
        sprintf((char *)DataString,"w err");
        //while (1);
    }
    else
    {
        //printf("光强度：%f lx\n\r",((I2C_Data[0]<<8)+I2C_Data[1])/(1.2*2));
        sprintf((char *)DataString,"%0.2f",((I2C_Data[0]<<8)+I2C_Data[1])/(1.2*2));
    }
}

void BH1750_Init(void)
{
    extern I2C_INIT_INFO I2C_Info;	//I2C控制器相关信息
    I2C_Info.controlMode = 1;           //硬件控制
    I2C_Info.speed = 10000;              //10K
    I2C_Info.subAddrMode = 2;           //无子地址模式
    I2C_Info.subAddrWidth = 0;          //子地址为0字节宽度
    I2C_Info.slaveAddr = 0x46;          //BH1750FVI传感器I2C地址
    I2C_Info.channel = 1;               //使用I2C2
    VT_I2C_HardInit( &I2C_Info);
    I2C_Data[0] = 0x01;
    VT_I2C_HardWriteNByte(I2C2, 0, I2C_Data, 1);    //传感器芯片上电命令
}