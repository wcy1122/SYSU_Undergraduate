#include<bits/stdc++.h>
#include<stdlib.h>
#include<stdio.h>
#include<winsock2.h>
#include<string.h>
#include"conio.h"
#define	BUFLEN 2000
#define WSVERS MAKEWORD(2,2)
#pragma comment(lib,"ws2_32.lib")
using namespace std;
void get_ip(char *s,in_addr p){
  int a=p.S_un.S_un_b.s_b1;
  int b=p.S_un.S_un_b.s_b2;
  int c=p.S_un.S_un_b.s_b3;
  int d=p.S_un.S_un_b.s_b4;
  sprintf(s,"%d.%d.%d.%d",a,b,c,d);
}
void error(int cc){
  if(cc)printf("recvfrom() failed; %d\n",WSAGetLastError());
}
bool check(int cc){
  return cc>0&&cc!=SOCKET_ERROR;
}

int main(int argc, char *argv[])
{
  char		host[]="127.0.0.1";
  char		service[]="50500";
  struct	sockaddr_in sin,from;
  int		fromsize=sizeof(from),cc;
  char   	buf[BUFLEN],ip[BUFLEN],port[BUFLEN],*pts;
  time_t	now;
  SOCKET	sock;

  WSADATA wsadata;
  WSAStartup(WSVERS, &wsadata);		
  sock = socket(PF_INET, SOCK_DGRAM,IPPROTO_UDP);
  memset(&sin, 0, sizeof(sin));
  sin.sin_family=AF_INET;
  sin.sin_addr.s_addr=INADDR_ANY;
  sin.sin_port=htons((u_short)atoi(service));
  bind(sock,(struct sockaddr *)&sin,sizeof(sin));
  printf("服务器已启动！\n");
   
  while(!_kbhit())
  {
	cc=recvfrom(sock,buf,BUFLEN,0,(SOCKADDR *)&from,&fromsize);
    if(!check(cc)){error(cc);break;}
    buf[cc] = '\0';
	printf("\n客户端的消息：%s\n",buf);
	sendto(sock,buf,BUFLEN,0,(SOCKADDR *)&from,sizeof(from));
	get_ip(ip,from.sin_addr);
	printf("客户端IP地址：%s\n",ip);
	sendto(sock,ip,BUFLEN,0,(SOCKADDR *)&from,sizeof(from));
	printf("客户端端口号：%d\n",from.sin_port);
	sprintf(port,"%d",from.sin_port);
	sendto(sock,port,BUFLEN,0,(SOCKADDR *)&from,sizeof(from));	
	(void)time(&now);
	pts=ctime(&now);
	printf("时间：%s",pts);
	sendto(sock,pts,BUFLEN,0,(SOCKADDR *)&from,sizeof(from));
  }
  
  closesocket(sock);
  WSACleanup();
  return 0;
}
