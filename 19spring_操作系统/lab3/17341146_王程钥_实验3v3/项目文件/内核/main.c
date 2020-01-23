#include"stdio.h"
#include"string.h"
#include"clock.h"
#include"algo.h" 
#define FileSector 12
#define OffSetOfUserName 0x8000
#define OffSetOfUserSize 0x8400
#define OffSetOfUserSrc 0x8800
#define OffSetOfUserPrg 0xe100
char str[BUFLEN];

void run_com(char pos,int offset)
{
  int i;char p;
  if(!pos)return;
  p=(pos-1)/18;
  pos%=18;if(!pos)pos=18;
  have_run=1;
  asm push es
  asm push ds
  asm push si
  asm push di
  asm push bp
  
  asm mov ax,0h
  asm mov es,ax
  asm mov bx,offset
  asm mov cl,pos
  asm mov ah,2
  asm mov al,1
  asm mov dl,0
  asm mov dh,p
  asm mov ch,0
  asm int 13h
  
  asm mov ax,0
  asm mov es,ax
  asm mov ax,offset
  asm call es:ax
  
  asm pop bp
  asm pop di
  asm pop si
  asm pop ds
  asm pop es
  
  puts("successfully run!");
}

int find(char *sec,int tp)
{
  int i,pos=0,size;
  int addr=OffSetOfUserName;
  char sz[20],name[20],type[20],po[20],tw[20];
  if(!tp)strcpy(tw,"com");
  else strcpy(tw,"bat");
  fopen(addr,FileSector);
  addr=fscanf(addr,sz,'\000');
  size=string_to_int(sz);
  for(i=0;i<size;i++)
  {
    addr=fscanf(addr,name,'\000');
    addr=fscanf(addr,type,'\000');
	addr=fscanf(addr,po,'\000');
	pos=string_to_int(po);
    if(strcmp(sec,name)&&strcmp(tw,type))
	{
	  printf("program found in sector ");
      putint(pos);puts("!");
      return pos;
	}
  }
  puts("program not found");
  return 0;
}

int get_name(int p,char *str,char *sec)
{
  int i=p,len=0;
  for(;;i++)
  {
	if(str[i]==','||str[i]=='\000'){sec[len]='\000';break;}
	sec[len++]=str[i];
  }
  return i;
}


void work(char *str)
{
  int i,cur_pos,len=strlen(str);
  char sec[BUFLEN],buf[BUFLEN];
  strcpy(buf,str);
  for(i=2;i<len;i=cur_pos+1)
  {
    cur_pos=get_name(i,buf,sec);
    if(cur_pos<0)break;
	run_com(find(sec,0),OffSetOfUserPrg);
  }
}

void clear()
{
  asm mov ah,06h
  asm mov al,0
  asm mov ch,0
  asm mov cl,0
  asm mov dh,24
  asm mov dl,79
  asm mov bh,07h
  asm int 10h
  move(0,0);
  have_run=0;
}

int get_size(char pos)
{
  char tmp;
  int i,res=0,addr=OffSetOfUserSize;
  fopen(OffSetOfUserSize,pos);
  for(i=0;i<512;i++)
  {
	asm mov bx,addr
	asm mov al,[bx]
	asm mov tmp,al
	asm inc bx
	asm mov addr,bx
	if(tmp!='\000')res=i;
  }
  return res+1;
}

void ls()
{
  int i,size,spos,fsize,addr=OffSetOfUserName;
  puts("=====================================");
  print_len("filename",10);
  putchar('|');
  print_len("type",8);
  putchar('|');
  print_len("sector",8);
  putchar('|');
  print_len("filesize",8);
  putchar(13);
  
  fopen(addr,FileSector);
  addr=fscanf(addr,str,'\000');
  size=string_to_int(str);
  
  for(i=0;i<size;i++)
  {
    puts("-------------------------------------");
    addr=fscanf(addr,str,'\000');
	print_len(str,10);
    putchar('|');
	addr=fscanf(addr,str,'\000');
	print_len(str,8);
    putchar('|');
	addr=fscanf(addr,str,'\000');
    spos=string_to_int(str);
	putint_len(spos,8);
	putchar('|');
	fsize=get_size(spos);
	putint_len(fsize,8);
	putchar(13);
  }
  puts("=====================================");
}

