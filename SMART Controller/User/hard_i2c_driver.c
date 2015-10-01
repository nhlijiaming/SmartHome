#include <stm32f10x.h>
#include "hard_i2c_driver.h"
I2C_INIT_INFO I2C_Info;	///<I2C�����������Ϣ

uint8_t VT_I2C_HardInit(I2C_INIT_INFO *I2C_Info)
{
	I2C_InitTypeDef I2C_InitStructure;
   	GPIO_InitTypeDef GPIO_InitStructure;
   	if(0==I2C_Info->channel)
    {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
	   	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO , ENABLE);
	
	   	/* Configure I2C1 pins: PB6->SCL and PB7->SDA */
	   	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
	   	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	   	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	   	GPIO_Init(GPIOB, &GPIO_InitStructure);
			
	   	I2C_DeInit(I2C1);
	   	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	   	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	   	I2C_InitStructure.I2C_OwnAddress1 = 0x30;
	   	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
		if(I2C_Info->slaveAddr>>8)
        {
			I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_10bit;
		}
        else
        {
	   		I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
		}
	   	I2C_InitStructure.I2C_ClockSpeed = I2C_Info->speed;
	    
	   	I2C_Cmd(I2C1, ENABLE);
	   	I2C_Init(I2C1, &I2C_InitStructure);
	
	   	I2C_AcknowledgeConfig(I2C1, ENABLE);
   	}
    else
    {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
	   	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO , ENABLE);
	
	   	/* Configure I2C1 pins: PB10->SCL and PB11->SDA */
	   	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11;
	   	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	   	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	   	GPIO_Init(GPIOB, &GPIO_InitStructure);
			
	   	I2C_DeInit(I2C2);
	   	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	   	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	   	I2C_InitStructure.I2C_OwnAddress1 = 0x30;
	   	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
		if(I2C_Info->slaveAddr>>8)
        {
			I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_10bit;
		}
        else
        {
	   		I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
		}
	   	I2C_InitStructure.I2C_ClockSpeed = I2C_Info->speed;
	    
	   	I2C_Cmd(I2C2, ENABLE);
	   	I2C_Init(I2C2, &I2C_InitStructure);
	
	   	I2C_AcknowledgeConfig(I2C2, ENABLE);
	}
	return 0;
}
/**
  * @brief  ͨ��Ӳ��I2Cд���� ��
  * @param  I2Cx Ӳ��I2Cͨ����
  * @param  subaddr д���ݵ��ӵ�ַ�����������ӵ�ַ������������塣
  * @param  s ��Ҫд�����ݻ�������ַ��
  * @param  num ��Ҫд�������ֽ�����
  * @retval д����״̬��
  */
uint8_t VT_I2C_HardWriteNByte(I2C_TypeDef *I2Cx, uint32_t subaddr, uint8_t *s, uint32_t num)
{
  	uint32_t	subaddrNum=I2C_Info.subAddrWidth;
	uint32_t retry=0;
	uint8_t *p_data=s;
	I2C_GenerateSTART(I2Cx, ENABLE);
	retry=0;
  	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
	{
		retry++;
		if(retry>=1000){
			I2C_GenerateSTOP(I2Cx, ENABLE);
			return MASTER_MODE_SELECT_FAILED;	
		}
	}
	//�����豸��ַ
	if(I2C_Info.slaveAddr>>8){
  		//I2C_Send7bitAddress(I2Cx, I2C_Addr, I2C_Direction_Transmitter);
	}
    else
    {
		I2C_Send7bitAddress(I2Cx, I2C_Info.slaveAddr, I2C_Direction_Transmitter);
		retry=0;
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		{
			retry++;
			if(retry>=1000){
				I2C_GenerateSTOP(I2Cx, ENABLE);
				return MASTER_TRANSMITTER_MODE_SELECTED_FAILED;	
			}
		}
	}
  	
	//�����ӵ�ַ
	while(subaddrNum>0){
	  	I2C_SendData(I2Cx, subaddr>>((subaddrNum-1)*8));
		retry=0;
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
			retry++;
			if(retry>=1000){
				I2C_GenerateSTOP(I2Cx, ENABLE);
				return MASTER_BYTE_TRANSMITTED_FAILED;	
			}
		}
		subaddrNum--;
	}
	//��������
	while(num--){
	  	I2C_SendData(I2Cx, *p_data++); 
		retry=0;
  		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
			retry++;
			if(retry>=1000){
				I2C_GenerateSTOP(I2Cx, ENABLE);
				return MASTER_BYTE_TRANSMITTED_FAILED;	
			}
		}
	}
	//ֹͣ����
	I2C_GenerateSTOP(I2Cx, ENABLE);
	return 0;
}
/**
  * @brief  ͨ��Ӳ��I2C������ ��
  * @param  I2Cx Ӳ��I2Cͨ����
  * @param  subaddr �����ݵ��ӵ�ַ�����������ӵ�ַ������������塣
  * @param  s ��Ҫ�������ݴ洢��������ַ��
  * @param  num ��Ҫ���������ֽ�����
  * @retval ������״̬��
  */
