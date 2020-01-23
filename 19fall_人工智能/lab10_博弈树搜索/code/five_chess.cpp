#include<bits/stdc++.h>
#include<conio.h>
#include<Windows.h>
#define ll long long
#define seed 19990213
#define inf 2147483647
#define END 2100000000
#define N 20
using namespace std;
int len=11,s[N][N];
unordered_map<ll,int>vis;
HANDLE hout=GetStdHandle(STD_OUTPUT_HANDLE);
int t1[]={1,0,1,1};
int t2[]={0,1,1,-1};

char get(int x)
{
  if(x==0)return ' ';
  if(x==2)return 'X';
  return 'O';
}

void print_board()
{
  system("cls");
  for(int i=1;i<=4*len+5;i++)printf("_");
  printf("\n");
  printf("|   |");
  for(int i=1;i<=len;i++)
  {
    if(i<10)printf(" %d |",i);
    else printf(" %c |",'A'+i-10);
  }
  printf("\n");
  printf("|");
  for(int i=1;i<=len+1;i++)printf("___|");
  printf("\n");
  for(int i=1;i<=len;i++)
  {
  	if(i<10)printf("| %d |",i);
    else printf("| %c |",'A'+i-10);
    for(int j=1;j<=len;j++)printf("   |");
    printf("\n");
    printf("|");
    for(int j=1;j<=len+1;j++)printf("___|");
    printf("\n");
  }
}

bool cha(int x,int y)
{
  if(x<1||x>len||y<1||y>len)return true;
  return false;
}

ll get_hash(int s[][N])
{
  ll sum=0;
  for(int i=1;i<=len;i++)
    for(int j=1;j<=len;j++)
      sum=sum*seed+s[i][j];
  return sum;
}

int evaluate(int s[][N],int type,int method=1)
{
  int len_val[]={0,100,5000,20000,50000};
  int res=0;
  if(method==0)
  {
    for(int i=1;i<=len;i++)
      for(int j=1;j<=len;j++)
	    for(int k=0;k<4;k++)
	    {
		  if(s[i-t1[k]][j-t2[k]]==s[i][j])continue;
		  if(cha(i+4*t1[k],j+4*t2[k]))continue;
		  int l=0,ch=s[i][j];
		  for(int x=i,y=j;l<5;l++,x+=t1[k],y+=t2[k])
		    if(s[x][y]!=ch)break;
	      if(ch==2)res-=len_val[l];
	      else res+=len_val[l];
		}
	// 计算连续段长度，加权求和 
  }
  else if(method==1)
  {
  	int tmp[N][N][4]={0};
  	int count34=0,count3=0,count4=0;
    for(int i=1;i<=len;i++)
      for(int j=1;j<=len;j++)
	    for(int k=0;k<4;k++)
	    {
	      if(cha(i+4*t1[k],j+4*t2[k]))continue;
	      int cnt[4]={0};
		  for(int l=1,x=i,y=j;l<=5;l++,x+=t1[k],y+=t2[k])
	        cnt[s[x][y]]++;
	      if(cnt[2]&&cnt[3])continue; 
	      int now=max(cnt[2],cnt[3]);
		  int inv=(cnt[2]?-1:1);
		  int pre=tmp[i-t1[k]][j-t2[k]][k];
		  
	      if(now==4&&cnt[type])return inv*(END+3); // 4+1 type玩家必胜 
	      if(now==4&&pre==now)count4++; // 活四
		  if(now==4)count34++;// 眠四 
		  if(now==3&&pre==3)
		  {
		    if(cnt[type])count3++; // 活三 3+1 下一步变活四
			if(!cha(i-2*t1[k],j-2*t2[k]))
			{
			  int prep=tmp[i-2*t1[k]][j-2*t2[k]][k];
			  if(prep==3)count34++; // 活三 
			} 
		  } 
	      res+=inv*len_val[now];
	      tmp[i][j][k]=now;
	    }
	int inv=((type==2)?-1:1); 
	if(count4)return -inv*(END+2); //对手活四 ,type 输 
	if(count3)return inv*(END+1); //type玩家活三 
	if(count34>=2)return -inv*END; // 对手两个活三 or 眠四，type输 
	// +判断 必败/必胜 局面 
	// +计算所有5格区间，根据落子情况加权求和，统计潜在情况 
  }
  else assert(0);
  return res;
}
// 数点数估价函数 

