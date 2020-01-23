#include<bits/stdc++.h>
#define N 1440*1024
using namespace std;
char s[N*2],ch;int tot=0;
int size=7;
char sz[]="7";
char filename[][10]={"stone1","stone2","stone3","stone4","shell1","shell2","testint"};
char type[][10]={"com","com","com","com","bat","bat","com"};
char sector[][10]={"14","15","16","17","18","19","26"};

int main()
{
  FILE *fp,*des;int len; 
  fp=fopen("guide.com","rb");
  while(fread(&ch,sizeof(char),1,fp))s[tot++]=ch;
  fclose(fp);
  
  fp=fopen("test.com","rb");
  while(fread(&ch,sizeof(char),1,fp))s[tot++]=ch;
  fclose(fp);
  
  if(tot>12*512)assert(0);
  tot=12*512;
  
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
  if(tot>13*512)assert(0);
  tot=13*512;
  
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
  
  fp=fopen("shell1.txt","rb");
  while(fread(&ch,sizeof(char),1,fp))s[tot++]=ch;
  fclose(fp);
  tot=18*512;
  
  fp=fopen("shell2.txt","rb");
  while(fread(&ch,sizeof(char),1,fp))s[tot++]=ch;
  fclose(fp);
  tot=19*512;
  
  fp=fopen("newint8.com","rb");
  while(fread(&ch,sizeof(char),1,fp))s[tot++]=ch;
  fclose(fp);
  tot=20*512;
  
  fp=fopen("newint9.com","rb");
  while(fread(&ch,sizeof(char),1,fp))s[tot++]=ch;
  fclose(fp);
  tot=21*512;
  
  fp=fopen("newint33.com","rb");
  while(fread(&ch,sizeof(char),1,fp))s[tot++]=ch;
  fclose(fp);
  tot=22*512;
  
  fp=fopen("newint34.com","rb");
  while(fread(&ch,sizeof(char),1,fp))s[tot++]=ch;
  fclose(fp);
  tot=23*512;
  
  fp=fopen("newint35.com","rb");
  while(fread(&ch,sizeof(char),1,fp))s[tot++]=ch;
  fclose(fp);
  tot=24*512;
  
  fp=fopen("newint36.com","rb");
  while(fread(&ch,sizeof(char),1,fp))s[tot++]=ch;
  fclose(fp);
  tot=25*512;
  
  fp=fopen("testint.com","rb");
  while(fread(&ch,sizeof(char),1,fp))s[tot++]=ch;
  fclose(fp);
  tot=26*512;
  
  
  des=fopen("test.flp","wb");
  fwrite(s,sizeof(char),N,des);
  fclose(des);
  return 0;
}
