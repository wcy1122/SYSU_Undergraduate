short Ax,Bx,Cx,Dx;
char Ah,Al,Bh,Bl,Ch,Cl,Dh,Dl;
char x,y,xx,yy;
void move(char a,char b)
{
  asm mov ah,02h
  asm mov bh,0
  asm mov dh,a
  asm mov dl,b
  asm int 10h
  x=a;y=b;
}

void putchar(char c,char col)
{
  int pos;
  asm push ax
  asm push bx
  asm push cx
  asm push dx
  if(c>=32)
  {
    asm mov ah,09h
    asm mov al,c
    asm mov bh,0
    asm mov bl,col
    asm mov dh,x
	asm mov dl,y
    asm mov cx,1
    asm int 10h
  }
  if(c==13)pos=(x+1)*80;
  else pos=x*80+y+1;
  x=pos/80;y=pos%80;
  move(x,y);
  asm pop dx
  asm pop cx
  asm pop bx
  asm pop ax
}
/*没有退格上下左右*/ 

void puts(char *s,char col)
{
  int i;
  for(i=0;s[i]!='\000';i++)putchar(s[i],col);
  putchar(13,col);
}

void sleep(int cnt)
{
  int i,j,k;
  for(i=1;i<=cnt;i++)
    for(j=1;j<=500;j++)
      for(k=1;k<=20;k++);
}

void print_format(char *s)
{
  int i;char X=x,Y=y;
  puts(s,0x0F);move(X,Y);
  for(i=0;s[i]!='\000';i++)
  {
    if('A'<=s[i]&&s[i]<='Z')putchar(s[i],0x0c);
    else putchar(s[i],0x0e);
    sleep(10000);
  }
}

void sys_exit()
{
  asm mov ax,5301h
  asm xor bx,bx
  asm int 15h
  asm mov ax,530Eh
  asm xor bx,bx
  asm mov cx,102h
  asm int 15h
  asm mov ax,5307h
  asm mov bx,1
  asm mov cx,3
  asm int 15h
}

void clear_screen(char a,char b,char c,char d)
{
  asm mov ah,06h
  asm mov al,0
  asm mov ch,a
  asm mov cl,b
  asm mov dh,c
  asm mov dl,d
  asm mov bh,07h
  asm int 10h
}

void rool_up(char a,char b,char c,char d)
{
  asm mov ah,06h
  asm mov al,1
  asm mov bh,07h
  asm mov ch,a
  asm mov cl,b
  asm mov dh,c
  asm mov dl,d
  asm int 10h
}

void rool_down(char a,char b,char c,char d)
{
  asm mov ah,07h
  asm mov al,1
  asm mov bh,07h
  asm mov ch,a
  asm mov cl,b
  asm mov dh,c
  asm mov dl,d
  asm int 10h
}

void print_poem()
{
  clear_screen(0,40,12,79);
  move(2,50);puts("Deng He Que Lou",0x0d);
  move(3,55);puts("Meng Hao Ran",0x0d);
  move(5,50);print_format("Bai Ri Yi Shan Jin,");
  move(6,50);print_format("Huang He Ru Hai Liu.");
  move(7,50);print_format("Yu Qiong Qian Li Mu,");
  move(8,50);print_format("Geng Shang Yi Ceng Lou.");
}

void print_song()
{
  clear_screen(0,40,12,79);
  move(2,50);puts("Hou Lai",0x0d);
  move(3,60);puts("Liu Ruo Ying",0x0d);
  move(5,40);print_format("Hou Lai");
  move(6,50);print_format("Wo Zong Suan Xue Hui Le");
  move(7,40);print_format("Ru He Qu Ai");
  clear_screen(5,40,7,79);
  move(5,40);print_format("Ke Xi Ni Zao Yi Jing");
  move(6,50);print_format("Xiao Shi Zai Ren Hai");
  clear_screen(5,40,7,79);
  move(5,40);print_format("Hou Lai");
  move(6,50);print_format("Zhong Yu Zai Yan Lei Zhong");
  move(7,40);print_format("Ming Bai");
  clear_screen(5,40,7,79);
  move(5,40);print_format("You Xie Ren");
  move(6,50);print_format("Yi Dan Cuo Guo Jiu Bu Zai.");
}

void show_ouch()
{
  move(xx,yy+40);
  puts("OUCH!",0x0F);
  xx=(xx*23+rand())%13;
  yy=(yy*23+rand())%31;
}

int rand()
{
  int res;
  asm mov ah,02h
  asm int 1ah
  asm mov res,dx
  return res;
}

int21h_main()
{
  asm sti;
  Ah=Ax/256;Al=Ax%256;
  Bh=Bx/256;Bl=Bx%256;
  Ch=Cx/256;Cl=Cx%256;
  Dh=Dx/256;Dl=Dx%256;
  if(Ah==0x00)show_ouch(); 
  if(Ah==0x01)print_poem();
  if(Ah==0x02)print_song();
  if(Ah==0x03)clear_screen(Ch,Cl,Dh,Dl);
  if(Ah==0x04)rool_up(Ch,Cl,Dh,Dl);
  if(Ah==0x05)rool_down(Ch,Cl,Dh,Dl);
  if(Ah==0x4c)sys_exit();
}
