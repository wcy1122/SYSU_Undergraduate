#ifndef __SYSPRO_H
#define __SYSPRO_H

#include"pcb.h"
#include"sysio.h"
#include"string.h"

void STACK_Copy(int Now_SS,int Pre_SS)
{
  int i;
  asm push ds
  for(i=0;i<0x100;i+=0x02)
  {
    asm mov ax,Pre_SS
	asm mov ds,ax
	asm mov bx,i
	asm mov dx,[bx]
	asm mov ax,Now_SS
	asm mov ds,ax
	asm mov bx,i
	asm mov [bx],dx
  }
  asm pop ds
}

__fork()
{
  int id,SS;
  PCB *Now=&Pro_List[Cur_Pro],*New;
  Save_Pro(Now);
  id=Find_Empty();
  if(id==-1)Now->registers.AX=-1;
  else
  { 
    New=&Pro_List[id];
	SS=DataSegment+id*BASE;
	PCB_Copy(New,Now);
    STACK_Copy(SS,Now->registers.SS);
    New->FID=Cur_Pro;
    New->son_cnt=0;
	strcpy(New->name,"child");
    New->registers.AX=0;
    New->registers.SS=SS;
    Now->son_cnt++;
    Now->registers.AX=id;
    Make_Alive(id);
  } 
  Restore_Pro(Now);
}

__wait()
{
  Save_Pro(&Pro_List[Cur_Pro]);
  Make_Pause(Cur_Pro);
  Schedule();
  Restore_Pro(&Pro_List[Cur_Pro]);
}

__exit(char RET)
{
  asm cli
  Kill_Pro(Cur_Pro,RET);
  asm sti
  asm jmp $
}

# endif
