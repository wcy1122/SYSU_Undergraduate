org  7c00h
OffSetOfUserPrg1 equ 0a100h
start:
	mov	ax,cs
	mov	ds,ax
	mov	ax,ds
	mov	es,ax
	
	mov bx,OffSetOfUserPrg1
	mov cx,bx
	mov ah,2 
	mov al,11
	mov dl,0
	mov dh,0
	mov ch,0
	mov cl,2
	int 13H ;
	jmp OffSetOfUserPrg1

AfterRun:
	jmp $
	times 510-($-$$) db 0
	db 0x55,0xaa
