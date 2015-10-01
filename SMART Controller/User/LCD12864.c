/************************************************************
  Copyright ( C ), 2012 - 2013
  FileName: LCD12864.c
  Author: Lj       Version : 1.00         Date: 2013/9/3
  Description:     ST7920驱动文件
  Function List:   
    1. 
    2. 
    3. 
    4. 
    5. 
  History:
      <author>  <time>   <version >      <desc>
      Lj        2013/9/3     1.0     build this moudle  
  Connection:
      PE7: RST
      PE9 PE11 PE13 PE15 PB11 PB13 PB15 PD9: DB7-DB0
      PD11: E
      PD13: RW
      PD15: RS
***********************************************************/
#include <stm32f10x.h>
#include <LCD12864.h>


/******************************************************************************/
/*                           define symbols                                   */
/******************************************************************************/

#define RS_Set GPIO_SetBits(GPIOA, RS)
#define RW_Set GPIO_SetBits(GPIOA, RW)
#define E_Set GPIO_SetBits(GPIOB, E)

#define RS_Reset GPIO_ResetBits(GPIOA, RS)
#define RW_Reset GPIO_ResetBits(GPIOA, RW)
#define E_Reset GPIO_ResetBits(GPIOB, E)

//GPIOA
#define DB0 GPIO_Pin_0
#define DB1 GPIO_Pin_1
#define DB2 GPIO_Pin_2
#define DB3 GPIO_Pin_3
#define DB4 GPIO_Pin_4
#define DB5 GPIO_Pin_5
#define DB6 GPIO_Pin_6
#define DB7 GPIO_Pin_7

//GPIOA
#define RS GPIO_Pin_11
#define RW GPIO_Pin_12
//GPIOB
#define E GPIO_Pin_9

/******************************************************************************/
/*                         internal function                                  */
/******************************************************************************/

/******** 配置GPIO ********/
void LCD12864_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = RS | RW | DB0 | DB1 | DB2 | DB3 | DB4 | DB5 | DB6 | DB7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init( GPIOA, &GPIO_InitStructure );
    
    GPIO_InitStructure.GPIO_Pin = E;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init( GPIOB, &GPIO_InitStructure );
    return;
}

/******** 等待空闲 ********/
void LCD12864_WaitForIdle(void)
{
    unsigned char BF;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /******** 设置DB7为输入 ********/
    GPIO_InitStructure.GPIO_Pin = DB7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init( GPIOA, &GPIO_InitStructure );
    
	RS_Reset;
	RW_Set;
    BF = 1;
	while( BF )
	{
        E_Set;
        BF = GPIO_ReadInputDataBit( GPIOA, DB7 );
        E_Reset;
    }
    
    /******** 恢复DB7为输出 ********/
    GPIO_InitStructure.GPIO_Pin = DB7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init( GPIOA, &GPIO_InitStructure );
}

/******** 写数据到指令寄存器 ********/
void LCD12864_SendCommand(unsigned char comm)
{
	LCD12864_WaitForIdle();     //等待空闲
    RS_Reset;
	RW_Reset;
	E_Set;
    if ( comm & 0x01 ) GPIO_SetBits( GPIOA, DB0 ); else GPIO_ResetBits( GPIOA, DB0 );
    if ( comm & 0x02 ) GPIO_SetBits( GPIOA, DB1 ); else GPIO_ResetBits( GPIOA, DB1 );
    if ( comm & 0x04 ) GPIO_SetBits( GPIOA, DB2 ); else GPIO_ResetBits( GPIOA, DB2 );
    if ( comm & 0x08 ) GPIO_SetBits( GPIOA, DB3 ); else GPIO_ResetBits( GPIOA, DB3 );
    if ( comm & 0x10 ) GPIO_SetBits( GPIOA, DB4 ); else GPIO_ResetBits( GPIOA, DB4 );
    if ( comm & 0x20 ) GPIO_SetBits( GPIOA, DB5 ); else GPIO_ResetBits( GPIOA, DB5 );
    if ( comm & 0x40 ) GPIO_SetBits( GPIOA, DB6 ); else GPIO_ResetBits( GPIOA, DB6 );
    if ( comm & 0x80 ) GPIO_SetBits( GPIOA, DB7 ); else GPIO_ResetBits( GPIOA, DB7 );
    E_Reset;
    //__delay();
}

