#include<bits/stdc++.h>
#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <time.h>
#define	BUFLEN		2000
#define WSVERS		MAKEWORD(2, 2)
#pragma comment(lib,"ws2_32.lib")
using namespace std;
int main(int argc, char *argv[])
{
  char		host[]="127.0.0.1";
  char		service[]="50500";
  struct 	sockaddr_in toAddr;
  char		buf[BUFLEN+1];
  SOCKET	sock;
  int 		tosize=sizeof(toAddr),cc;
  time_t	now;

  WSADATA wsadata;
  WSAStartup(WSVERS, &wsadata);
  sock=socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP);
  memset(&toAddr,0,sizeof(toAddr));
  toAddr.sin_family=AF_INET;
  toAddr.sin_port=htons((u_short)atoi(service));
  toAddr.sin_addr.s_addr=inet_addr(host);

  printf("输入消息：");
  scanf(" %s",buf);printf("\n\n"); 
  cc=sendto(sock,buf,BUFLEN,0,(SOCKADDR *)&toAddr,sizeof(toAddr));
  if(cc==SOCKET_ERROR)printf("发送失败，错误号：%d\n", WSAGetLastError());
  else printf("发送成功!\r\n");

  recvfrom(sock,buf,BUFLEN,0,(SOCKADDR *)&toAddr,&tosize);
  printf("客户端的消息：%s\n",buf);
  recvfrom(sock,buf,BUFLEN,0,(SOCKADDR *)&toAddr,&tosize);
  printf("客户端IP地址：%s\n",buf);
  recvfrom(sock,buf,BUFLEN,0,(SOCKADDR *)&toAddr,&tosize);
  printf("客户端端口号：%s\n",buf);
  recvfrom(sock,buf,BUFLEN,0,(SOCKADDR *)&toAddr,&tosize);
  printf("时间：%s\n",buf);
  closesocket(sock);
  WSACleanup();

  printf("按任意键退出...");
  getchar();getchar();
  return 0; 
}
