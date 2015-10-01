#ifndef __LCD12864_H
#define __LCD12864_H

void LCD12864_SendData(unsigned char data);
void LCD12864_SendCommand(unsigned char comm);
void LCD12864_Init(void);
void LCD12864_Clear(void);
void LCD12864_Disp(unsigned char *chn);
void LCD12864_SetPosition(unsigned char x, unsigned char y);
void LCD12864_Char(unsigned char x, unsigned char y, unsigned char *chn);

#endif
