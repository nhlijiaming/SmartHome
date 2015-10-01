#include <stm32f10x.h>
#include "GSM.h"
#include "Resolver.h"

unsigned char GSM_ReceiveBuff[3][70];
unsigned char GSM_ReceiveBuffUpdated;

unsigned char GSM_InitialCommand[][30]={
"AT+CMGF=1",			// TXTģʽ
"AT+CSCA=\"+8613010670500\"",	// д��Ϣ���ĺ���
"AT+CSCS=\"GSM\"", 			// �ַ���
"AT+CSMP=17,167,0,0",     		//���ö���Ϣ�ı�ģʽ����
"AT+CMGS=\"+8618666363845\""    //���ն��ŵ绰����
};

//AT+CMGF=1
//AT+CSCA="+8613010670500"
//AT+CSCS="GSM"
//AT+CSMP=17,167,0,0


void init_GSM()
{
    
    
}

extern volatile unsigned char GSM_Refresh;
extern volatile unsigned char GSM_BuffCount;
extern volatile unsigned char GSM_Buff[10][100];
void ReceiveSMS()
{
    static unsigned char Receive_Flag=0;
    static unsigned char lastCount=0;
    
    while (lastCount != GSM_BuffCount)
    {
        if (Receive_Flag)
        {
            Receive_Flag = 0;
            Resolver(GSM_Buff[lastCount]);
        }
        if (GSM_Buff[lastCount][0]=='+' && GSM_Buff[lastCount][1]=='C' && GSM_Buff[lastCount][2]=='M' && GSM_Buff[lastCount][3]=='T')
        {
            Receive_Flag = 1;
        }
        lastCount ++;
        if (lastCount >= 10) lastCount = 0;
    }
}