#include<bits/stdc++.h>
#include<stdlib.h>
#include<stdio.h>
#include<winsock2.h>
#include<string.h>
#define	BUFLEN	2000
#define WSVERS	MAKEWORD(2,0)
#pragma comment(lib,"ws2_32.lib")
using namespace std;
void error(int cc){
  if(!cc)printf("Server closed!");
  else printf("Error: %d.\n",GetLastError());
}
bool check(int cc){
  return cc>0&&cc!=SOCKET_ERROR;
}

int main(int argc,char *argv[])
{
  char		host[]="127.0.0.1";
  char		service[]="50500";
  struct	sockaddr_in sin;
  char		buf[BUFLEN+1];
  SOCKET	sock;
  int		cc;
  char 		str[BUFLEN];
  WSADATA wsadata;
  WSAStartup(WSVERS,&wsadata);
  sock=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);

  memset(&sin,0,sizeof(sin));
  sin.sin_family=AF_INET;
  sin.sin_addr.s_addr=inet_addr(host); 
  sin.sin_port=htons((u_short)atoi(service));
  int ret=connect(sock,(struct sockaddr *)&sin,sizeof(sin));
    cout<<ret<<endl;
  printf("����Ҫ���͵���Ϣ��");
  scanf(" %s",str); 
  send(sock,str,strlen(str),0);
  cc=recv(sock,buf,BUFLEN,0);
  if(check(cc))
  {
    printf("\n�յ�����Ϣ��\n");
    buf[cc]='\0',printf("���ݣ�%s\n",buf);
    cc=recv(sock,buf,BUFLEN,0);
    buf[cc]='\0',printf("ʱ�䣺%s",buf);
    cc=recv(sock,buf,BUFLEN,0);
    buf[cc]='\0',printf("�ͻ���IP��ַ��%s\n",buf);
    cc=recv(sock,buf,BUFLEN,0);
    buf[cc]='\0',printf("�ͻ��˶˿ںţ�%s\n",buf);
  }
  else error(cc);
  
  closesocket(sock);
  WSACleanup();
  printf("\n���س�������...");
  getchar();getchar();
	
  return 0; 
}
