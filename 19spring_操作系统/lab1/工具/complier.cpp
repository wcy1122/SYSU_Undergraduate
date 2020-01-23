#include<bits/stdc++.h>
#define N 1440*1024
using namespace std;
char s[N*2],ch;int tot=0;
int main()
{
  FILE *f1,*f2;
  f1=fopen("test.com","rb");
  f2=fopen("test.flp","wb");
  while(fread(&ch,sizeof(char),1,f1))s[tot++]=ch;
  fwrite(s,sizeof(char),N,f2);
  fclose(f1);fclose(f2);
  return 0;
}
