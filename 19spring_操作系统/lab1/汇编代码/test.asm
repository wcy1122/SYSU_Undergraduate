; 程序源代码（stone.asm）
; 本程序在文本方式显示器上从左边射出一个*号,以45度向右下运动，撞到边框后反射,如此类推.
; 凌应标 2014/3
; wcy1122 2019/3
; MASM汇编格式
    Dn_Rt equ 1                  ;D-Down,U-Up,R-right,L-Left
    Up_Rt equ 2                  ;
    Up_Lt equ 3                  ;
    Dn_Lt equ 4                  ;
    delay equ 50000					; 计时器延迟计数,用于控制画框的速度
    ddelay equ 580					; 计时器延迟计数,用于控制画框的速度
    ;.386
    org 7c00h					; 程序加载到100h，可用于生成COM
    ;ASSUME cs:code,ds:code
;code SEGMENT ;MASM style

section .data
    count dw delay
    dcount dw ddelay
    rdul db Dn_Rt         ; 向右下运动
    x    dw 7
    y    dw 0
    char1 db 'wcy'
	char2 db 'ycw'
	char3 db '       wangchengyao__17341146       '
	
section .text

start:
	;xor ax,ax					; AX = 0   程序加载到0000：100h才能正确执行
    mov ax,cs
	;mov es,ax					; ES = 0
	mov ds,ax					; DS = CS
	;mov es,ax					; ES = CS
	mov ax,0B800h				; 文本窗口显存起始地址
	mov gs,ax					; GS = B800h

show_name:	
	mov bx,3884
	mov byte ah,2Fh				;  0000：黑底、1111：亮白字（默认值为07h）
	mov si,char3
	mov cx,36
		print2:
			mov byte al,[si]
			mov [gs:bx],ax
			inc si
			inc bx
			inc bx
			loop print2

loop1:
	dec word[count]				; 递减计数变量
	jnz loop1					; >0：跳转;
	mov word[count],delay
	dec word[dcount]				; 递减计数变量
      jnz loop1
	mov word[count],delay
	mov word[dcount],ddelay
      mov al,1
      cmp al,byte[rdul]
	jz  DnRt
      mov al,2
      cmp al,byte[rdul]
	jz  UpRt
      mov al,3
      cmp al,byte[rdul]
	jz  UpLt
      mov al,4
      cmp al,byte[rdul]
	jz  DnLt
      jmp $	

DnRt:
	inc word[x]
	inc word[y]
	mov bx,word[x]
	mov ax,24
	sub ax,bx
      jz  dr2ur
	mov bx,word[y]
	mov ax,78
	sub ax,bx
      jz  dr2dl
	jmp show
dr2ur:
      mov word[x],23
	  mov bx,word[y]
	  mov ax,78
	  sub ax,bx
	    jz  dr2dl
	  xor di,1
      mov byte[rdul],Up_Rt	  
      jmp show
dr2dl:
	  xor di,1
      mov word[y],76
      mov byte[rdul],Dn_Lt	
      jmp show

UpRt:
	dec word[x]
	inc word[y]
	mov bx,word[y]
	mov ax,78
	sub ax,bx
      jz  ur2ul
	mov bx,word[x]
	mov ax,-1
	sub ax,bx
      jz  ur2dr
	jmp show
ur2ul:
      mov word[y],76
	  mov bx,word[x]
	  mov ax,-1
	  sub ax,bx
        jz  ur2dr
	  xor di,1
      mov byte[rdul],Up_Lt	
      jmp show
ur2dr:
	  xor di,1
      mov word[x],1
      mov byte[rdul],Dn_Rt	
      jmp show

	  
UpLt:
	dec word[x]
	dec word[y]
	mov bx,word[x]
	mov ax,-1
	sub ax,bx
      jz  ul2dl
	mov bx,word[y]
	mov ax,-1
	sub ax,bx
      jz  ul2ur
	jmp show
ul2dl:
      mov word[x],1
	  mov bx,word[y]
	  mov ax,-1
	  sub ax,bx
	    jz  ul2ur
	  xor di,1
      mov byte[rdul],Dn_Lt	
      jmp show
ul2ur:
	  xor di,1
      mov word[y],1
      mov byte[rdul],Up_Rt	
      jmp show
	
	
DnLt:
	inc word[x]
	dec word[y]
	mov bx,word[y]
	mov ax,-1
	sub ax,bx
      jz  dl2dr
	mov bx,word[x]
	mov ax,24
	sub ax,bx
      jz  dl2ul
	jmp show
dl2dr:
      mov word[y],1
	  mov bx,word[x]
	  mov ax,24
	  sub ax,bx
        jz  dl2ul
	  xor di,1
      mov byte[rdul],Dn_Rt	
      jmp show
dl2ul:
	  xor di,1
      mov word[x],23
      mov byte[rdul],Up_Lt	
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
	
	mov byte ah,0Eh				;  0000：黑底、1111：亮白字（默认值为07h）
	mov si,char1
	inc di
	dec di
	jz print
	mov si,char2
	mov byte ah,0Dh
	
	print:
	mov cx, 3
		print_str:
			mov byte al,[si]			;  AL = 显示字符值（默认值为20h=空格符）
			mov [gs:bx],ax  		;  显示字符的ASCII码值  //有错！！！
			inc si
			add bx,2
			loop print_str
	jmp loop1
	
end:
    jmp $                   ; 停止画框，无限循环 
	
;datadef: //MASM style
;code ENDS
    ;END start
