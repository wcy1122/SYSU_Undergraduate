#ifndef __PCB_H
#define __PCB_H

extern void Show_Time();

#include"stdio.h"
#include"clock.h"
#include"string.h"
#define BUFLEN 100
#define us unsigned short
#define DIE   0
#define BLOCK 1
#define READY 2
#define RUN   3

us ax_save,bx_save,cx_save,dx_save,si_save,di_save,bp_save;
us es_save,ds_save,ss_save,sp_save,ip_save,cs_save,flags_save;
int TIME,Cur_Pro;
const int Pro_Size=50;

typedef struct Register_Image{
  int AX,BX,CX,DX,SI,DI,BP;
  int ES,DS,SS,SP,IP,CS,FLAGS;
}RegImg;

typedef struct PCB{
  RegImg registers;
  int state;
  char name[BUFLEN];
}PCB;
PCB Pro_List[BUFLEN];

void Clear_PCB(PCB *Pro)
{
  Pro->registers.AX=0;
  Pro->registers.BX=0;
  Pro->registers.CX=0;
  Pro->registers.DX=0;
  Pro->registers.SI=0;
  Pro->registers.DI=0;
  Pro->registers.BP=0;
  Pro->registers.ES=0;
  Pro->registers.DS=0;
  Pro->registers.SS=0;
  Pro->registers.SP=0;
  Pro->registers.IP=0;
  Pro->registers.CS=0;
  Pro->registers.FLAGS=0;
  Pro->state=DIE;
}

void Save_Pro(PCB *Pro)
{
  Pro->registers.AX=ax_save;
  Pro->registers.BX=bx_save;
  Pro->registers.CX=cx_save;
  Pro->registers.DX=dx_save;
  Pro->registers.SI=si_save;
  Pro->registers.DI=di_save;
  Pro->registers.BP=bp_save;
  Pro->registers.ES=es_save;
  Pro->registers.DS=ds_save;
  Pro->registers.SS=ss_save;
  Pro->registers.SP=sp_save;
  Pro->registers.IP=ip_save;
  Pro->registers.CS=cs_save;
  Pro->registers.FLAGS=flags_save;
}

void Restore_Pro(PCB *Pro)
{
  ax_save=Pro->registers.AX;
  bx_save=Pro->registers.BX;
  cx_save=Pro->registers.CX;
  dx_save=Pro->registers.DX;
  si_save=Pro->registers.SI;
  di_save=Pro->registers.DI;
  bp_save=Pro->registers.BP;
  es_save=Pro->registers.ES;
  ds_save=Pro->registers.DS;
  ss_save=Pro->registers.SS;
  sp_save=Pro->registers.SP;
  ip_save=Pro->registers.IP;
  cs_save=Pro->registers.CS;
  flags_save=Pro->registers.FLAGS;
}

void Make_Alive(int id){Pro_List[id].state=READY;}
void Make_Die(int id){Pro_List[id].state=DIE;}
void Make_Pause(int id){Pro_List[id].state=BLOCK;}
int Is_Pause(int id){return Pro_List[id].state==BLOCK;}

int Find_Pro(char *str)
{
  int i;
  for(i=0;i<Pro_Size;i++)
    if(strcmp(Pro_List[i].name,str)&&Pro_List[i].state!=DIE)return i;
  return -1;
}

int Pro_Empty()
{
  int i;
  for(i=0;i<Pro_Size;i++)
    if(Pro_List[i].state!=DIE)return 0;
  return 1;
}

void Show_Pro()
{
  int i;
  puts("=============================");
  print_len("id",7);putchar('|');
  print_len("name",10);putchar('|');
  print_len("state",10);puts("");
  for(i=0;i<Pro_Size;i++)
  {
    if(Pro_List[i].state==DIE)continue;
    puts("-----------------------------");
	putint_len(i,7);putchar('|');
    print_len(Pro_List[i].name,10);putchar('|');
	if(Pro_List[i].state==BLOCK)print_len("pause",10);
	else print_len("run",10);puts(""); 
  }
  puts("=============================");
}

int init(char *str,int segment,int offset)
{
  int i;
  for(i=0;i<Pro_Size;i++)
    if(Pro_List[i].state==DIE)break;
  strcpy(Pro_List[i].name,str);
  Pro_List[i].state=DIE;
  Pro_List[i].registers.AX=0;
  Pro_List[i].registers.BX=0;
  Pro_List[i].registers.CX=0;
  Pro_List[i].registers.DX=0;
  Pro_List[i].registers.SI=0;
  Pro_List[i].registers.DI=0;
  Pro_List[i].registers.BP=0;
  Pro_List[i].registers.FLAGS=512;
  Pro_List[i].registers.CS=segment;
  Pro_List[i].registers.DS=segment;
  Pro_List[i].registers.ES=segment;
  Pro_List[i].registers.IP=offset;
  Pro_List[i].registers.SS=segment;
  Pro_List[i].registers.SP=offset-4;
  return i;
}

void print_all()
{
  printf("ax: ");putint(ax_save);puts("");
  printf("bx: ");putint(bx_save);puts("");
  printf("cx: ");putint(cx_save);puts("");
  printf("dx: ");putint(dx_save);puts("");
  printf("cs: ");putint(cs_save);puts("");
  printf("ds: ");putint(ds_save);puts("");
  printf("es: ");putint(es_save);puts("");
  printf("ip: ");putint(ip_save);puts("");
  printf("si: ");putint(si_save);puts("");
  printf("di: ");putint(di_save);puts("");
  printf("bp: ");putint(bp_save);puts("");
  printf("flags: ");putint(flags_save);puts("");
  printf("Cur_Pro: ");putint(Cur_Pro);puts("");
} 
/*for debug*/ 

void Pro_Clear()
{
  int i;
  for(i=1;i<Pro_Size;i++)Clear_PCB(&Pro_List[i]);
} 

initialize()
{
  int i;
  for(i=0;i<Pro_Size;i++)Clear_PCB(&Pro_List[i]);
  Cur_Pro=0;
  Make_Alive(Cur_Pro);
  strcpy(Pro_List[Cur_Pro].name,"kernal");
}

time_main()
{
  Schedule();
  if(TIME)Show_Int8(); 
}

#endif
