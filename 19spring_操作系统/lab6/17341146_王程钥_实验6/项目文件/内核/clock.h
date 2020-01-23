#ifndef __CLOCK_H
#define __CLOCK_H

#include"stdio.h"
int TIME,Time_wait=4,now=0;

void sleep(int cnt)
{
  int i,j,sum=0;
  for(i=1;i<=cnt;i++)
    for(j=1;j<=10000;j++)sum++,sum--;
}

void Show_Wheel()
{
  Time_wait--;
  if(Time_wait)return;
  Time_wait=4;
  if(now==0)putchar_no_format('|');
  if(now==1)putchar_no_format('/');
  if(now==2)putchar_no_format('-');
  if(now==3)putchar_no_format(92);
  now=(now+1)%4;
} 

void bcd_to_ascii(unsigned char tmp)
{
  unsigned char A,B;
  asm push ax
  asm mov ah,tmp
  asm mov al,tmp
  asm and al,0x0f
  asm add al,0x30
  asm shr ah,4
  asm and ah,0x0f
  asm add ah,0x30
  asm mov A,ah
  asm mov B,al
  asm pop ax
  putchar_no_format(A);
  putchar_no_format(B);
}

void Show_Time()
{
  char Yh,Yl,M,D,H,S;
  
  asm mov ah,04h
  asm int 1ah
  
  asm mov Yh,ch
  asm mov Yl,cl
  asm mov M,dh
  asm mov D,dl
  bcd_to_ascii(Yh);
  bcd_to_ascii(Yl);
  putchar_no_format('/');
  bcd_to_ascii(M);
  putchar_no_format('/');
  bcd_to_ascii(D);
  putchar_no_format(' ');
  
  asm mov ah,02h
  asm int 1ah
  
  asm mov H,ch
  asm mov M,cl
  asm mov S,dh
  bcd_to_ascii(H);
  putchar_no_format(':'); 
  bcd_to_ascii(M);
  putchar_no_format(':');
  bcd_to_ascii(S);
}

void Show_Int8()
{
  int X=x,Y=y;
  move(24,59);
  Show_Time();
  putchar_no_format(' ');
  Show_Wheel();
  move(X,Y);
}

#endif
