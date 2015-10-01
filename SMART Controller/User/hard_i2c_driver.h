/**
  ******************************************************************************
  * @file    hard_i2c_driver.h
  * $Author: wdluo $
  * $Revision: 29 $
  * $Date:: 2012-07-14 11:01:01 +0800 #$
  * @brief   主函数包含的头文件.
  ******************************************************************************
  * @attention
  *
  *<h3><center>&copy; Copyright 2009-2012, ViewTool</center>
  *<center><a href="http:\\www.viewtool.com">http://www.viewtool.com</a></center>
  *<center>All Rights Reserved</center></h3>
  * 
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HARD_I2C_DRIVER_H__
#define __HARD_I2C_DRIVER_H__

//I2C初始化信息结构体
typedef struct
{
    unsigned char controlMode;  ///<I2C控制方式
    unsigned int  speed;        ///<I2C读写速度
    unsigned char subAddrMode;  ///<是否有子地址
    unsigned char subAddrWidth; ///<子地址字节数
    unsigned short slaveAddr;   ///<设备地址
	unsigned char channel;		///<需要初始化的通道
}I2C_INIT_INFO;

#define MASTER_MODE_SELECT_FAILED                          (1)
#define MASTER_TRANSMITTER_MODE_SELECTED_FAILED            (2)    
#define MASTER_RECEIVER_MODE_SELECTED_FAILED               (3)
#define MASTER_BYTE_RECEIVED_FAILED                        (4)
#define MASTER_BYTE_TRANSMITTING_FAILED                    (5)
#define MASTER_BYTE_TRANSMITTED_FAILED                     (6)
#define MASTER_MODE_ADDRESS10_FAILED                       (7)
#define MASTER_GET_I2C_FLAG_BUSY_FAILED					   (8)

extern uint8_t VT_I2C_HardInit(I2C_INIT_INFO *I2C_Info);
extern uint8_t VT_I2C_HardWriteNByte(I2C_TypeDef *I2Cx, uint32_t subaddr, uint8_t *s, uint32_t num);
extern uint8_t VT_I2C_HardReadNByte(I2C_TypeDef *I2Cx, uint32_t suba,uint8_t *s,uint32_t num);
#endif

