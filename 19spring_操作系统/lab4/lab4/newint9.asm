org 9900h

work:
	push ax
	push bx
	push cx
	push dx
	push ds
	push es
	push bp
	push si
	push di
	
	mov ax,cs
	mov ds,ax
	mov es,ax
	
	in al,60h
	mov byte[cc],al
	
	mov ax,word[X]
	mov bx,80
	mul bx
	add ax,word[Y]
	mov bx,2
	mul bx
	mov bx,ax
	;get position
	
show:
    mov ah,0Fh
	mov byte[gs:bx],'O'
	mov byte[gs:bx+2],'U'
	mov byte[gs:bx+4],'C'
	mov byte[gs:bx+6],'H'
	mov byte[gs:bx+8],'!'
	
get_x:
	mov ax,word[X]
	mov bx,17
	mul bx
	call random
	add ax,bx
	xor dx,dx
	mov bx,23
	div bx
	mov word[X],dx
	
get_y:	
	mov ax,word[Y]
	mov bx,17
	mul bx
	call random
	add ax,bx
	xor dx,dx
	mov bx,73
	div bx
	mov word[Y],dx
	
	jmp end

random:
	push ax
	mov ah,02h
	int 1ah
	mov bx,dx
	pop ax
	ret
	
end:
	pop di
	pop si
	pop bp
	pop es
	pop ds
	pop dx
	pop cx
	pop bx
	pop ax
	mov al,20h	   ; AL = EOI
	out 20h,al	   ; 发送EOI到主8529A
	out 0A0h,al	   ; 发送EOI到从8529A
	iret		   ; 从中断返回
	
	
datadef:
	cc db 0
	X dw 15
	Y dw 30
	pos dw 0
	