#ifndef __SYSIO_H
#define __SYSIO_H

#define MAX_SIZE 25*80
#define BUFLEN 100
int have_run,POS;
char X_pos,Y_pos;

void __set_POS(char x)
{
  POS=x;
}

void __set_HAVE_RUN(char x)
{
  have_run=x;
}

void gpos(char *x,char *y)
{
  *x=X_pos;
  *y=Y_pos;
}

void __move(char x,char y)
{
  asm mov ah,02h
  asm mov bh,0
  asm mov dh,x
  asm mov dl,y
  asm int 10h
  X_pos=x;Y_pos=y;
}

void __clear_screen(char a,char b,char c,char d)
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

void __rool_up(char a,char b,char c,char d)
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

void __rool_down(char a,char b,char c,char d)
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

void __putchar_no_format(char c)
{
  char x=X_pos,y=Y_pos;int pos;
  if(c>=32)
  {
    asm mov ah,09h
    asm mov al,c
    asm mov bl,0x07
    asm mov bh,0
    asm mov dh,x
	asm mov dl,y
    asm mov cx,1
    asm int 10h
  }
  if(c==13)pos=(x+1)*80;
  else pos=x*80+y+1;
  if(pos>=MAX_SIZE)pos=0;
  x=pos/80;y=pos%80;
  __move(x,y);
}

void __putchar_color(char c,char col)
{
  char x=X_pos,y=Y_pos;int pos;
  if(c>=32)
  {
    asm mov ah,09h
    asm mov al,c
    asm mov bl,col
    asm mov bh,0
    asm mov dh,x
	asm mov dl,y
    asm mov cx,1
    asm int 10h
  }
  if(c==13)pos=(x+1)*80;
  else if(c==8)pos=x*80+max(y-1,POS);
  else pos=x*80+y+1;
  if(pos>=MAX_SIZE)
  {
    if(have_run)__rool_down(0,40,22,79);
	__rool_up(0,0,24,79);
	__rool_down(23,59,23,79);
	pos-=80;
  }
  x=pos/80;y=pos%80;
  __move(x,y);
  if(c==8)
  {
    asm mov ah,09h
    asm mov al,' '
    asm mov bl,col
    asm mov bh,0
    asm mov dh,x
	asm mov dl,y
    asm mov cx,1
    asm int 10h
  }
}

#endif
