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
char des[BUFLEN];

SOCKET build(SOCKET sock,char *host,char *service)
{
  struct sockaddr_in sin;
  memset(&sin,0,sizeof(sin));
  sin.sin_family=AF_INET;
  sin.sin_addr.s_addr=inet_addr(host);
  sin.sin_port=htons((u_short)atoi(service));
  connect(sock,(struct sockaddr *)&sin,sizeof(sin));
  return sock;
}

void get_ip(char *buf,char *host,char *service)
{ 
  int x=0,len=strlen(buf),tot=0,sum=0,res=0,ip[10];
  for(;buf[x]!='(';x++);
  for(x++;buf[x]!=')';x++)
  {
    if(buf[x]==',')ip[tot++]=sum,sum=0;
    else sum=sum*10+buf[x]-'0';
  }
  sprintf(host,"%d.%d.%d.%d",ip[0],ip[1],ip[2],ip[3]);
  for(sum=ip[4]*256+sum;sum;sum/=10)
    service[res++]=sum%10+'0';
  service[res]='\0';
  for(int i=0,j=res-1;i<j;i++,j--)
    swap(service[i],service[j]);
}

unsigned __stdcall work(void *p)
{
  int cc;char buf[BUFLEN+5];
  FILE *fp=fopen(des,"wb");
  SOCKET sock=(*(SOCKET *)p);
  printf("开始下载文件...\n"); 
  while(1)
  {
    cc=recv(sock,buf,BUFLEN,0);
    if(!cc||cc==SOCKET_ERROR){
      if(!cc)printf("遗失对主机连接\n");
      else printf("Error: %d.\n",GetLastError());
      break;
    }
    buf[cc]='\0';fwrite(buf,1,cc,fp);
  }
  printf("下载结束\n"); 
}

int main(int argc, char *argv[])
{
  char host[BUFLEN],service[BUFLEN];
  char from[BUFLEN],str[BUFLEN],buf[BUFLEN];
  SOCKET sock,dsock;
  HANDLE hThread;
  WSADATA wsadata;
  int cc;
  printf("输入格式： 主机名，ftp文件地址，目标文件地址\n"); 
  //172.18.187.10 /ebook/ftp.pdf aaa.pdf
  scanf(" %s %s %s",host,from,des);
  strcpy(service,"21");
  
  printf("开始准备下载文件..\n"); 
  
  WSAStartup(WSVERS,&wsadata);
  sock=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
  sock=build(sock,host,service);
  
  recv(sock,buf,BUFLEN,0);
  printf("%s",buf);
  
  strcpy(str,"user abc\r\n");
  send(sock,str,strlen(str),0);
  cc=recv(sock,buf,BUFLEN,0);buf[cc]='\0';
  printf("%s%s",str,buf);
  strcpy(str,"pass 123666\r\n");
  send(sock,str,strlen(str),0);
  cc=recv(sock,buf,BUFLEN,0);buf[cc]='\0';
  printf("%s%s",str,buf);
  strcpy(str,"type I\r\n");
  send(sock,str,strlen(str),0);
  cc=recv(sock,buf,BUFLEN,0);buf[cc]='\0';
  printf("%s%s",str,buf);
  strcpy(str,"pasv\r\n");
  send(sock,str,strlen(str),0);
  cc=recv(sock,buf,BUFLEN,0);buf[cc]='\0';
  printf("%s%s",str,buf);
  get_ip(buf,host,service);
  sprintf(str,"retr %s\r\n",from);
  send(sock,str,strlen(str),0);
  cc=recv(sock,buf,BUFLEN,0);buf[cc]='\0';
  printf("%s%s",str,buf);
  
  dsock=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
  dsock=build(dsock,host,service);
  hThread=(HANDLE)_beginthreadex(NULL,0,(&work),(void*)&dsock,0,NULL);
  
  WaitForSingleObject(hThread,INFINITE);
  
  closesocket(sock);
  closesocket(dsock);
  WSACleanup();
  return 0;
}
