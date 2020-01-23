#include<bits/stdc++.h>
#include<stdlib.h>
#include<stdio.h>
#include<winsock2.h>
#include<time.h>
#include"conio.h"
#define	BUFLEN	2000 
#define	WSVERS	MAKEWORD(2,0)
#pragma comment(lib,"ws2_32.lib")
using namespace std;
int main(int argc,char *argv[]) 
{
  struct	sockaddr_in fsin;
  SOCKET	msock,ssock;
  WSADATA	wsadata;
  char		service[]="50500";
  struct 	sockaddr_in sin;
  int	    alen;
  char		*pts;
  time_t	now;
  char 		str[BUFLEN],dst[BUFLEN];
  
  printf("输入接收文件夹：");
  scanf(" %s",dst);
  sprintf(des,".\\%s\\",dst);
  mkdir(dst);
  
  printf("等待连接...\n");
  WSAStartup(WSVERS, &wsadata);
  msock=socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  memset(&sin, 0, sizeof(sin));
  sin.sin_family=AF_INET;
  sin.sin_addr.s_addr=INADDR_ANY;
  sin.sin_port=htons((u_short)atoi(service));
  bind(msock,(struct sockaddr *)&sin,sizeof(sin));
  listen(msock,5);
  printf("连接成功！\r\n");
   
  while(1)
  {
    while(1)
    {
    alen=sizeof(struct sockaddr);
	ssock=accept(msock,(struct sockaddr *)&fsin,&alen);
	recv(ssock,str,sizeof(str),0);
	printf("正接收文件%s...\n",str);
	
	while(1)
	{
	  j
	}
	
    (void)closesocket(ssock);
  }
  
  printf("程序结束\n");
  (void)closesocket(msock);
  WSACleanup();
  system("pause");
  
  return 0;
}