int shell_work(char *command)
{
  if(strcmp_prefix(command,"./"))work(command);
  else if(strcmp(command,"ls"))ls();
  else if(strcmp(command,"cls"))clear();
  else if(strcmp(command,"time"))get_time();
  else if(strcmp(command,"end"))return 0;
  else puts("invalid command!");
  return 1;
}

void run_shell(int pos)
{
  int i,addr=OffSetOfUserSrc;
  char command[BUFLEN];
  fopen(addr,pos);
  while(1)
  {
    addr=fscanf(addr,command,';');
    if(!shell_work(command))break;
  }
  puts("successfully run a script program");
}


void shell(char *str)
{
  int i,curr_pos;
  char sec[BUFLEN],temp[BUFLEN];
  strcpy(temp,str);
  puts("");
  puts("--------------------");
  puts("running in shell mod");
  for(i=6;i<strlen(temp);i=curr_pos+1)
  {
    curr_pos=get_name(i,temp,sec);
	if(curr_pos<0)break;
	run_shell(find(sec,1));
	puts("");
  }
  puts("shell mod end");
  puts("--------------------");
  puts("");
} 

void help()
{
  puts("  'time'   --- get server time");
  puts("  'ls'     --- show infomation of user program");
  puts("  'cls'    --- clear the screen");
  puts("  'res'    --- restart wcy1122's OS");
  puts("  'time'   --- get server time");
  puts("  'exit'   --- exit");
  puts("  './<name>'       --- run a user program");
  puts("  './<f1>,<f2>'    --- run multiple program");
  puts("  'shell <name>'      --- run a script program");
  puts("  'shell <f1>,<f2>'   --- run multiple script program");
}

char kbhit()
{
  char c;
  asm mov ah,1
  asm int 16h
  asm mov c,al
  return c;
}

void putword()
{
  move(10,25);
  put_col("Welcome to wcy1222's OS",0x0a);
  move(11,25);
  put_col("Hello World!",0x0a);
  move(12,25);
  put_col("Press any key to continue.",0x0a);
}

int s[]={20,30,40,50,60,80};
void show()
{  
  char x=5,y=0,col=0x03,c='A',st,pre=-1;
  int p=0,xx=1;
  while(1)
  {
  	move(x,y);st=kbhit();
  	if(st==pre)break;pre=st;
    putchar_color(c,col);
    if(++y==s[p])p++,c++,xx=-xx;
	if(c>'Z')c='A';x+=xx;
    if(y==80)
	{
	  x=5;y=0;p=0;clear();putword();
	  if(++col==0x0c)col=0x03;
    }
    sleep(12000);
  }
}

void start()
{
  clear();
  putword();show();
  getchar();clear();
  puts("wcy1122's OS v1.0 by shell");
  puts("Input 'help' if you need");
}


int cmd()
{
  while(1)
  {
  	printf(">>");
  	gets(str);
    if(strcmp_prefix(str,"./"))work(str);
    else if(strcmp_prefix(str,"shell"))shell(str);
    else if(strcmp(str,"help"))help();
    else if(strcmp(str,"ls"))ls();
    else if(strcmp(str,"cls"))clear();
    else if(strcmp(str,"time"))get_time();
    else if(strcmp(str,"exit"))return 0;
    else if(strcmp(str,"res"))return 1;
	else puts("invalid command!");
  }
}

main()
{
  while(1)
  {
  	start();
	if(!cmd())break;
  }
  puts("Bye, see you again!");
  return;
}
