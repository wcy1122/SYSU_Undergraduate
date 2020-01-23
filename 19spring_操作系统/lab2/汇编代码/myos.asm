;程序源代码（myos1.asm）
org  7c00h		; BIOS将把引导扇区加载到0:7C00h处，并开始执行
OffSetOfUserPrg1 equ 0a100h

bits 16

Start:
	mov	ax,cs	       ; 置其他段寄存器值与CS相同
	mov	ds,ax	       ; 数据段
	mov ah,06h
	mov al,0
	mov bh,27h
	mov ch,0
	mov cl,39
	mov dh,24
	mov dl,39
	int 10h
	
	mov ah,06h
	mov al,0
	mov bh,27h
	mov ch,12
	mov cl,0
	mov dh,12
	mov dl,79
	int 10h
	
	mov ah,06h
	mov al,0
	mov bh,47h
	mov ch,11
	mov cl,18
	mov dh,13
	mov dl,62
	int 10h
	;显示界面
	
	mov	bp,Message		 ; BP=当前串的偏移地址
	mov	ax,ds		 ; ES:BP = 串地址
	mov	es,ax		 ; 置ES=DS
	mov	cx,MessageLength  ; CX = 串长（=9）
	mov	ax,1301h		 ; AH = 13h（功能号）、AL = 01h（光标置于串尾）
	mov	bx,0047h		 ; 页号为0(BH = 0) 黑底白字(BL = 07h)
    mov dh,11		       ; 行号=0
	mov	dl,28			 ; 列号=0
	int	10h			 ; BIOS的10h功能：显示一行字符
	mov	bp,Info		 ; BP=当前串的偏移地址
	mov	cx,InfoLength  ; CX = 串长（=9）
	mov dh,13		       ; 行号=0
	mov	dl,28			 ; 列号=0
	int	10h			 ; BIOS的10h功能：显示一行字符
	mov	bp,Hint		 ; BP=当前串的偏移地址
	mov	cx,HintLength  ; CX = 串长（=9）
	mov dh,12		       ; 行号=0
	mov	dl,20			 ; 列号=0
	int	10h			 ; BIOS的10h功能：显示一行字符
	
	
input:
	mov ah,0
	int 16h
	cmp al,'1'
	  jl input
	cmp al,'4'
	  jg input
	sub al,'0'
	mov byte[x],al
	
LoadnEx:
    ;读软盘或硬盘上的若干物理扇区到内存的ES:BX处：
    mov ax,cs                ;段地址 ; 存放数据的内存基地址
    mov es,ax                ;设置段地址（不能直接mov es,段地址）
    mov bx,OffSetOfUserPrg1  ;偏移地址; 存放数据的内存偏移地址
    mov ah,2                 ;功能号
    mov al,1                 ;扇区数
    mov dl,0                 ;驱动器号 ; 软盘为0，硬盘和U盘为80H
    mov dh,0                 ;磁头号 ; 起始编号为0
    mov ch,0                 ;柱面号 ; 起始编号为0
    mov byte cl,[x]          ;起始扇区号 ; 起始编号为1
	add cl,1
    int 13H                  ;调用读磁盘BIOS的13h功能
    ; 用户程序a.com已加载到指定内存区域中
    jmp OffSetOfUserPrg1
	
AfterRun:	
    jmp Start                 ;无限循环
	
	x db 0
	Message db 'Welcome to wcy1122"s OS!'
	MessageLength equ ($-Message)
	Hint    db 'Input the guide program you want to run:'
	HintLength equ ($-Hint)
	Info 	db 'Wang Chengyao  17341146'
	InfoLength equ ($-Info)
	times 510-($-$$) db 0
	db 0x55,0xaa
