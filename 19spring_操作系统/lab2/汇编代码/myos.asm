;����Դ���루myos1.asm��
org  7c00h		; BIOS���������������ص�0:7C00h��������ʼִ��
OffSetOfUserPrg1 equ 0a100h

bits 16

Start:
	mov	ax,cs	       ; �������μĴ���ֵ��CS��ͬ
	mov	ds,ax	       ; ���ݶ�
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
	;��ʾ����
	
	mov	bp,Message		 ; BP=��ǰ����ƫ�Ƶ�ַ
	mov	ax,ds		 ; ES:BP = ����ַ
	mov	es,ax		 ; ��ES=DS
	mov	cx,MessageLength  ; CX = ������=9��
	mov	ax,1301h		 ; AH = 13h�����ܺţ���AL = 01h��������ڴ�β��
	mov	bx,0047h		 ; ҳ��Ϊ0(BH = 0) �ڵװ���(BL = 07h)
    mov dh,11		       ; �к�=0
	mov	dl,28			 ; �к�=0
	int	10h			 ; BIOS��10h���ܣ���ʾһ���ַ�
	mov	bp,Info		 ; BP=��ǰ����ƫ�Ƶ�ַ
	mov	cx,InfoLength  ; CX = ������=9��
	mov dh,13		       ; �к�=0
	mov	dl,28			 ; �к�=0
	int	10h			 ; BIOS��10h���ܣ���ʾһ���ַ�
	mov	bp,Hint		 ; BP=��ǰ����ƫ�Ƶ�ַ
	mov	cx,HintLength  ; CX = ������=9��
	mov dh,12		       ; �к�=0
	mov	dl,20			 ; �к�=0
	int	10h			 ; BIOS��10h���ܣ���ʾһ���ַ�
	
	
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
    ;�����̻�Ӳ���ϵ����������������ڴ��ES:BX����
    mov ax,cs                ;�ε�ַ ; ������ݵ��ڴ����ַ
    mov es,ax                ;���öε�ַ������ֱ��mov es,�ε�ַ��
    mov bx,OffSetOfUserPrg1  ;ƫ�Ƶ�ַ; ������ݵ��ڴ�ƫ�Ƶ�ַ
    mov ah,2                 ;���ܺ�
    mov al,1                 ;������
    mov dl,0                 ;�������� ; ����Ϊ0��Ӳ�̺�U��Ϊ80H
    mov dh,0                 ;��ͷ�� ; ��ʼ���Ϊ0
    mov ch,0                 ;����� ; ��ʼ���Ϊ0
    mov byte cl,[x]          ;��ʼ������ ; ��ʼ���Ϊ1
	add cl,1
    int 13H                  ;���ö�����BIOS��13h����
    ; �û�����a.com�Ѽ��ص�ָ���ڴ�������
    jmp OffSetOfUserPrg1
	
AfterRun:	
    jmp Start                 ;����ѭ��
	
	x db 0
	Message db 'Welcome to wcy1122"s OS!'
	MessageLength equ ($-Message)
	Hint    db 'Input the guide program you want to run:'
	HintLength equ ($-Hint)
	Info 	db 'Wang Chengyao  17341146'
	InfoLength equ ($-Info)
	times 510-($-$$) db 0
	db 0x55,0xaa
