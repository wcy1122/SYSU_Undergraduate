#ifndef __SYSWORK_H
#define __SYSWORK_H

#include"stdio.h"
#include"clock.h"
short Ax,Bx,Cx,Dx;
char Ah,Al,Bh,Bl,Ch,Cl,Dh,Dl;
int xx,yy;

void print_format(char *s)
{
  int i;char X=x,Y=y;
  puts_col(s,0x0F);move(X,Y);
  for(i=0;s[i]!='\000';i++)
  {
    if('A'<=s[i]&&s[i]<='Z')putchar_color(s[i],0x0c);
    else putchar_color(s[i],0x0e);
    sleep(2000);
  }
}

void int_sys_exit()
{
  asm mov ax,5301h
  asm xor bx,bx
  asm int 15h
  asm mov ax,530Eh
  asm xor bx,bx
  asm mov cx,102h
  asm int 15h
  asm mov ax,5307h
  asm mov bx,1
  asm mov cx,3
  asm int 15h
}

void int_clear_screen(char a,char b,char c,char d)
{
  asm mov ah,06h
  asm mov al,0
  asm mov ch,a
  asm mov cl,b
  asm mov dh,c
  asm mov dl,d
  asm mov bh,07h
  asm int 10h
}

void int_rool_up(char a,char b,char c,char d)
{
  asm mov ah,06h
  asm mov al,1
  asm mov bh,07h
  asm mov ch,a
  asm mov cl,b
  asm mov dh,c
  asm mov dl,d
  asm int 10h
}

void int_rool_down(char a,char b,char c,char d)
{
  asm mov ah,07h
  asm mov al,1
  asm mov bh,07h
  asm mov ch,a
  asm mov cl,b
  asm mov dh,c
  asm mov dl,d
  asm int 10h
}

void print_poem()
{
  clear_screen(16,40,21,79);
  move(16,50);puts_col("Deng He Que Lou",0x0d);
  move(17,55);puts_col("Meng Hao Ran",0x0d);
  move(18,50);print_format("Bai Ri Yi Shan Jin,");
  move(19,50);print_format("Huang He Ru Hai Liu.");
  move(20,50);print_format("Yu Qiong Qian Li Mu,");
  move(21,50);print_format("Geng Shang Yi Ceng Lou.");
}

void print_song()
{
  clear_screen(16,40,21,79);
  move(16,50);puts_col("Hou Lai",0x0d);
  move(17,60);puts_col("Liu Ruo Ying",0x0d);
  move(19,40);print_format("Hou Lai");
  move(20,50);print_format("Wo Zong Suan Xue Hui Le");
  move(21,40);print_format("Ru He Qu Ai");
  clear_screen(19,40,21,79);
  move(19,40);print_format("Ke Xi Ni Zao Yi Jing");
  move(20,50);print_format("Xiao Shi Zai Ren Hai");
  clear_screen(19,40,21,79);
  move(19,40);print_format("Hou Lai");
  move(20,50);print_format("Zhong Yu Zai Yan Lei Zhong");
  move(21,40);print_format("Ming Bai");
  clear_screen(19,40,21,79);
  move(19,40);print_format("You Xie Ren");
  move(20,50);print_format("Yi Dan Cuo Guo Jiu Bu Zai.");
}

void show_ouch()
{
  move(xx+16,yy+40);
  puts_col("OUCH!",0x0F);
  xx=(xx*23+rand())%5;
  yy=(yy*23+rand())%31;
}

int rand()
{
  int res;
  asm mov ah,02h
  asm int 1ah
  asm mov res,dx
  return res;
}

int21h_main()
{
  Ah=Ax/256;Al=Ax%256;
  Bh=Bx/256;Bl=Bx%256;
  Ch=Cx/256;Cl=Cx%256;
  Dh=Dx/256;Dl=Dx%256;
  if(Ah==0x00)show_ouch(); 
  if(Ah==0x01)print_poem();
  if(Ah==0x02)print_song();
  if(Ah==0x03)int_clear_screen(Ch,Cl,Dh,Dl);
  if(Ah==0x04)int_rool_up(Ch,Cl,Dh,Dl);
  if(Ah==0x05)int_rool_down(Ch,Cl,Dh,Dl);
  if(Ah==0x4c)int_sys_exit();
}

#endif