/******************************************************************************/
/*                         external function                                  */
/******************************************************************************/

/******** 写数据到数据寄存器 ********/
void LCD12864_SendData(unsigned char data)
{
	LCD12864_WaitForIdle();     //等待空闲
	RS_Set;
	RW_Reset;
	E_Set;
    if ( data & 0x01 ) GPIO_SetBits( GPIOA, DB0 ); else GPIO_ResetBits( GPIOA, DB0 );
    if ( data & 0x02 ) GPIO_SetBits( GPIOA, DB1 ); else GPIO_ResetBits( GPIOA, DB1 );
    if ( data & 0x04 ) GPIO_SetBits( GPIOA, DB2 ); else GPIO_ResetBits( GPIOA, DB2 );
    if ( data & 0x08 ) GPIO_SetBits( GPIOA, DB3 ); else GPIO_ResetBits( GPIOA, DB3 );
    if ( data & 0x10 ) GPIO_SetBits( GPIOA, DB4 ); else GPIO_ResetBits( GPIOA, DB4 );
    if ( data & 0x20 ) GPIO_SetBits( GPIOA, DB5 ); else GPIO_ResetBits( GPIOA, DB5 );
    if ( data & 0x40 ) GPIO_SetBits( GPIOA, DB6 ); else GPIO_ResetBits( GPIOA, DB6 );
    if ( data & 0x80 ) GPIO_SetBits( GPIOA, DB7 ); else GPIO_ResetBits( GPIOA, DB7 );
    E_Reset;
    //__delay();
}

/******** 初始化 ********/
void LCD12864_Init(void)
{
    LCD12864_GPIO_Config();
//    PSB=1;  //开启并行模式
//    RST_Set;
//    RST_Reset;
//    RST_Set;
  	LCD12864_SendCommand( 0x30 );	//基本指令集,8位并行
	LCD12864_SendCommand( 0x06 );	//启始点设定：光标右移
	LCD12864_SendCommand( 0x0C );	//显示状态开关：整体显示开，光标显示关，光标显示反白关
	LCD12864_SendCommand( 0x01 );	//清除显示DDRAM
}

/******** 清屏 ********/
void LCD12864_Clear(void)
{
    LCD12864_SendCommand( 0x30 );
    LCD12864_SendCommand( 0x01 );   //清除显示DDRAM
}

/******** 显示汉字或字符 ********/
void LCD12864_Disp(unsigned char *chn)
{ 
  unsigned char i, j; 
  LCD12864_SendCommand( 0x30 ); //*基本指令动作 

  LCD12864_SendCommand( 0x80 ); //*设定绘图RAM地址 第一行
  j=0; 
  for ( i=0 ; i < 16 ; i++ ) 
    LCD12864_SendData( chn[j * 16 + i] ); 

  LCD12864_SendCommand( 0x90 ); //*设定绘图RAM地址 第二行
  j=1; 
  for ( i=0 ; i < 16 ; i++ ) 
    LCD12864_SendData( chn[j * 16 + i] ); 

  LCD12864_SendCommand( 0x88 ); //*设定绘图RAM地址 第三行
  j=2; 
  for ( i=0 ; i < 16 ; i++ ) 
    LCD12864_SendData( chn[j * 16 + i] ); 

  LCD12864_SendCommand( 0x98 ); //*设定绘图RAM地址 第四行
  j=3; 
  for ( i=0 ; i < 16 ; i++ ) 
    LCD12864_SendData( chn[j * 16 + i] ); 
} 

void LCD12864_SetPosition(unsigned char x, unsigned char y)
{
    switch( x )
    {
        case 1: LCD12864_SendCommand( 0x80 + y ); break;
        case 2: LCD12864_SendCommand( 0x90 + y ); break;
        case 3: LCD12864_SendCommand( 0x88 + y ); break;
        case 4: LCD12864_SendCommand( 0x98 + y ); break;
        default :;
    }
}

void LCD12864_Char(unsigned char x, unsigned char y, unsigned char *chn)
{
    LCD12864_SetPosition(x, y);
    while (*chn!=0)
    {
        LCD12864_SendData(*chn);
        chn ++;
    }
}