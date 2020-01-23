#define __CLOCK_H
#define __CLOCK_H

void sleep(int cnt)
{
  int i,j;
  for(i=1;i<=cnt;i++)
    for(j=1;j<=10000;j++);
}

int hex_dec(int x)
{
  int i=0,res=0;char ch[5];
  for(;x;x/=16)ch[i++]=x%16;
  for(i--;i>=0;i--)res=res*10+ch[i];
  return res;
}

void get_time()
{
  char Yh,Yl,M,D,h,m,s;
  asm mov ah,04h
  asm int 1ah
  asm mov Yh,ch
  asm mov Yl,cl
  asm mov M,dh
  asm mov D,dl
  putint(hex_dec(Yh));
  putint(hex_dec(Yl));
  putchar(':');
  putint(hex_dec(M));
  putchar(':');
  putint(hex_dec(D));
  putchar(' ');
  asm mov ah,02h
  asm int 1ah
  asm mov h,ch
  asm mov m,cl
  asm mov s,dh
  putint(hex_dec(h));
  putchar(':');
  putint(hex_dec(m));
  putchar(':');
  putint(hex_dec(s));
  puts(13);
}
