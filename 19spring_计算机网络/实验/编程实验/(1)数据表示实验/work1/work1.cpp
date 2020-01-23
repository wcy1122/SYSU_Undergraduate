#include<bits/stdc++.h>
#include<windows.h> 
#define N 10000
#define BUF_LEN 100
#define USER_NAME_LEN 20
#define EMAIL_LEN 80
#define TIME_BUF_LEN 30
using namespace std;
typedef unsigned long DWORD;
struct Person {
  char username[USER_NAME_LEN];
  int level;
  char email[EMAIL_LEN];
  DWORD sendtime;
  time_t regtime;
}per[N]; 
char pts[TIME_BUF_LEN];
time_t now;

bool input_one_person(Person &personSent)
{
  char inputBuf[BUF_LEN];
  int inputNumber;
  char buf[sizeof(Person)];
  printf("username: ");
  if(!(~scanf("%s",inputBuf)))return 0;
  strcpy(personSent.username,inputBuf);
  printf("level: ");
  scanf("%d",&inputNumber);
  personSent.level=inputNumber;
  printf("email: ");
  scanf("%s",inputBuf);
  strcpy(personSent.email,inputBuf);
  now=time(NULL);
  personSent.sendtime=(DWORD)now;
  personSent.regtime=now;
  return 1;
}

void put_into_file(Person personSent)
{
  FILE *fp;
  if((fp=fopen("Persons.stru","ab+"))==NULL) {
    printf("can't open the file");
    getchar();exit(0);
  }
  if(fwrite(&personSent,sizeof(Person),1,fp)!=1){
    printf("file write error\n");
	printf("Press any key to continue...");
	getchar();
  }
  fclose(fp);
}

void output_one_person(Person personRecv)
{
  char regtime[TIME_BUF_LEN];
  char sendtime[TIME_BUF_LEN];
  printf("用户名： %s\n",personRecv.username);
  printf("级别：%d\n",personRecv.level);
  printf("Email地址：%s\n",personRecv.email);
  time_t t1=(time_t)personRecv.sendtime;
  sprintf(sendtime,"%s",ctime(&t1));
  printf("发送时间：%s",sendtime);
  sprintf(regtime,"%s",ctime(&personRecv.regtime));
  printf("注册时间：%s",regtime);
  printf("\n");
}

void input()
{
  Person personSent;
  printf("Hint: Press ctrl+z in username to stop\n");
  while(input_one_person(personSent))
	put_into_file(personSent),printf("\n");
  printf("\n");
}

void output()
{
  Person personRecv;FILE *fp;
  char regtime[TIME_BUF_LEN];
  char sendtime[TIME_BUF_LEN];
  if((fp=fopen("Persons.stru","rb"))==NULL)
  {
    printf("can't open the file");
    printf("Press any key to continue...");
	getchar();exit(0);
  }
  while(fread(&personRecv,sizeof(Person),1,fp)==1)
  	output_one_person(personRecv);
  fclose(fp);
  printf("\r\nSuccessful output, Press any key to continue...");
  getchar();
}

int main()
{
  input();
  output();
  return 0;
}
