#include<bits/stdc++.h> 
#include<stdlib.h>
#include<stdio.h>
#include<winsock2.h>
#include<ws2tcpip.h>
#include<string.h>
#define	BUFLEN 100
#define WSVERS MAKEWORD(2, 0)
#pragma comment(lib,"ws2_32.lib")
using namespace std;
unsigned __stdcall work(void *p)
{
  int cc;char buf[BUFLEN+5];
  SOCKET sock=(*(SOCKET *)p);
  while(1)
  {
    cc=recv(sock,buf,BUFLEN,0);
    if(!cc||cc==SOCKET_ERROR){
      if(!cc)printf("\n遗失对主机连接\n");
      else printf("Error: %d.\n",GetLastError());
      break;
    }
    buf[cc]='\0';printf("%s",buf);
  }
}

int main(int argc, char *argv[])
{
  char		host_name[BUFLEN];
  char		service[BUFLEN];
  struct  	sockaddr_in sin;
  char 		str[BUFLEN],host[BUFLEN];
  SOCKET	sock;
  HANDLE 	hThread;
  WSADATA 	wsadata;
  
  while(1)
  { 
    printf(">>");
    
    scanf(" %s %s",host_name,service);getchar();
    WSAStartup(WSVERS,&wsadata);
    sock=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
    
    if('0'<=host_name[0]&&host_name[0]<='9')
	{
	  strcpy(host,host_name);
    }
    else
	{
	  hostent*  hostp=gethostbyname(host_name);
      sockaddr_in in;
      memcpy(&in.sin_addr,hostp->h_addr,sizeof(hostp->h_addr));
      sprintf(host,inet_ntoa(in.sin_addr));
    }
  
    memset(&sin,0,sizeof(sin));
    sin.sin_family=AF_INET;
    sin.sin_addr.s_addr=inet_addr(host);
    sin.sin_port=htons((u_short)atoi(service));
    connect(sock,(struct sockaddr *)&sin,sizeof(sin));
  
    hThread=(HANDLE)_beginthreadex(NULL,0,(&work),(void*)&sock,0,NULL);
    
    while(1)
    {
	  int len=0;
      gets(str);len=strlen(str);
      str[len++]='\r';str[len++]='\n';str[len]='\0';
	  send(sock,str,strlen(str),0);
    }
    
    closesocket(sock);
    WSACleanup();
  }
  return 0;
}
