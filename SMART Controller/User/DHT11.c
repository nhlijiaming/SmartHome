#include "stm32f10x.h"
#include "delay.h"
#include "DHT11.h"
#include <stdio.h>

u8 tdata[4]={0x00,0x00,0x00,0x00};
u8 sbuf,check;

#define DHT11_Out_Set GPIO_SetBits(GPIOA, GPIO_Pin_1)
#define DHT11_Out_Reset GPIO_ResetBits(GPIOA, GPIO_Pin_1)
#define DHT11_In GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)

extern uint8_t DataString[20];

void DHT11_PortIN(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	//Configure pin as input 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 	//浮动输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
void DHT11_PortOUT(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	//Configure pin as input 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

unsigned char StartDHT11(void)
{
    DHT11_PortOUT();
    DHT11_Out_Reset;
    delay_ms(18);  //触发开始,总线拉低要大于18ms
    
    DHT11_Out_Set;	//释放总线
    delay_us(25);  //wait 20-40uS 等待DHT11的低电平响应信号
    
    DHT11_PortIN();
    if(!DHT11_In)
    {
        while(!DHT11_In);//低电平的响应信号，80us
        delay_us(25);
        while(DHT11_In);//紧接着是80us的高电平数据准备信号
        //拉低后DHT11会拉高总线80us,接着会开始传数据	
        return 1;   
    }
    return 0;
}

void com(void)
{
	u8 i;
	for(i = 0;i<8;i++)
	{
		sbuf <<= 1;
		delay_us(20);//接收到响应后会出现50us的低电平表示发送数据的开始，所以这里小延时一下
		while(!DHT11_In);//等到高电平的出现，高电平的时间表示的是数据位的0和1
		delay_us(30);//数据为0的信号时间为26-28us，1则为70us，这里超时检测
//		tt = 100;
//		while(tt++);
		if(DHT11_In)//如果还为高
		{
            sbuf |= 0x01;
            delay_us(30);//这里的延时足够了，40+10+30>70了
		}
		else //如果变低
		{
			sbuf &= 0xfe;
		}
	}
}

u8 ReadDHT11(void)
{
	u8 sum;
	if(StartDHT11())	//判断是否已经响应
	{
		com();
		tdata[0]=sbuf;
		com();
		tdata[1]=sbuf;
		com();
		tdata[2]=sbuf;		
		com();
		tdata[3]=sbuf;
		com();
		check = sbuf;
		sum = (tdata[0]+tdata[1]+tdata[2]+tdata[3]);
		//sum = sum - 0x80;
	}
	if(check == sum)
        return(1);
	else	
        return 0;
}