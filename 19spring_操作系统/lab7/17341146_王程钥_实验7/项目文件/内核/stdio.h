#ifndef __STDIO_H
#define __STDIO_H

#include"string.h"
#include"algo.h"
#include"system.h"
#define MAX_SIZE 25*80
#define BUFLEN 100
#define SIZE 30 
char str[BUFLEN];int POS;

void putchar(char c)
{
  putchar_color(c,0x07);
}

void put_col(char *s,char col)
{
  int i,len=strlen(s);
  for(i=0;i<len;i++)putchar_color(s[i],col);
}

void puts_col(char *s,char col)
{
  int i;
  for(i=0;s[i]!='\000';i++)putchar_color(s[i],col);
  putchar_color(13,col);
}

void puts(char *s)
{
  int i,len=strlen(s);char x,y;
  for(i=0;i<len;i++)putchar(s[i]);
  putchar(13);
}

void printf(char *s)
{
  int i,len=strlen(s);
  for(i=0;i<len;i++)putchar(s[i]);
}

void putint(unsigned int x)
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
  if(!x)str[len++]='0';
  else while(x)str[len++]=x%10+48,x/=10;
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
  int i,len=0,p=0,po=sum_cmd;
  char x,y,c=getchar();
  for(;c!=13;c=getchar())
  {
  	get_pos(&x,&y);
  	if(c==-72){
	  if(!po)continue;
  	  move(x,POS);
  	  for(i=0;i<len;i++)putchar(' ');
  	  move(x,POS);
	  printf(mem[--po]);
	  strcpy(s,mem[po]);
	  get_pos(&x,&y);
  	  len=y-POS;p=len;
	}
	else if(c==-80){
	  if(po>=sum_cmd-1)continue;
  	  move(x,POS);
  	  for(i=0;i<len;i++)putchar(' ');
  	  move(x,POS);
	  printf(mem[++po]);
	  strcpy(s,mem[po]);
	  get_pos(&x,&y);
  	  len=y-POS;p=len;
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
	  move(x,p+POS);
	}
    else{
      for(i=p;i<len;i++)putchar(s[i]);
      for(i=len;i>p;i--)s[i]=s[i-1];
	  ++len;s[p++]=c;
	  move(x,p+POS);
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
  char citou,zhumian,shanqu;
  zhumian=pos/36;
  citou=(pos/18)&1;
  shanqu=pos%18;
  if(!shanqu)shanqu=18;
  
  asm push es
  asm mov bx,addr
  asm mov ah,2
  asm mov al,1
  asm mov dl,0
  asm mov dh,citou
  asm mov ch,zhumian
  asm mov cl,shanqu
  asm int 13h
  asm pop es
}

int fscanf(int addr,char *s,char end)
{
  int i,len=0,flag=0;char tmp;
  asm push ax
  asm push bx
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
  asm pop bx
  asm pop ax
  return addr;
}

#endif
