#include"system.h"
#define BUFLEN 10

void puts(char *s)
{
  int i;
  for(i=0;s[i]!='\000';i++)putchar_color(s[i],0x07);
  putchar_color(13,0x07);
}

void printf(char *s)
{
  int i;
  for(i=0;s[i]!='\000';i++)putchar_color(s[i],0x07);
}

void putint(unsigned int x)
{
  int len=0;char s[BUFLEN];
  if(!x){putchar_color('0',0x07);return;}
  for(;x;x/=10)s[len++]=x%10+48;
  for(len--;len>=0;len--)
    putchar_color(s[len],0x07);
} 
/*IO part*/ 

test_main()
{
  int id1,id2;id1=fork();id2=fork();
  if(id1&&id2)
  {
    puts("I have two son!");
    printf("Son_Id= ");putint(id1);puts("");
    printf("Son_Id= ");putint(id2);puts("");
    wait();
  }
  else if(id1||id2)
  {
    puts("I have a son!");
    printf("Son_Id= ");
	if(id1)putint(id1);
	else putint(id2);
    puts("");wait();
  }
  else
  {
    puts("I want a son qaq");
  }
  exit(0);
}
