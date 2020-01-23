#include<bits/stdc++.h> 
#include<stdlib.h>
#include<stdio.h>
#include<winsock2.h>
#include<time.h>
#include"conio.h"
#define	BUFLEN 200
#define QLEN 50
#define	WSVERS MAKEWORD(2, 0)
#pragma comment(lib,"ws2_32.lib") 
using namespace std;
SOCKET msock,ssock[QLEN];int tot;
struct info{SOCKET sock;sockaddr_in sin;int pos;};

void send_to_all(SOCKET sock,sockaddr_in sin,char *buf)
{
  char str1[BUFLEN],str2[BUFLEN];
  char str3[BUFLEN],str4[BUFLEN];
  char *pts;time_t now;
  time(&now);pts=ctime(&now);
  pts[strlen(pts)-1]='\0';
  sprintf(str1,"server time: %s\n",pts);
  sprintf(str2,"IP address: %s\n",inet_ntoa(sin.sin_addr));
  sprintf(str3,"IP port: %d\n",sin.sin_port);
  sprintf(str4,"message: %s\n\n",buf);
  
  printf("%s%s%s%s",str1,str2,str3,str4);
  
  for(int i=1;i<=tot;i++)
  {  
    send(ssock[i],str1,strlen(str1),0);
	send(ssock[i],str2,strlen(str2),0);
	send(ssock[i],str3,strlen(str3),0);
	send(ssock[i],str4,strlen(str4),0);
  }
}

unsigned __stdcall work(void *p)
{
  int cc;
  char buf[BUFLEN],str[BUFLEN];
  info tmp=(*(info *)p);
  sprintf(str,"client %d enter!",tmp.pos);
  send_to_all(tmp.sock,tmp.sin,str); 
  while(1)
  {
    int cc=recv(tmp.sock,buf,BUFLEN,0);
    if(!cc||cc==SOCKET_ERROR)break;
    buf[cc]='\0';send_to_all(tmp.sock,tmp.sin,buf);
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
  info		tmp;

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
	tmp=(info){ssock[tot],fsin,tot};
    hThread[tot]=(HANDLE)_beginthreadex(NULL,0,&work,(void*)&tmp,0,NULL);
  }
  
  for(int i=tot;i;i--)
    WaitForSingleObject(hThread[i],INFINITE);
  for(int i=1;i<=tot;i++)closesocket(ssock[i]);
  closesocket(msock);
  WSACleanup();
  return 0;
}

