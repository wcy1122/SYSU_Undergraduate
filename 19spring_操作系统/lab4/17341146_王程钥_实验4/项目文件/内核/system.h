#ifndef __SYSTEM_H
#define __SYSTEM_H

void clear_screen(char a,char b,char c,char d)
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

void rool_up(char a,char b,char c,char d)
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

void rool_down(char a,char b,char c,char d)
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

void sys_exit()
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

#endif
