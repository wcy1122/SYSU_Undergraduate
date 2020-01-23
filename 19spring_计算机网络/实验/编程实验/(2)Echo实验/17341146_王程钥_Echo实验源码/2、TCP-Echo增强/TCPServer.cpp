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
void get_ip(char *s,in_addr p){
  int a=p.S_un.S_un_b.s_b1;
  int b=p.S_un.S_un_b.s_b2;
  int c=p.S_un.S_un_b.s_b3;
  int d=p.S_un.S_un_b.s_b4;
  sprintf(s,"%d.%d.%d.%d",a,b,c,d);
}

int main(int argc,char *argv[]) 
{
  struct sockaddr_in	fsin;
  struct sockaddr_in	sin;
  SOCKET	msock,ssock;
  WSADATA	wsadata;
  char		service[]="50500";
  int	    alen,cc;
  time_t	now;
  char 		str[BUFLEN],port[BUFLEN],ip[BUFLEN],*pts;

  WSAStartup(WSVERS, &wsadata);
  msock=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
  
  memset(&sin,0,sizeof(sin));
  sin.sin_family=AF_INET;
  sin.sin_addr.s_addr=INADDR_ANY;
  sin.sin_port=htons((u_short)atoi(service));
  bind(msock,(struct sockaddr *)&sin,sizeof(sin));
  listen(msock,5);
  printf("服务器已启动！\n");
  
  while(!_kbhit())
  {
    alen=sizeof(struct sockaddr);
	ssock=accept(msock,(struct sockaddr *)&fsin,&alen);
	recv(ssock,str,sizeof(str),0);
	printf("\n收到消息：%s\n",str);
	send(ssock,str,strlen(str),0);
    (void)time(&now);
    pts=ctime(&now);
    printf("收到时间：%s",pts);
	send(ssock,pts,strlen(pts),0);
    get_ip(ip,fsin.sin_addr);
    printf("客户端IP地址：%s\n",ip);
    send(ssock,ip,strlen(ip),0);
	printf("客户端端口号：%d\n",fsin.sin_port);
	sprintf(port,"%d",fsin.sin_port);
	send(ssock,port,strlen(port),0);
	(void)closesocket(ssock);
  }
    
  (void)closesocket(msock);
  WSACleanup();
  printf("按回车键继续...");
  getchar();getchar();
  
  return 0;
}

