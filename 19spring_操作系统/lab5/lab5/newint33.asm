extrn _int21h_main:near
extrn _Ax
extrn _Bx
extrn _Cx
extrn _Dx
.8086
_TEXT segment byte public 'CODE'
DGROUP group _TEXT,_DATA,_BSS
       assume cs:_TEXT
org 0100h
start:
	push ds
	push es
	push bp
	push si
	push di
	
	sti
	mov si,ax
	mov ax,0c40h
	mov ds,ax           ; DS = CS
	mov es,ax           ; ES = CS
	mov [_Ax],si
	mov [_Bx],bx
	mov [_Cx],cx
	mov [_Dx],dx
	call near ptr _int21h_main
	
	mov al,20h	   ; AL = EOI
	out 20h,al	   ; 发送EOI到主8529A
	out 0A0h,al	   ; 发送EOI到从8529A
	pop di
	pop si
	pop bp
	pop es
	pop ds
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
