#include"stdio.h"
#include"string.h"
#include"clock.h"
#include"algo.h" 
#include"system.h"
#define FileSector 19
#define OffSetOfUserName 0x8000
#define OffSetOfUserSize 0x8400
#define OffSetOfUserSrc 0x8800
#define OffSetOfUserPrg 0xe100
char str[BUFLEN];
int cs9,ip9,cs8,ip8,OUCH,TIME;

void load_int(char pos,int Intvec,int offset,char size)
{
  int i;char citou,zhumian,shanqu;
  int pre=Intvec,cs_8,ip_8,cs_9,ip_9;
  zhumian=pos/36;
  citou=(pos/18)&1;
  shanqu=pos%18;
  if(!shanqu)shanqu=18;
  asm push es    
  asm mov ax,0h
  asm mov es,ax
  asm mov bx,offset
  asm mov ah,2
  asm mov al,size
  asm mov dl,0
  asm mov dh,citou
  asm mov ch,zhumian
  asm mov cl,shanqu
  asm int 13h
  asm pop es
  
  asm push ds
  asm mov ax,0
  asm mov ds,ax
  Intvec*=4;
  asm mov bx,Intvec
  if(pre==8){
  	asm mov ax,[bx]
    asm mov ip_8,ax
  }
  if(pre==9){
  	asm mov ax,[bx]
    asm mov ip_9,ax
  }
  asm mov ax,offset
  asm mov [bx],ax
  Intvec+=2;
  asm mov bx,Intvec
  if(pre==8){
  	asm mov ax,[bx]
    asm mov cs_8,ax
  }
  if(pre==9){
  	asm mov ax,[bx]
    asm mov cs_9,ax
  }
  asm mov ax,0
  asm mov [bx],ax
  asm pop ds
  if(pre==8)cs8=cs_8,ip8=ip_8;
  if(pre==9)cs9=cs_9,ip9=ip_9;
}

void reload_int(int vec,int Intcs,int Intip)
{
  asm push ds
  asm mov ax,0
  asm mov ds,ax
  vec*=4;
  asm mov bx,vec
  asm mov ax,Intip
  asm mov [bx],ax
  vec+=2;
  asm mov bx,vec
  asm mov ax,Intcs
  asm mov [bx],ax
  asm pop ds
}

#define OffSetOfNewInt8h 0x9500
#define OffSetOfNewInt9h 0x9900
#define OffSetOfNewInt21h 0xc500
void load()
{
  load_int(28,0x21,OffSetOfNewInt21h,4);
}

void set_time()
{
  if(!TIME)load_int(26,0x08,OffSetOfNewInt8h,1);
  else reload_int(0x08,cs8,ip8),clear_screen(24,59,24,79);
  TIME^=1;
}

void set_ouch()
{
  OUCH^=1;
  if(!OUCH)puts("OUCH mod off");
  else puts("OUCH mod on");
}

void run_com(char pos,int offset)
{
  int i;char citou,zhumian,shanqu;
  if(!pos)return;
  if(OUCH)load_int(27,0x09,OffSetOfNewInt9h,1);
  zhumian=pos/36;
  citou=(pos/18)&1;
  shanqu=pos%18;
  if(!shanqu)shanqu=18;
  
  have_run=1;
  asm push es
  asm push ds
  asm push si
  asm push di
  asm push bp
  asm push ax
  asm push bx
  asm push cx
  asm push dx
  
  asm mov ax,0h
  asm mov es,ax
  asm mov bx,offset
  asm mov ah,2
  asm mov al,1
  asm mov dl,0
  asm mov dh,citou
  asm mov ch,zhumian
  asm mov cl,shanqu
  asm int 13h
  
  asm mov ax,0
  asm mov es,ax
  asm mov ax,offset
  asm call es:ax
  
  asm pop dx
  asm pop cx
  asm pop bx
  asm pop ax
  asm pop bp
  asm pop di
  asm pop si
  asm pop ds
  asm pop es
  
  puts("successfully run!");
  if(OUCH)reload_int(0x09,cs9,ip9);
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
  clear_screen(0,0,24,79);
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
  puts("  'ls'     --- show infomation of user program");
  puts("  'cls'    --- clear the screen");
  puts("  'res'    --- restart wcy1122's OS");
  puts("  'time'   --- show/hide system time");
  puts("  'ouch'   --- ouch mod on/off");
  puts("  'system' --- system setting");
  puts("  'exit'   --- exit");
  puts("  './<name>'       --- run a user program");
  puts("  './<f1>,<f2>'    --- run multiple program");
  puts("  'shell <name>'      --- run a script program");
  puts("  'shell <f1>,<f2>'   --- run multiple script program");
}

int dec_to_bcd(int num)
{
  char res=0,s[BUFLEN],len=1;
  while(num)s[++len]=num%16,num/=16;
  while(len)res=res*16+s[len--];
  return res;
}

void system_call()
{
  char str[BUFLEN],op,X,Y;
  clear_screen(0,40,15,79);have_run=1;
  puts("  'ouch'   --- show ouch");
  puts("  'poem'   --- read a poem");
  puts("  'music'  --- listen to music");
  puts("  'cls'    --- clear the screen");
  puts("  'exit'   --- exit system work");
  while(1)
  {
    printf("system>>");POS=8;
	gets(str);
    if(strcmp(str,"exit"))break;
    if(strcmp(str,"ouch"))op=0;
    else if(strcmp(str,"poem"))op=1;
    else if(strcmp(str,"music"))op=2;
    else if(strcmp(str,"cls")){
	  clear_screen(0,40,15,79);continue;
	}
    else{puts("Invalid command!");continue;}
    X=x;Y=y;
    asm mov ah,op
	asm int 21h
	move(X,Y);
  }
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
  puts("wcy1122's OS v2.0 by shell");
  puts("Input 'help' if you need");
}

int cmd()
{
  while(1)
  {
  	printf(">>");POS=2;
  	gets(str);
  	if(!strlen(str))continue;
    if(strcmp_prefix(str,"./"))work(str);
    else if(strcmp_prefix(str,"shell"))shell(str);
    else if(strcmp(str,"time"))set_time();
    else if(strcmp(str,"ouch"))set_ouch();
    else if(strcmp(str,"help"))help();
    else if(strcmp(str,"ls"))ls();
    else if(strcmp(str,"cls"))clear();
    else if(strcmp(str,"system"))system_call();
    else if(strcmp(str,"exit"))return 0;
    else if(strcmp(str,"res"))return 1;
	else puts("invalid command!");
  }
}

main()
{
  load();
  while(1)
  {
  	start();
	if(!cmd())break;
  }
  puts("Bye, hope to see you again!");
  puts("Press any key to exit");
  getchar();sys_exit();
  return;
}
