#ifndef __SYSTEM_H
#define __SYSTEM_H

void clear_screen(char a,char b,char c,char d)
{
  asm mov ah,03h
  asm mov ch,a
  asm mov cl,b
  asm mov dh,c
  asm mov dl,d
  asm int 21h
}

void rool_up(char a,char b,char c,char d)
{
  asm mov ah,04h
  asm mov ch,a
  asm mov cl,b
  asm mov dh,c
  asm mov dl,d
  asm int 21h
}

void rool_down(char a,char b,char c,char d)
{
  asm mov ah,05h
  asm mov ch,a
  asm mov cl,b
  asm mov dh,c
  asm mov dl,d
  asm int 21h
}

void sys_exit()
{
  asm mov ah,0x4c;
  asm int 21h;
}

#endif
