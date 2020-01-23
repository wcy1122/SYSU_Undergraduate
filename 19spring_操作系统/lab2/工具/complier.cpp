#include<bits/stdc++.h>
#define N 1440*1024
using namespace std;
char s[N*2],ch;int tot=0;
int main()
{
  FILE *fp,*des;
  fp=fopen("test.com","rb");
  while(fread(&ch,sizeof(char),1,fp))s[tot++]=ch;
  fclose(fp);
 
  fp=fopen("a.com","rb");
  while(fread(&ch,sizeof(char),1,fp))s[tot++]=ch;
  fclose(fp);
  
  fp=fopen("b.com","rb");
  while(fread(&ch,sizeof(char),1,fp))s[tot++]=ch;
  fclose(fp);
  
  fp=fopen("c.com","rb");
  while(fread(&ch,sizeof(char),1,fp))s[tot++]=ch;
  fclose(fp);
  
  fp=fopen("d.com","rb");
  while(fread(&ch,sizeof(char),1,fp))s[tot++]=ch;
  fclose(fp);
  
  des=fopen("test.flp","wb");
  fwrite(s,sizeof(char),N,des);
  fclose(des);
  return 0;
}