int check_win()
{
  for(int i=1;i<=len;i++)
    for(int j=1;j<=len;j++)
    {
      if(!s[i][j])continue;
      for(int k=0;k<4;k++)
      {
		int ch=s[i][j],tag=0;
		if(cha(i+4*t1[k],j+4*t2[k]))continue;
		for(int x=i,y=j,l=0;l<5;l++,x+=t1[k],y+=t2[k])
		  if(s[x][y]!=ch){tag=1;break;}
		if(!tag)return ch;
	  }
	}
  return 0;
}

bool surround(int x,int y)
{
  for(int i=x-1;i<=x+1;i++)
    for(int j=y-1;j<=y+1;j++)
    {
      if(i<1||i>len||j<1||j>len)continue;
	  if(s[i][j])return true;
	}
  return false;
}

int dfs(int type,int dep,int alpha,int beta,int method)
{
  ll hash_val=get_hash(s);
  if(vis.count(hash_val))return vis[hash_val];
  // 重复状态 
  int win=check_win(),res;
  if(win==2)res=-(END+dep+10);
  else if(win==3)res=END+dep+10;
  else if(!dep)res=evaluate(s,type,method);
  else if(type==2)  // type: 2_min 
  {
    for(int i=1,tag=0;i<=len&&!tag;i++)
      for(int j=1;j<=len;j++)
      {
	    if(s[i][j])continue;
	    if(!surround(i,j))continue;
	    s[i][j]=2;
	    beta=min(beta,dfs(type^1,dep-1,alpha,beta,method));
	    s[i][j]=0;
	    if(beta<=alpha){tag=1;break;}
	  }
	res=beta;
  }
  else  // type: 3_max 
  {
    for(int i=1,tag=0;i<=len&&!tag;i++)
      for(int j=1;j<=len;j++)
      {
	    if(s[i][j])continue;
	    if(!surround(i,j))continue;
	    s[i][j]=3;
	    alpha=max(alpha,dfs(type^1,dep-1,alpha,beta,method));
	    s[i][j]=0;
	    if(beta<=alpha){tag=1;break;}
	  }
	res=alpha;
  }
  vis[hash_val]=res;
  return res;
}

int do_step(int &x,int &y,int type,int method,int MAX_DEPTH)
{
  int ti=clock(); 
  vis.clear();x=1;y=1;
  int alpha=-inf,beta=inf,tmp;
  for(int i=1;i<=len;i++)
    for(int j=1;j<=len;j++)
    {
	  if(s[i][j])continue; 
	  if(!surround(i,j))continue;
	  s[i][j]=type;
	  tmp=dfs(type^1,MAX_DEPTH,alpha,beta,method);
	  if(type==2){
	    if(tmp<beta)beta=tmp,x=i,y=j;
	  }
	  else{
	    if(tmp>alpha)alpha=tmp,x=i,y=j;
	  }
	  s[i][j]=0;
	}
  /*
  if(clock()-ti>5000){
    MAX_DEPTH--;
  }*/
  if(type==2)return beta;
  return alpha;
}

void GetXY(int &x,int &y) 
{
  CONSOLE_SCREEN_BUFFER_INFO pBuffer; 
  GetConsoleScreenBufferInfo(hout, &pBuffer); 
  x=pBuffer.dwCursorPosition.X;
  y=pBuffer.dwCursorPosition.Y; 
}

void SetXY(int x,int y)
{
  COORD p;
  p.X=x;p.Y=y;
  SetConsoleCursorPosition(hout,p);
}

void show(int x,int y,int c)
{
  int nowx,nowy;
  if(c==2)SetConsoleTextAttribute(hout,4);
  else SetConsoleTextAttribute(hout,3);
  s[x][y]=c;c=get(c);
  // set color
  GetXY(nowx,nowy); 
  SetXY(y*4+2,x*2+1);
  // set pos
  printf("%c",c);
  // output
  SetXY(nowx,nowy);
  SetConsoleTextAttribute(hout,7);
  // restore pos && color
}

