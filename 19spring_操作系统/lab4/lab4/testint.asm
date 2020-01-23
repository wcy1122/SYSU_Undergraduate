org 0e100h
start:
  mov ax,cs
  mov ds,ax
  mov es,ax
  
  mov ah,06h
  mov al,0
  mov ch,0
  mov cl,40
  mov dh,12
  mov dl,79
  mov bh,07h ;黑底白字
  int 10h
  
  mov bp,Message	      ; BP=当前串的偏移地址
  mov ax,ds		      ; ES:BP = 串地址
  mov es,ax		      ; 置ES=DS
  mov cx,MessageLength ; CX = 串长（=9）
  mov ax,1301h		  ; AH = 13h（功能号）、AL = 01h（光标置于串尾）
  mov bx,006Fh		  ; 页号为0(BH = 0) 黑底白字(BL = 07h)
  mov dh,2		      ; 行号=0
  mov dl,50			  ; 列号=0
  int 10h			      ; BIOS的10h功能：显示一行字符
  
  
  int 33h
  int 34h
  int 35h
  int 36h
  ret
  
datadef:
	Message db 'Deng He Que Lou'
	MessageLength equ ($-Message)
    times 510-($-$$) db 0
    db 0x55,0xaa