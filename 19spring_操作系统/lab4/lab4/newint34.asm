org 0c700h

start:
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
	mov	bp,Message1	      ; BP=当前串的偏移地址
	mov	ax,ds		      ; ES:BP = 串地址
	mov	es,ax		      ; 置ES=DS
	mov	cx,Message1Length ; CX = 串长（=9）
	mov	ax,1301h		  ; AH = 13h（功能号）、AL = 01h（光标置于串尾）
	mov	bx,003Fh		  ; 页号为0(BH = 0) 黑底白字(BL = 07h)
    mov dh,6		      ; 行号=0
	mov	dl,60			  ; 列号=0
	int	10h			      ; BIOS的10h功能：显示一行字符
	mov ax,cs
	mov ds,ax
	mov	bp,Message2       ; BP=当前串的偏移地址
	mov	ax,ds		      ; ES:BP = 串地址
	mov	es,ax		      ; 置ES=DS
	mov	cx,Message2Length ; CX = 串长（=9）
	mov	ax,1301h		  ; AH = 13h（功能号）、AL = 01h（光标置于串尾）
	mov	bx,0007h		  ; 页号为0(BH = 0) 黑底白字(BL = 07h)
    mov dh,7		      ; 行号=0
	mov	dl,58			  ; 列号=0
	int	10h			      ; BIOS的10h功能：显示一行字符
	
	sti
	
	mov ax,cs
	mov ds,ax
	mov es,ax
	mov ax,0B800h
    mov gs,ax
	
	mov si,Message2
	mov word[len],Message2Length
	mov bx,1236
print_str:
	mov byte al,[si]
	mov ah,0eh
	cmp al,'Z'
	jg putchar
	mov ah,0ch
putchar:
	mov [gs:bx],ax
nxt:
	inc si
	add bx,2
	mov word[count1],50
	mov word[count2],10000
	call sleep
	dec word[len]
	jnz print_str
	jmp end
	
sleep:
	dec word[count2]
	jnz sleep
	mov word[count2],10000
	dec word[count1]
	jnz sleep
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
	len dw 0
	count1 dw 0
	count2 dw 0
    Message1 db 'This is int 34h'
	Message1Length equ ($-Message1)
    Message2 db 'Huang He Ru Hai Liu'
	Message2Length equ ($-Message2)	
