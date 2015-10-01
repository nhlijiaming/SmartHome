/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
 */
u32 TimingDelay;
void SysTick_Handler(void)
{
    if (TimingDelay != 0x00)
    { 
        TimingDelay--;
    }
}
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 
extern volatile unsigned char SS_Flag;

void EXTI9_5_IRQHandler()
{
    SS_Flag = 1;
    EXTI_ClearITPendingBit(EXTI_Line9);
}

void EXTI15_10_IRQHandler()
{
    SS_Flag = 1;
    EXTI_ClearITPendingBit(EXTI_Line13);
}
      
extern volatile unsigned char Hr, Min, Sec;
extern volatile unsigned char INT;
void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearFlag(TIM2,TIM_IT_Update);//进入中断先清除更新标志
        Sec ++;
        if (Sec>=60) {Min ++; Sec = 0;}
        if (Min>=60) {Hr ++; Min = 0;}
        if (Hr >=24) Hr = 0;
        INT ++;
    }
}

extern volatile unsigned char USART_Refresh;
extern volatile unsigned char USART_Buff[40];
void USART1_IRQHandler(void)
{
    static char count = 0;
    unsigned char temp;
    
    if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==SET)
    {
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
        temp = USART_ReceiveData(USART1);
        if (temp == '$')
        {
            count = 0;
        }
        else
        {
            if (count<35) count ++;
            if (temp == '@') USART_Refresh = 1;
        }
        USART_Buff[count] = temp;
    }
}

extern volatile unsigned char GSM_Refresh;
extern volatile unsigned char GSM_BuffCount;
extern volatile unsigned char GSM_Buff[10][100];
void USART2_IRQHandler(void)
{
    static char count = 0;
    unsigned char temp;
    
    if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE)==SET)
    {
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
        temp = USART_ReceiveData(USART2);
        GSM_Buff[GSM_BuffCount][count] = temp;
        if (count<95) count ++;
        if (temp == 0x0A)
        {
            GSM_Refresh = 1;
            GSM_BuffCount ++;
            if (GSM_BuffCount>=10) GSM_BuffCount = 0;
            count = 0;
        }
    }
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
