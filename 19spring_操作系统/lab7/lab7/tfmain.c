#include"system.h"
#define BUFLEN 10

char str[80]="129djwqhdsajd128dw9i39ie93i8494urjoiew98kdkd";
int LetterNr=0;

void putint(unsigned int x)
{
  int len=0;char s[BUFLEN];
  if(!x){putchar_color('0',0x07);return;}
  for(;x;x/=10)s[len++]=x%10+48;
  for(len--;len>=0;len--)
    putchar_color(s[len],0x07);
} 

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
/*IO part*/ 

int CountLetter(char *s)
{
  int i,ans=0;
  for(i=0;s[i]!='\000';i++)
  {
    if('a'<=s[i]&&s[i]<='z')ans++;
    if('A'<=s[i]&&s[i]<='Z')ans++;
  }
  return ans;
}

test_main()
{
  int pid=fork(),ch;
  if(pid==-1)puts("error in fork!");
  else if(pid)
  {
  	puts("I'm father!!");
	printf("Ch_Id= ");putint(pid);puts("");
	ch=wait();
	puts("I'm father again!!");
	printf("Ret_Val= ");putint(ch);puts("");
	printf("Fa_LetterNr= ");putint(LetterNr);puts("");
  } 
  else
  {
  	puts("I'm child!!");
    LetterNr=CountLetter(str);
    printf("Ch_LetterNr= ");putint(LetterNr);puts("");
  }
  exit(0);
}
