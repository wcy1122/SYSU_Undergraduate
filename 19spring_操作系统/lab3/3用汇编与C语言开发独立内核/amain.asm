		;程序源代码（showstr.asm）
extern _upper:near   	;声明一个c程序函数upper
extern _Message:near     	;声明一个外部变量
org 100h
start:	mov  ax,  cs
	mov  ds,  ax           	; DS = CS
	mov  es,  ax          	; ES = CS
	mov  ss,  ax       	; SS = CS
	mov  sp, 100h    
	call near ptr _upper 	;调用C的函数
	mov bp, offset _Message ; BP=当前串的偏移地址
	mov ax, ds		;BP = 串地址
	mov es, ax		;置ES=DS 
	mov cx, 10        	; CX = 串长（=10）
	mov  ax, 1301h	;  AH = 13h（功能号） AL = 01h（光标置于串尾）
	mov bx, 0007h	; 页号为0(BH = 0) 黑底白字(BL = 07h)
	mov  dh, 10	; 行号=10
	mov dl, 10		; 列号=10
	int10h		; BIOS的10h功能：显示一行字符
	jmp $
