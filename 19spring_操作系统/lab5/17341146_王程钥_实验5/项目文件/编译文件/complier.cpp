#include<bits/stdc++.h>
#define N 1440*1024
using namespace std;
char s[N*2],ch;int tot=0;
int size=6;
char sz[]="6";
char filename[][10]={"stone1","stone2","stone3","stone4","shell1","shell2"};
char type[][10]={"com","com","com","com","bat","bat"};
char sector[][10]={"20","21","22","23","24","25"};

int main()
{
  FILE *fp,*des;int len; 
  fp=fopen("guide.com","rb");
  while(fread(&ch,sizeof(char),1,fp))s[tot++]=ch;
  fclose(fp);
  
  fp=fopen("test.com","rb");
  while(fread(&ch,sizeof(char),1,fp))s[tot++]=ch;
  fclose(fp);
  if(tot>18*512)assert(0);
  tot=18*512;
  
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
  if(tot>19*512)assert(0);
  tot=19*512;
  
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
  tot=24*512;
  
  fp=fopen("shell2.bat","rb");
  while(fread(&ch,sizeof(char),1,fp))s[tot++]=ch;
  fclose(fp);
  tot=25*512;
  
  fp=fopen("newint8.com","rb");
  while(fread(&ch,sizeof(char),1,fp))s[tot++]=ch;
  fclose(fp);
  tot=26*512;
  
  fp=fopen("newint9.com","rb");
  while(fread(&ch,sizeof(char),1,fp))s[tot++]=ch;
  fclose(fp);
  tot=27*512;
  
  fp=fopen("sys.com","rb");
  while(fread(&ch,sizeof(char),1,fp))s[tot++]=ch;
  fclose(fp);
  if(tot>31*512)assert(0);
  tot=31*512;
  
  des=fopen("test.flp","wb");
  fwrite(s,sizeof(char),N,des);
  fclose(des);
  return 0;
}
