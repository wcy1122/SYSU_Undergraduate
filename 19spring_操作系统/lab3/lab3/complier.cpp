#include<bits/stdc++.h>
#define N 1440*1024
using namespace std;
char s[N*2],ch;int tot=0;
char sz[]="6";
char filename[][10]={"stone1","stone2","stone3","stone4","shell1","shell2"};
char type[][10]={"com","com","com","com","bat","bat"};
char sector[][10]={"13","14","15","16","17","18"};

int main()
{
  FILE *fp,*des;int len; 
  fp=fopen("guide.com","rb");
  while(fread(&ch,sizeof(char),1,fp))s[tot++]=ch;
  fclose(fp);
  
  fp=fopen("test.com","rb");
  while(fread(&ch,sizeof(char),1,fp))s[tot++]=ch;
  fclose(fp);
  if(tot>11*512)assert(0);
  tot=11*512;
  
  len=strlen(sz);
  for(int i=0;i<len;i++)s[tot++]=sz[i];
  s[tot++]=0;
  for(int i=0;i<6;i++)
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
  
  if(tot>12*512)assert(0);
  tot=12*512;
  
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
  
  fp=fopen("shell1.bat","rb");
  while(fread(&ch,sizeof(char),1,fp))s[tot++]=ch;
  fclose(fp);
  tot=17*512;
  
  fp=fopen("shell2.bat","rb");
  while(fread(&ch,sizeof(char),1,fp))s[tot++]=ch;
  fclose(fp);
  tot=18*512;
  
  
  des=fopen("test.flp","wb");
  fwrite(s,sizeof(char),N,des);
  fclose(des);
  return 0;
}