uint8_t VT_I2C_HardReadNByte(I2C_TypeDef *I2Cx, uint32_t subaddr,uint8_t *s,uint32_t num)
{
  	int32_t	subaddrNum=I2C_Info.subAddrWidth;
	uint8_t *p_data=s;	
	uint32_t retry=0;
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
	{
		retry++;
		if(retry>=1000){
			I2C_GenerateSTOP(I2Cx, ENABLE);
			return MASTER_GET_I2C_FLAG_BUSY_FAILED;	
		}
	}
	I2C_AcknowledgeConfig(I2Cx, ENABLE);
    I2C_GenerateSTART(I2Cx, ENABLE);
	retry=0;
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
	{
		retry++;
		if(retry>=1000)
        {
			I2C_GenerateSTOP(I2Cx, ENABLE);
			return MASTER_MODE_SELECT_FAILED;	
		}
	}
	if(subaddrNum>0)
    {
		//�����豸��ַ
		if(I2C_Info.slaveAddr>>8)
        {
	  		//I2C_Send7bitAddress(I2Cx, I2C_Addr, I2C_Direction_Transmitter);
		}
        else
        {
			I2C_Send7bitAddress(I2Cx, I2C_Info.slaveAddr, I2C_Direction_Transmitter);
			retry=0;
			while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
			{
				retry++;
				if(retry>=1000){
					I2C_GenerateSTOP(I2Cx, ENABLE);
					return MASTER_TRANSMITTER_MODE_SELECTED_FAILED;	
				}
			}
		} 
		//�����ӵ�ַ
		while(subaddrNum>0)
        {
		  	I2C_SendData(I2Cx, subaddr>>((subaddrNum-1)*8));
			retry=0;
			while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
			{
				retry++;
				if(retry>=1000)
                {
					I2C_GenerateSTOP(I2Cx, ENABLE);
					return MASTER_BYTE_TRANSMITTED_FAILED;	
				}
			}
			subaddrNum--;
		}
		//��������I2C����
		I2C_GenerateSTART(I2Cx, ENABLE);
		retry=0;
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
		{
			retry++;
			if(retry>=1000)
            {
				I2C_GenerateSTOP(I2Cx, ENABLE);
				return MASTER_MODE_SELECT_FAILED;	
			}
		}
		//���Ͷ��豸��ַ
		I2C_Send7bitAddress(I2Cx, I2C_Info.slaveAddr, I2C_Direction_Receiver);
		retry=0;
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
		{
			retry++;
			if(retry>=1000)
            {
				I2C_GenerateSTOP(I2Cx, ENABLE);
				return MASTER_RECEIVER_MODE_SELECTED_FAILED;	
			}
		}
	}
    else
    {
		I2C_Send7bitAddress(I2Cx, I2C_Info.slaveAddr, I2C_Direction_Receiver);
		retry=0;
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
		{
			retry++;
			if(retry>=1000)
            {
				I2C_GenerateSTOP(I2Cx, ENABLE);
				return MASTER_RECEIVER_MODE_SELECTED_FAILED;	
			}
		}
	}
	//��ȡ����
    while (num)
    {
		if(num==1)
		{
     		I2C_AcknowledgeConfig(I2Cx, DISABLE);
    		I2C_GenerateSTOP(I2Cx, ENABLE);
		}
	    retry = 0;
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED))
		{
			retry++;
			if(retry>=1000)
            {
				I2C_GenerateSTOP(I2Cx, ENABLE);
				return MASTER_BYTE_RECEIVED_FAILED;	
			}
		}
	    *p_data = I2C_ReceiveData(I2Cx);
	    p_data++;
	    /* Decrement the read bytes counter */
	    num--;
    }

	I2C_AcknowledgeConfig(I2Cx, ENABLE);
	return 0;
}