void Set_Windows()
{
  SetConsoleTitle("WCY1122's Five Chess Game");
  SMALL_RECT rc={0,0,50,30};
  SetConsoleWindowInfo(hout,true ,&rc);
  // resize windows
}

void init()
{
  for(int i=1;i<=len;i++)
    for(int j=1;j<=len;j++)s[i][j]=0;
  for(int i=0;i<=len+1;i++)
  {
    s[0][i]=1;s[len+1][i]=1;
    s[i][0]=1;s[i][len+1]=1;
  } 
  print_board();
  show(6,6,2);
  show(6,7,2);
  show(7,6,3);
  show(6,5,3);
}

void Choose_Mode(int &mode,int &YOU_DEPTH,int &AI_DEPTH,int &YOU_METHOD,int &AI_METHOD)
{
  printf("Welcome to wcy1122's Five Chess Game!\n");
  printf("Input Mode, 0 you first, 1 AI first\n");
  while(1)
  {
  	printf("Mode: ");
	scanf("%d",&mode);
	if(mode==0||mode==1)break;
	printf("Invalid Mode!\n");
  }
  printf("Input Your Depth && AI Depth: ");
  scanf("%d %d",&YOU_DEPTH,&AI_DEPTH);
  YOU_DEPTH--;AI_DEPTH--;
  printf("Input Your Method && AI Method: ");
  scanf("%d %d",&YOU_METHOD,&AI_METHOD);
}

void clear(int posx,int posy,int w,int h)
{
  SetXY(posx,posy);
  for(int i=1;i<=w;i++)
  {
    for(int j=1;j<=h;j++)printf(" ");
    printf("\n");
  }
  SetXY(posx,posy);
}

void show_invalid()
{
  int posx,posy;
  GetXY(posx,posy);
  SetXY(0,posy-1);
  for(int i=1;i<=50;i++)printf(" ");
  SetXY(0,posy-1);
  printf("Invalid Input! Press Any Key to continue");
  getchar();
  getchar();
  SetXY(0,posy-1);
  for(int i=1;i<=50;i++)printf(" ");
  SetXY(0,posy-1);
}

int main()
{
  int x=0,y=0,WIN=0,Value=0,Score=0;
  int posx,posy,prex,prey;
  int MODE,YOU_COL,AI_COL;
  int YOU_DEPTH,AI_DEPTH;
  int YOU_METHOD,AI_METHOD; 
  Set_Windows();
  Choose_Mode(MODE,YOU_DEPTH,AI_DEPTH,YOU_METHOD,AI_METHOD);
  init();
  GetXY(posx,posy);
  if(MODE==0)YOU_COL=2,AI_COL=3;
  else YOU_COL=3,AI_COL=2;
  int steps=0;
  for(;;steps++)
  {
  	clear(posx,posy,5,50);
  	if(MODE==1) // AI first 
	{
	  Value=do_step(x,y,AI_COL,AI_METHOD,AI_DEPTH);
      show(x,y,AI_COL);;
      if(check_win()){WIN=AI_COL;break;}
	}
	// AI step
  	prex=x;prey=y;
    do_step(x,y,YOU_COL,YOU_METHOD,YOU_DEPTH);
    printf("Opponent's Position: (%d,%d)\n",prex,prey);
	printf("Opponent's Score: %d\n",Value);
	printf("Your Previous Score: %d\n",Score);
    printf("Recommend Pos: (%d,%d)\n",x,y);
    // Output
  	while(1)
  	{
      printf("Input Your Position: ");
	  scanf("%d %d",&x,&y);
      if(x>0&&x<=len&&y>0&&y<=len&&!s[x][y])break;
      show_invalid();
	}
	Score=evaluate(s,YOU_COL,YOU_METHOD);
    show(x,y,YOU_COL);
    if(check_win()){WIN=YOU_COL;break;}
    // your step
    if(MODE==0) // you first 
    {
      Value=do_step(x,y,AI_COL,AI_METHOD,AI_DEPTH);
      show(x,y,AI_COL);
      if(check_win()){WIN=AI_COL;break;}
	}
	// AI step
  }
  clear(posx,posy,5,50);
  printf("End with %d steps\n",steps);
  if(WIN==YOU_COL)printf("You win!\n");
  else printf("You lose!\n");
  system("pause");
  return 0;
} 
