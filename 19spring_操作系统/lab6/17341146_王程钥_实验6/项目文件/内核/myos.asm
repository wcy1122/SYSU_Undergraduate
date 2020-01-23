extrn _main:near
extrn _initialize:near
extrn _time_main:near
extrn _int21h_main:near
extrn _Ax:word
extrn _Bx:word
extrn _Cx:word
extrn _Dx:word
extrn _num:word
extrn _count:word
extrn _ax_save:word
extrn _bx_save:word
extrn _cx_save:word
extrn _dx_save:word
extrn _si_save:word
extrn _di_save:word
extrn _bp_save:word
extrn _es_save:word
extrn _ds_save:word
extrn _ss_save:word
extrn _sp_save:word
extrn _ip_save:word
extrn _cs_save:word
extrn _flags_save:word
extrn _Cur_Pro:word

.8086
_TEXT segment byte public 'CODE'
DGROUP group _TEXT,_DATA,_BSS
       assume cs:_TEXT
org 0100h
start:
  mov ax,0a00h
  mov ds,ax           ; DS = CS
  mov es,ax           ; ES = CS
  mov ss,ax           ; SS = cs
  mov sp,100h
  
  call near ptr _initialize
  
;***************************************
;* load int8h Interrupt                *
;***************************************
  
  push ds
  mov ax,0h
  mov ds,ax
  mov bx,20h
  mov ax,offset Timer
  mov [bx],ax
  mov bx,22h
  mov ax,es
  mov [bx],ax
  pop ds

  
;***************************************
;* load int21h Interrupt               *
;***************************************
  
  push ds
  mov ax,0h
  mov ds,ax
  mov bx,84h
  mov ax,offset System
  mov [bx],ax
  mov bx,86h
  mov ax,es
  mov [bx],ax
  pop ds
  
;***************************************
;* main function                       *
;***************************************
  
  call near ptr _main
  jmp $
  
;***************************************
;* int8h Interrupt                     *
;***************************************
  
Timer:
Save:
  push ds					; stack: *\flags\cs\ip\ds(user)
  push cs					; stack: *\flags\cs\ip\ds(user)\cs(kernal)
  pop ds					; stack: *\flags\cs\ip\ds(user)
							; ds=cs(kernal)
  
  mov _ax_save,ax			; save ax
  pop ax					; Stack: *\flags\cs\ip
  mov _ds_save,ax			; save ds
  pop ax					; stack: *\flags\cs
  mov _ip_save,ax			; save ip
  pop ax					; stack: *\flags
  mov _cs_save,ax			; save cs
  pop ax					; stack: *
  mov _flags_save,ax		; save ax
  mov _bx_save,bx			; save bx
  mov _cx_save,cx			; save cs
  mov _dx_save,dx			; save dx
  mov _si_save,si			; save si
  mov _di_save,di			; save di
  mov _bp_save,bp			; save bp
  mov _es_save,es			; save es
  mov _ss_save,ss			; save ss
  mov _sp_save,sp			; save sp
  
  mov ax,0a00h
  mov ds,ax
  mov es,ax
  mov ss,ax
  mov sp,0h
  
  call near ptr _time_main
  
Restore:
  mov bx,_bx_save			; restore bx
  mov cx,_cx_save			; restore cs
  mov dx,_dx_save			; restore dx
  mov si,_si_save			; restore si
  mov di,_di_save			; restore di
  mov bp,_bp_save			; restore bp
  mov es,_es_save			; restore es
  mov ss,_ss_save			; restore ss
  mov sp,_sp_save			; restore sp
  mov ax,_flags_save		; save flags
  push ax					; stack *\flags
  mov ax,_cs_save			; save cs
  push ax					; stack *\flags\cs
  mov ax,_ip_save			; save ip
  push ax					; stack *\flags\cs\ip
  mov ax,_ax_save			; save ax
  mov ds,_ds_save			; save ds
  
int_8h_end:
  mov al,20h	   ; AL = EOI
  out 20h,al	   ; 发送EOI到主8529A
  out 0A0h,al	   ; 发送EOI到从8529A
  iret
  
;***************************************
;* int21h Interrupt                    *
;***************************************
System:
  push ds
  push es
  push ax
  push bx
  push cx
  push dx
  push di
  push si
  push bp
  
  sti
  mov [_Ax],ax
  mov [_Bx],bx
  mov [_Cx],cx
  mov [_Dx],dx
  call near ptr _int21h_main
  
  pop bp
  pop si
  pop di
  pop dx
  pop cx
  pop bx
  pop ax
  pop es
  pop ds
  
int_21h_end:
  mov al,20h	   ; AL = EOI
  out 20h,al	   ; 发送EOI到主8529A
  out 0A0h,al	   ; 发送EOI到从8529A
  iret

_TEXT ends
;************DATA segment*************
_DATA segment word public 'DATA'
_DATA ends
;*************BSS segment*************
_BSS	segment word public 'BSS'
_BSS ends
;**************end of file***********
end start
