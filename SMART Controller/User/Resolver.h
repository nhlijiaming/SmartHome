#ifndef __RESOLVER_H
#define __RESOLVER_H

#define Relay1_Reset GPIO_SetBits(GPIOA, GPIO_Pin_6)
#define Relay1_Set GPIO_ResetBits(GPIOA, GPIO_Pin_6)
#define Relay2_Reset GPIO_SetBits(GPIOA, GPIO_Pin_4)
#define Relay2_Set GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define Relay3_Reset GPIO_SetBits(GPIOB, GPIO_Pin_0)
#define Relay3_Set GPIO_ResetBits(GPIOB, GPIO_Pin_0)
#define Relay4_Reset GPIO_SetBits(GPIOA, GPIO_Pin_0)
#define Relay4_Set GPIO_ResetBits(GPIOA, GPIO_Pin_0)

void Measure_GI(void);

void Measure_LI(void);

void Measure_TH(void);

void Measure_SS(void);

void Resolver(volatile unsigned char Comm[]);

#endif
