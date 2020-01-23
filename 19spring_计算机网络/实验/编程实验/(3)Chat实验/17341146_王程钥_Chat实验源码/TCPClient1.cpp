#include<bits/stdc++.h> 
#include<stdlib.h>
#include<stdio.h>
#include<winsock2.h>
#include<string.h>
#define	BUFLEN 2000
#define WSVERS MAKEWORD(2, 0)
#pragma comment(lib,"ws2_32.lib")
using namespace std;
unsigned __stdcall work(void *p)
{
  int cc;char buf[BUFLEN];
  SOCKET sock=(*(SOCKET *)p);
  while(1)
  {
    cc=recv(sock,buf,BUFLEN,0);
    if(!cc||cc==SOCKET_ERROR){
      printf("Error: %d.\n",GetLastError());
      break;
    }
    buf[cc]='\0';printf("%s",buf);
  }
}

int main(int argc, char *argv[])
{
  char		host[]="127.0.0.1";
  char		service[]="50500";
  struct  	sockaddr_in sin;
  char 		str[BUFLEN];
  SOCKET	sock;
  HANDLE 	hThread;
  WSADATA 	wsadata;
  
  WSAStartup(WSVERS,&wsadata);
  sock=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);

  memset(&sin,0,sizeof(sin));
  sin.sin_family=AF_INET;
  sin.sin_addr.s_addr=inet_addr(host);
  sin.sin_port=htons((u_short)atoi(service));
  connect(sock,(struct sockaddr *)&sin,sizeof(sin));
  
  hThread=(HANDLE)_beginthreadex(NULL,0,(&work),(void*)&sock,0,NULL);
  while(~scanf(" %s",str)){
  	if(!strcmp(str,"exit"))break;
    send(sock,str,strlen(str),0);
    printf("send: %s\n",str); 
  }
  
  closesocket(sock);
  WSACleanup();
  system("pause");
  return 0;
}
