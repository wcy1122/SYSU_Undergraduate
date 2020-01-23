#include<bits/stdc++.h> 
#include<stdlib.h>
#include<stdio.h>
#include<winsock2.h>
#include<time.h>
#include"conio.h"
#define	BUFLEN 2000
#define QLEN 50
#define	WSVERS MAKEWORD(2, 0)
#pragma comment(lib,"ws2_32.lib") 
using namespace std;
SOCKET msock,ssock[QLEN];int tot;

void send_to_all(SOCKET sock,char *buf)
{
  char *pts,str1[BUFLEN],str2[BUFLEN];
  time_t now;
  time(&now);pts=ctime(&now);
  pts[strlen(pts)-1]='\0';
  sprintf(str1,"server time: %s",pts);
  sprintf(str2,"message: %s\n",buf);
  printf("%s\n%s\n",str1,str2);
  for(int i=1;i<=tot;i++)
  {  
    send(ssock[i],str1,strlen(str1),0); 
	send(ssock[i],str2,strlen(str2),0);
  }
}

unsigned __stdcall work(void *p)
{
  int cc;
  char buf[BUFLEN];
  SOCKET sock=(*(SOCKET *)p);
  while(1)
  {
    int cc=recv(sock,buf,BUFLEN,0);
    if(!cc||cc==SOCKET_ERROR)break;
    buf[cc]='\0';send_to_all(sock,buf);
  }
}

int main(int argc, char *argv[])
{
  struct	sockaddr_in fsin;
  WSADATA	wsadata;
  char		service[]="50500",buf[BUFLEN];
  struct  	sockaddr_in sin;
  int	    alen;
  HANDLE 	hThread[QLEN];

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
	ssock[++tot]=accept(msock,(struct sockaddr *)&fsin,&alen);
	
    hThread[tot]=(HANDLE)_beginthreadex(NULL,0,&work,(void*)&ssock[tot],0,NULL);
  }
  
  for(int i=tot;i;i--)
    WaitForSingleObject(hThread[i],INFINITE);
  for(int i=1;i<=tot;i++)closesocket(ssock[i]);
  closesocket(msock);
  WSACleanup();
  return 0;
}

