; 程序源代码（stone.asm）
; 本程序在文本方式显示器上从左边射出一个*号,以45度向右下运动，撞到边框后反射,如此类推.
; 凌应标 2014/3
; wcy1122 2019/3
; NASM汇编格式
    Dn_Rt equ 1                  ;D-Down,U-Up,R-right,L-Left
    Up_Rt equ 2                  ;
    Up_Lt equ 3                  ;
    Dn_Lt equ 4                  ;
    delay equ 50000					; 计时器延迟计数,用于控制画框的速度
    ddelay equ 580					; 计时器延迟计数,用于控制画框的速度
	maxcnt equ 50
	L	 equ 59
	R 	 equ 80
	U    equ 7
	D    equ 16
	org 100h				; 程序加载到100h，可用于生成COM
	
start:
	mov ax,cs
	mov	ds,ax
	mov	es,ax
	mov di,0
	mov si,0
	mov ax,0B800h				; 文本窗口显存起始地址
	mov gs,ax					; GS = B800h
	
	mov ah,06h
	mov al,0
	mov ch,U+1
	mov cl,L+1
	mov dh,D-1
	mov dl,R-1
	mov bh,07h ;黑底白字
	int 10h
	;清屏

	mov word[cnt],maxcnt
	jmp show
loop1:
	dec word[count]				; 递减计数变量
	jnz loop1					; >0：跳转;
	mov word[count],delay
	dec word[dcount]				; 递减计数变量
    jnz loop1
	mov word[count],delay
	mov word[dcount],ddelay

work:
	dec word[cnt]
	jz end
	
	mov si,0
	mov bx,word[xx]
	add word[x],bx
	mov bx,word[yy]
	add word[y],bx

check_LR:
	mov bx,word[y]
	mov ax,L
	sub ax,bx
	  jz modify_L
	mov bx,word[y]
	mov ax,R
	sub ax,bx
	  jz modify_R

check_UD:  
	mov bx,word[x]
	mov ax,U
	sub ax,bx
	  jz modify_U	  
	mov bx,word[x]
	mov ax,D
	sub ax,bx
      jz modify_D
	  
	jmp show

modify_L:
	mov si,1
	mov word[y],L+2
	mov word[yy],1
	jmp check_UD

modify_R:
	mov si,1
	mov word[y],R-2
	mov word[yy],-1
	jmp check_UD
	
modify_U:
	mov si,1
	mov word[x],U+2
	mov word[xx],1
	jmp show
	
modify_D:
	mov si,1
	mov word[x],D-2
	mov word[xx],-1
	jmp show
	
show:	
	xor ax,ax                 ; 计算显存地址
    mov ax,word[x]
	mov bx,80
	mul bx
	add ax,word[y]
	mov bx,2
	mul bx
	mov bx,ax
	
	inc si
	dec si
	jz to_print
	xor di,1
	
to_print:
	mov byte ah,06h				;  0000：黑底、1111：亮白字（默认值为07h）
	inc di
	dec di
	jz print
	mov byte ah,03h
	print:
		mov al,'D'			;  AL = 显示字符值（默认值为20h=空格符）
		mov [gs:bx],ax  	;  显示字符的ASCII码值  //有错！！！
			
	jmp loop1
	
end:	
    retf
	
datadef:
	count dw delay
    dcount dw ddelay
    x    dw 11
    y    dw 79
	xx	 dw 1
	yy   dw -1
	cnt  dw 0
	
	times 510-($-$$) db 0
	db 0x55,0xaa