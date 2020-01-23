org 9500h
	
start:
	push ds
	push es
	push ax
	push bx
	push cx
	push dx
	push si
	push gs
	
	mov ax,cs
	mov ds,ax
	mov es,ax
	
	mov ax,0B800h
    mov gs,ax
	
	cmp byte[count],4
	jbe go
	mov byte[count],4
go:
	dec byte[count]
	jnz end	
	mov ah,0Fh
	dec byte[num]
	
	cmp byte[num],0
	  jz show0
	cmp byte[num],1
	  jz show1
	cmp byte[num],2
	  jz show2
	cmp byte[num],3
	  jz show3
show0:
	mov byte[gs:((80*24+79)*2)],'-'
	mov byte[num],4
	jmp reset
show1:
	mov byte[gs:((80*24+79)*2)],'/'
	jmp reset
show2:
	mov byte[gs:((80*24+79)*2)],'|'
	jmp reset
show3:
	mov byte[gs:((80*24+79)*2)],'\'
reset:
	mov byte[count],4
	
	
showtime:
	mov bx,(80*24+59)*2
	
	mov ah,04h
    int 1ah
	mov al,ch
	call bcd_to_ascii
	mov al,cl
	call bcd_to_ascii
	mov byte[gs:bx],'/'
	add bx,2
	;年
	
	mov al,dh
	call bcd_to_ascii
	mov byte[gs:bx],'/'
	add bx,2
	;月
	
	mov al,dl
	call bcd_to_ascii
	mov byte[gs:bx],' '
	add bx,2
	;日
	
	mov ah,02h
    int 1ah
	
	mov al,ch
	call bcd_to_ascii
	mov byte[gs:bx],':'
	add bx,2
	;时
	
	mov al,cl
	call bcd_to_ascii
	mov byte[gs:bx],':'
	add bx,2
	;分
	
	mov al,dh
	call bcd_to_ascii
	;秒
	
	jmp end
	
	
bcd_to_ascii:
	mov ah,al
	and al,0x0f
	add al,0x30
	shr ah,4
	and ah,0x0f
	add ah,0x30
	mov byte[gs:bx],ah
	add bx,2
	mov byte[gs:bx],al
	add bx,2
	ret
	

end:
	pop gs
	pop si
	pop dx
	pop cx
	pop bx
	pop ax
	pop es
	pop ds
	mov al,20h	   ; AL = EOI
	out 20h,al	   ; 发送EOI到主8529A
	out 0A0h,al	   ; 发送EOI到从8529A
	iret		   ; 从中断返回

datadef:
	num db 4
	count db 4
	
times 510-($-$$) db 0
db 0x55,0xaa