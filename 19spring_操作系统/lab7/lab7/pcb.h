#ifndef __PCB_H
#define __PCB_H

#include"clock.h"
#include"string.h"

#define us unsigned short
#define DIE   0
#define BLOCK 1
#define READY 2
#define RUN   3
#define BUFLEN 100
#define BASE 0x80
#define DataSegment 0x1000
#define OffSetOfUserPrg 0x100


us ax_save,bx_save,cx_save,dx_save,si_save,di_save,bp_save;
us es_save,ds_save,ss_save,sp_save,ip_save,cs_save,flags_save;
int TIME,Cur_Pro;
const int Pro_Size=30;

typedef struct Register_Image{
  int AX,BX,CX,DX,SI,DI,BP;
  int ES,DS,SS,SP,IP,CS,FLAGS;
}RegImg;

typedef struct PCB{
  RegImg registers;
  int state;
  int FID;
  int son_cnt;
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
  Pro->FID=0;
  Pro->son_cnt=0;
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

int Find_Empty()
{
  int i;
  for(i=0;i<Pro_Size;i++)
    if(Pro_List[i].state==DIE)return i;
  return -1;
}

int Pro_Empty()
{
  int i;
  for(i=0;i<Pro_Size;i++)
    if(Pro_List[i].state!=DIE)return 0;
  return 1;
}

void Kill_Pro(int id,char ret)
{
  int i,x;PCB *Now,*Fa;
  Make_Die(id);
  Now=&Pro_List[id];
  if(Now->FID)
  {  
    Fa=&Pro_List[Now->FID];
    Fa->son_cnt--;
    if(Fa->son_cnt==0)
    {
	  Fa->registers.AX=ret;
	  Fa->state=READY;
    }
  }
  for(i=0;i<Pro_Size;i++)
  {
    if(!Pro_List[i].state)continue;
	for(x=i;x&&x!=id;x=Pro_List[x].FID);
	if(x==id)Make_Die(i);
  }
}

void PCB_Copy(PCB *A,PCB *B)
{
  A->registers.AX=B->registers.AX;
  A->registers.BX=B->registers.BX;
  A->registers.CX=B->registers.CX;
  A->registers.DX=B->registers.DX;
  A->registers.SI=B->registers.SI;
  A->registers.DI=B->registers.DI;
  A->registers.BP=B->registers.BP;
  A->registers.FLAGS=B->registers.FLAGS;
  A->registers.CS=B->registers.CS;
  A->registers.DS=B->registers.DS;
  A->registers.ES=B->registers.ES;
  A->registers.SS=B->registers.SS;
  A->registers.IP=B->registers.IP;
  A->registers.SP=B->registers.SP;
}

int init(int i,char *str,int CS,int IP)
{
  strcpy(Pro_List[i].name,str);
  Pro_List[i].state=DIE;
  Pro_List[i].FID=0;
  Pro_List[i].son_cnt=0;
  Pro_List[i].registers.AX=0;
  Pro_List[i].registers.BX=0;
  Pro_List[i].registers.CX=0;
  Pro_List[i].registers.DX=0;
  Pro_List[i].registers.SI=0;
  Pro_List[i].registers.DI=0;
  Pro_List[i].registers.BP=0;
  Pro_List[i].registers.FLAGS=512;
  Pro_List[i].registers.CS=CS;
  Pro_List[i].registers.DS=CS;
  Pro_List[i].registers.ES=CS;
  Pro_List[i].registers.IP=IP;
  Pro_List[i].registers.SS=CS;
  Pro_List[i].registers.SP=IP-4;
}

void print_all()
{
  char x,y;
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
  printf("ss: ");putint(ss_save);puts("");
  printf("sp: ");putint(sp_save);puts("");
  printf("flags: ");putint(flags_save);puts("");
  printf("Cur_Pro: ");putint(Cur_Pro);puts("");
  printf("Cur_Pro: ");putint(Cur_Pro);puts("");
  get_pos(&x,&y);
  printf("Pos: ");putint(x);putchar(' ');putint(y);puts("");
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

void Schedule()
{
  int i;
  Save_Pro(&Pro_List[Cur_Pro]);
  if(Pro_List[Cur_Pro].state!=DIE)
    Pro_List[Cur_Pro].state=READY;
  while(1)
  {
    Cur_Pro=(Cur_Pro+1)%Pro_Size;
    if(Pro_List[Cur_Pro].state==READY)break;
  }
  Restore_Pro(&Pro_List[Cur_Pro]);
  Pro_List[Cur_Pro].state=RUN;
}

time_main()
{
  Schedule();
  if(TIME)Show_Int8(); 
}

#endif
