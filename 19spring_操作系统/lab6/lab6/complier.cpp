#include<bits/stdc++.h>
#define N 1440*1024
using namespace std;
char s[N*2],ch;int tot=0;
int size=6;
char sz[]="6";
char filename[][10]={"stone1","stone2","stone3","stone4","shell1","shell2"};
char type[][10]={"com","com","com","com","bat","bat"};
char sector[][10]={"37","38","39","40","41","42"};

int main()
{
  FILE *fp,*des;int len; 
  fp=fopen("guide.com","rb");
  while(fread(&ch,sizeof(char),1,fp))s[tot++]=ch;
  fclose(fp);
  
  len=strlen(sz);
  for(int i=0;i<len;i++)s[tot++]=sz[i];
  s[tot++]=0;
  for(int i=0;i<size;i++)
  { 
    len=strlen(filename[i]);
    for(int j=0;j<len;j++)s[tot++]=filename[i][j];
    s[tot++]=0;
	len=strlen(type[i]);
    for(int j=0;j<len;j++)s[tot++]=type[i][j];
    s[tot++]=0;
	len=strlen(sector[i]);
    for(int j=0;j<len;j++)s[tot++]=sector[i][j];
    s[tot++]=0;
  }
  if(tot>2*512)assert(0);
  tot=2*512;
  
  fp=fopen("test.com","rb");
  while(fread(&ch,sizeof(char),1,fp))s[tot++]=ch;
  fclose(fp);
  if(tot>36*512)assert(0);
  tot=36*512;
  

  
  fp=fopen("stone1.com","rb");
  while(fread(&ch,sizeof(char),1,fp))s[tot++]=ch;
  fclose(fp);
  fp=fopen("stone2.com","rb");
  while(fread(&ch,sizeof(char),1,fp))s[tot++]=ch;
  fclose(fp);
  fp=fopen("stone3.com","rb");
  while(fread(&ch,sizeof(char),1,fp))s[tot++]=ch;
  fclose(fp);
  fp=fopen("stone4.com","rb");
  while(fread(&ch,sizeof(char),1,fp))s[tot++]=ch;
  fclose(fp);
  
  fp=fopen("shell1.bat","rb");
  while(fread(&ch,sizeof(char),1,fp))s[tot++]=ch;
  fclose(fp);
  tot=41*512;
  
  fp=fopen("shell2.bat","rb");
  while(fread(&ch,sizeof(char),1,fp))s[tot++]=ch;
  fclose(fp);
  tot=42*512;
  
  des=fopen("test.flp","wb");
  fwrite(s,sizeof(char),N,des);
  fclose(des);
  return 0;
}
