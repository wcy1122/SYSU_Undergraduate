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
  char 		str[BUFLEN];

  WSAStartup(WSVERS, &wsadata);
  msock=socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  
  memset(&sin, 0, sizeof(sin));
  sin.sin_family=AF_INET;
  sin.sin_addr.s_addr=INADDR_ANY;
  sin.sin_port=htons((u_short)atoi(service));
  bind(msock,(struct sockaddr *)&sin,sizeof(sin));
  listen(msock,5);
  printf("��������������\n\n");
   
  while(!_kbhit())
  {
    alen=sizeof(struct sockaddr);
	ssock=accept(msock,(struct sockaddr *)&fsin,&alen);
	recv(ssock,str,sizeof(str),0);
	printf("�յ���Ϣ��%s\n",str);
    (void)time(&now);
    pts=ctime(&now);
    //int cc=send(ssock,pts,strlen(pts),0);
    printf("�յ�ʱ�䣺%s\n",pts);
    //send(ssock,str,strlen(str),0); 
    (void)closesocket(ssock);
  }
    
  (void)closesocket(msock);
  WSACleanup();
  printf("���س�������...");
  getchar();getchar();
  
  return 0;
}

