#include"stdio.h"
#include"string.h"
#include"clock.h"
#include"algo.h" 
#include"system.h"
#include"syswork.h"
#include"pcb.h"
#define FileSector 2
#define OffSetOfUserName 0x8000
#define OffSetOfUserSize 0x8400
#define OffSetOfUserSrc 0x8800

char str[BUFLEN];

int load_com(char pos,char size,int segment,int offset)
{
  int i;char citou,zhumian,shanqu;
  if(!pos)return 0;
  
  zhumian=pos/36;
  citou=(pos/18)&1;
  shanqu=pos%18;
  if(!shanqu)shanqu=18;
  
  asm push es
  asm mov ax,segment
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
  
  return 1;
}

int find(char *sec,char *tw,int *pos,int *size)
{
  int i,sec_pos,sec_size,tot_size;
  int addr=OffSetOfUserName;
  char sz[20],name[20],type[20],po[20];
  fopen(addr,FileSector);
  addr=fscanf(addr,sz,'\000');
  tot_size=string_to_int(sz);
  for(i=0;i<tot_size;i++)
  {
    addr=fscanf(addr,name,'\000');
    addr=fscanf(addr,type,'\000');
	addr=fscanf(addr,po,'\000');
	sec_pos=string_to_int(po);
	addr=fscanf(addr,sz,'\000');
	sec_size=string_to_int(sz);
    if(strcmp(sec,name)&&strcmp(tw,type))
	{
	  printf(name);putchar('.');printf(type);
	  printf(" found in sector ");
      putint(sec_pos);puts("!");
      *pos=sec_pos;*size=sec_size;
      return 1;
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

void fake_call(int CS,int IP)
{
  int tmp_cs=CS,tmp_ip=IP;
  asm mov cx,ds					/*要返回到内核！*/
  asm push ds
  asm mov ax,tmp_cs
  asm mov ds,ax
  asm mov bx,tmp_ip 
  asm sub bx,2
  asm mov [bx],cx				/*返回段地址进栈*/
  asm sub bx,2
  asm mov ax,offset com_ret
  asm mov [bx],ax				/*返回偏移量进栈*/
  asm pop ds
}
/*模拟 call*/

void fake_ret()
{
  asm com_ret:
  asm mov ax,0a00h
  asm mov ds,ax
  asm mov es,ax
  Make_Die(Cur_Pro);
  asm jmp $  
}
/*模拟ret后的处理*/

void run(char *str)
{
  int i,cur_pos,pos,size,id,len=strlen(str);
  int CS,IP,tag;
  char sec[BUFLEN],buf[BUFLEN];
  strcpy(buf,str);
  for(i=4;i<len;i=cur_pos+1)
  {
    cur_pos=get_name(i,buf,sec);
    if(cur_pos<0)break;
	tag=find(sec,"com",&pos,&size);
	if(!tag)continue;
	id=Find_Pro(sec);
    if(id>=0)
	{
	  if(Is_Pause(id))Make_Alive(id);
	  else puts("this program is already running");
	  continue;
	}
	id=Find_Empty();
	if(id<0){puts("There is no free flace");continue;}
	CS=DataSegment+id*BASE;
	IP=OffSetOfUserPrg;
    init(id,sec,CS,IP);
    fake_call(CS,IP);
    Make_Alive(id);
    load_com(pos,size,CS,IP);
    set_HAVE_RUN(1);have_run=1;
  }
}

void stop(char *str)
{
  int i,cur_pos,len=strlen(str),id;
  char sec[BUFLEN],buf[BUFLEN];
  strcpy(buf,str);
  for(i=5;i<len;i=cur_pos+1)
  {
    cur_pos=get_name(i,buf,sec);
    if(cur_pos<0)break;
    id=Find_Pro(sec);
    if(id<=0)
    {
      if(!id)puts("you are not allowed to stop kernal!");
	  else puts("this program is not running");
	  continue;
	}
	Kill_Pro(id,0);
  }
}

void pause(char *str)
{
  int i,cur_pos,len=strlen(str),id;
  char sec[BUFLEN],buf[BUFLEN];
  strcpy(buf,str);
  for(i=6;i<len;i=cur_pos+1)
  {
    cur_pos=get_name(i,buf,sec);
    if(cur_pos<0)break;
    id=Find_Pro(sec);
    if(id<=0)
    {
      if(!id)puts("you are not allowed to stop kernal!");
	  else puts("this program is not running");
	  continue;
	}
	Make_Pause(id);
  }
}

void set_time()
{
  if(TIME)clear_screen(24,59,24,79); 
  TIME^=1;
}

void clear()
{
  clear_screen(0,0,24,79);
  move(0,0);
  if(Pro_Empty())have_run=0,set_HAVE_RUN(0);
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
  int i,j,size,addr=OffSetOfUserName;
  int sec_pos,sec_size,file_size;
  puts("=====================================");
  print_len("filename",10);
  putchar('|');
  print_len("type",8);
  putchar('|');
  print_len("sector",8);
  putchar('|');
  print_len("filesize",8);
  putchar(13);
  puts("-------------------------------------");
  
  fopen(addr,FileSector);
  addr=fscanf(addr,str,'\000');
  size=string_to_int(str);
  
  for(i=0;i<size;i++)
  {
    addr=fscanf(addr,str,'\000');
	print_len(str,10);
    putchar('|');
    
	addr=fscanf(addr,str,'\000');
	print_len(str,8);
    putchar('|');
    
	addr=fscanf(addr,str,'\000');
    sec_pos=string_to_int(str);
	putint_len(sec_pos,8);
	putchar('|');
	
    addr=fscanf(addr,str,'\000');
    sec_size=string_to_int(str);
	file_size=512*(sec_size-1);
	file_size+=get_size(sec_pos+sec_size-1);
	putint_len(file_size,8);
	
	putchar(13);
  }
  puts("=====================================");
}

void Show_Pro()
{
  int i;
  puts("=============================");
  print_len("id",7);putchar('|');
  print_len("name",10);putchar('|');
  print_len("state",10);puts("");
  puts("-----------------------------");
  for(i=0;i<Pro_Size;i++)
  {
    if(Pro_List[i].state==DIE)continue;
	putint_len(i,7);putchar('|');
    print_len(Pro_List[i].name,10);putchar('|');
	if(Pro_List[i].state==BLOCK)print_len("pause",10);
	else print_len("run",10);puts(""); 
  }
  puts("=============================");
}

int shell_work(char *command)
{
  if(strcmp_prefix(command,"run"))run(command);
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
  int i,curr_pos,pos,size,tag;
  char sec[BUFLEN],temp[BUFLEN];
  strcpy(temp,str);
  puts("");
  puts("--------------------");
  puts("running in shell mod");
  for(i=6;i<strlen(temp);i=curr_pos+1)
  {
    curr_pos=get_name(i,temp,sec);
	if(curr_pos<0)break;
	tag=find(sec,"bat",&pos,&size);
	if(tag)run_shell(pos),puts("");
  }
  puts("shell mod end");
  puts("--------------------");
  puts("");
} 

void help()
{
  puts(" 'ls'     --- show info of program");
  puts(" 'show'   --- show info of process");
  puts(" 'cls'    --- clear the screen");
  puts(" 'time'   --- show/hide time");
  puts(" 'system' --- test system call");
  puts(" 'res'    --- restart wcy1122's OS");
  puts(" 'exit'   --- exit");
  puts(" 'run <name>'      --- run a program");
  puts(" 'run <f1>,<f2>'   --- run mul program");
  puts(" 'stop <name>'     --- stop a program");
  puts(" 'stop <f1>,<f2>'  --- stop mul program");
  puts(" 'pause <name>'    --- pause a program");
  puts(" 'pause <f1>,<f2>' --- pause mul program");
  puts(" 'shell <name>'    --- run a script");
  puts(" 'shell <f1>,<f2>' --- run mul script");
  
}

void system_call()
{
  char str[BUFLEN],op,x,y;
  clear_screen(16,40,21,79);
  set_HAVE_RUN(1);have_run=1;
  puts("  'ouch'   --- show ouch");
  puts("  'poem'   --- read a poem");
  puts("  'music'  --- listen to music");
  puts("  'cls'    --- clear the screen");
  puts("  'exit'   --- exit system work");
  while(1)
  {
    printf("system>>");set_POS(8);POS=8;
	gets(str);
    if(strcmp(str,"exit"))break;
    if(strcmp(str,"ouch"))op=0x10;
    else if(strcmp(str,"poem"))op=0x11;
    else if(strcmp(str,"music"))op=0x12;
    else if(strcmp(str,"cls")){
	  clear_screen(16,40,21,79);continue;
	}
    else{puts("Invalid command!");continue;}
    get_pos(&x,&y); 
    asm mov ah,op
	asm int 21h
	move(x,y);
  }
}

char kbhit()
{
  char c;
  asm mov ah,01h
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
  char x=5,y=0,col=0x03,c='A',st,pre=0;
  int p=0,xx=1;
  while(1)
  {
  	move(x,y);st=kbhit();
  	if(pre&&st!=pre)break;pre=st;
	putchar_color(c,col);
    if(++y==s[p])p++,c++,xx=-xx;
	if(c>'Z')c='A';x+=xx;
    if(y==80)
	{
	  x=5;y=0;p=0;clear();putword();
	  if(++col==0x0c)col=0x03;
    }
    sleep(3000);
  }
}

void start()
{
  clear();putword();show();getchar();clear();
  puts("This is wcy1122's OS v5.0");
  puts("Input 'help' if you need");
}

int cmd()
{
  while(1)
  {
  	printf(">>");set_POS(2);POS=2;
  	gets(str);
  	if(!strlen(str))continue;
    if(strcmp_prefix(str,"run"))run(str);
    else if(strcmp_prefix(str,"pause"))pause(str);
	else if(strcmp_prefix(str,"stop"))stop(str);
    else if(strcmp_prefix(str,"shell"))shell(str);
    else if(strcmp(str,"help"))help();
    else if(strcmp(str,"ls"))ls();
    else if(strcmp(str,"show"))Show_Pro();
    else if(strcmp(str,"cls"))clear();
    else if(strcmp(str,"time"))set_time();
    else if(strcmp(str,"system"))system_call();
    else if(strcmp(str,"exit"))return 0;
    else if(strcmp(str,"res"))return 1;
	else puts("invalid command!");
  }
}

main()
{
  while(1)
  {
  	Pro_Clear();
  	if(TIME)set_time();
	start();
	if(!cmd())break;
  }
  puts("Bye, hope to see you again!");
  puts("Press any key to exit");
  getchar();sys_exit();
  return;
}
