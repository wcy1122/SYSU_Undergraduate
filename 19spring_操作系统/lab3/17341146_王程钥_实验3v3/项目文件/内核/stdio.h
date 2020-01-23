#ifndef __STDIO_H
#define __STDIO_H

#include"string.h"
#include"algo.h" 
#define MAX_SIZE 25*80
#define SIZE 30
#define BUFLEN 100
char x,y,str[BUFLEN];int have_run;

void move(char a,char b)
{
  asm mov ah,02h
  asm mov bh,0
  asm mov dh,a
  asm mov dl,b
  asm int 10h
  x=a;y=b;
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

void putchar(char c)
{
  int pos;
  if(c>=32)
  {
    asm mov ah,0ah
    asm mov al,c
    asm mov bh,0
    asm mov dh,x
	asm mov dl,y
    asm mov cx,1
    asm int 10h
  }
  if(c==13)pos=(x+1)*80;
  else if(c==8)pos=x*80+max(y-1,2);
  else pos=x*80+y+1;
  if(pos>=MAX_SIZE)
  {
    if(have_run)rool_down(0,40,15,79);
	rool_up(0,0,24,79);
	pos-=80;
  }
  x=pos/80;y=pos%80;
  move(x,y);
  if(c==8)
  {
    asm mov ah,0ah
    asm mov al,' '
    asm mov bh,0
    asm mov dh,x
	asm mov dl,y
    asm mov cx,1
    asm int 10h
  }
}

void putchar_color(char c,char col)
{
  int pos;
  if(c>=32)
  {
    asm mov ah,09h
    asm mov al,c
    asm mov bh,0
    asm mov bl,col
    asm mov dh,x
	asm mov dl,y
    asm mov cx,1
    asm int 10h
  }
  if(c==13)pos=(x+1)*80;
  else pos=x*80+y+1;
  x=pos/80;y=pos%80;
  move(x,y);
}

void put_col(char *s,char col)
{
  int i,len=strlen(s);
  for(i=0;i<len;i++)putchar_color(s[i],col);
}

void puts(char *s)
{
  int i,len=strlen(s);
  move(x,y); 
  for(i=0;i<len;i++)putchar(s[i]);
  putchar(13);
}

void printf(char *s)
{
  int i,len=strlen(s);
  for(i=0;i<len;i++)putchar(s[i]);
}

void putint(int x)
{
  int len=0;char tmp,str[BUFLEN];
  if(!x){putchar('0');return;}
  while(x)str[len++]=x%10+48,x/=10;
  for(len--;len>=0;len--)putchar(str[len]);
} 

void print_len(char *s,int le)
{
  int i,len=strlen(s),sz=(le-len)/2;
  for(i=0;i<sz;i++)putchar(' ');
  for(i=0;i<len&&i<le;i++)putchar(s[i]);
  for(i=len+sz;i<le;i++)putchar(' ');
}

void putint_len(int x,int le)
{
  int i,j,len=0;char tmp;
  while(x)str[len++]=x%10+48,x/=10;
  str[len]='\000';
  for(i=0,j=len-1;i<j;i++,j--)
    tmp=str[i],str[i]=str[j],str[j]=tmp;
  print_len(str,le);
}

char getchar()
{
  char c,d;
  asm mov ah,0;
  asm int 16h;
  asm mov c,al;
  asm mov d,ah;
  if(!c)return -d;
  putchar(c);
  return c;
}

char mem[SIZE][BUFLEN];
int sum_cmd;
void gets(char *s)
{
  int i,len=0,p=0,po=sum_cmd;char c=getchar();
  for(;c!=13;c=getchar())
  {
  	if(c==-72){
	  if(!po)continue;
  	  move(x,2);
  	  for(i=0;i<len;i++)putchar(' ');
  	  move(x,2);
	  printf(mem[--po]);
	  strcpy(s,mem[po]);
  	  len=y-2;p=len;
	}
	else if(c==-80){
	  if(po>=sum_cmd-1)continue;
  	  move(x,2);
  	  for(i=0;i<len;i++)putchar(' ');
  	  move(x,2);
	  printf(mem[++po]);
	  strcpy(s,mem[po]);
  	  len=y-2;p=len;
	}
	else if(c==-75){
	  if(p)--y,--p,move(x,y);
	}
  	else if(c==-77){
	  if(p<len)++y,++p,move(x,y);
	}
    else if(c==8){
	  len=max(len-1,0);p=max(p-1,0);
	  for(i=p;i<len;i++)
	    s[i]=s[i+1],putchar(s[i]);
	  putchar(' ');
	  move(x,p+2);
	}
    else{
      for(i=p;i<len;i++)putchar(s[i]);
      for(i=len;i>p;i--)s[i]=s[i-1];
	  ++len;s[p++]=c;
	  move(x,p+2);
    }
  }
  s[len]='\000';strcpy(mem[sum_cmd],s);
  if(++sum_cmd>=SIZE)
  {
    for(i=0;i<SIZE-1;i++)strcpy(mem[i],mem[i+1]);
    sum_cmd--;
  }
}

void fopen(int addr,char pos)
{
  asm mov bx,addr
  asm mov ah,2
  asm mov al,1
  asm mov dl,0
  asm mov dh,0
  asm mov ch,0
  asm mov cl,pos
  asm int 13h
}

int fscanf(int addr,char *s,char end)
{
  int i,len=0,flag=0;char tmp;
  while(1)
  {
	asm mov bx,addr
	asm mov al,[bx]
	asm mov tmp,al
	asm inc bx
	asm mov addr,bx
	if(tmp==end)break; 
	if(tmp<=32&&!flag)continue;
	flag=1;s[len++]=tmp;
  }
  s[len]='\000';
  return addr;
}

#endif
