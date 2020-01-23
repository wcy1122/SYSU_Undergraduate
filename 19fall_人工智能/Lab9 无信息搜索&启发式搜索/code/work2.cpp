#include<bits/stdc++.h>
#define inf 2100000000
#define N 50
#define M 2500
using namespace std;
int n,m,Sx,Sy,Tx,Ty,pre[N][N],flag[N][N];
char s[N][N];
struct info{
  int x,y,dis,val;
  bool operator<(const info &p)const{
    return val>p.val;
  }
};
priority_queue<info>q;

void Input()
{
  FILE *fp=fopen("MazeData.txt","r");
  while(~fscanf(fp,"%s",s[++n]+1));
  fclose(fp);
  m=strlen(s[1]+1);
}

void Find_ST()
{ 
  for(int i=1;i<=n;i++)
    for(int j=1;j<=m;j++)
    {
	  if(s[i][j]=='S')Sx=i,Sy=j;
	  if(s[i][j]=='E')Tx=i,Ty=j;
	}
}

int calc(int X1,int Y1,int X2,int Y2)
{
  return abs(X1-X2)+abs(Y1-Y2);
} 

int astar(int Sx,int Sy,int Tx,int Ty,int type=0)
{
  int t1[]={-1,1,0,0};
  int t2[]={0,0,-1,1};
  q.push((info){Sx,Sy,0,calc(Sx,Sy,Tx,Ty)});
  flag[Sx][Sy]=1;
  if(type)
  {
    q.push((info){Tx,Ty,0,calc(Sx,Sy,Tx,Ty)});
    flag[Tx][Ty]=2;
  }
  int steps=0,x,y,dis;
  while(1)
  {
    info tmp=q.top();q.pop();
    x=tmp.x;y=tmp.y;dis=tmp.dis;
    if(!type&&x==Tx&&y==Ty)break;
    for(int i=0;i<4;i++)
    {
	  int xx=x+t1[i],yy=y+t2[i];
	  if(xx<1||xx>n||yy<1||yy>m)continue;
	  if(s[xx][yy]=='1')continue;
	  if(flag[xx][yy]==flag[x][y])continue;
	  if(!flag[xx][yy])
	  {
	    if(flag[x][y]==1){
		  q.push((info){xx,yy,dis+1,dis+1+calc(xx,yy,Tx,Ty)});
		}
		else{
		  q.push((info){xx,yy,dis+1,dis+1+calc(xx,yy,Sx,Sy)});
		}
		flag[xx][yy]=flag[x][y];pre[xx][yy]=i;steps++;
	  }
	  else
	  {
	  	int prep=i;
		if(flag[x][y]==1)x=xx,y=yy;
		else prep=(i^1);
	  	while(1) 
	  	{
		  if(x==Tx&&y==Ty)break;
		  int xx=x-t1[pre[x][y]];
    	  int yy=y-t2[pre[x][y]];
    	  int tmp=pre[x][y];
		  pre[x][y]=prep;
    	  prep=(tmp^1);
		  x=xx;y=yy;
		}
		pre[Tx][Ty]=prep; 
	    return steps;
	  }
	}
  }
  return steps;
}

void get_ans()
{
  int t1[]={-1,1,0,0};
  int t2[]={0,0,-1,1};
  int x=Tx,y=Ty;
  vector<int>ansx,ansy;
  while(1)
  {
    ansx.push_back(x);
    ansy.push_back(y);
    if(s[x][y]=='0')s[x][y]='#';
	if(x==Sx&&y==Sy)break;
	int xx=x-t1[pre[x][y]];
    int yy=y-t2[pre[x][y]];
	x=xx;y=yy;
  }
  for(int i=1;i<=n;i++)printf("%s\n",s[i]+1);
  printf("--------------------\n");
  for(int i=ansx.size()-1;i;i--)
    printf("(%d,%d) -> ",ansx[i],ansy[i]);
  printf("(%d,%d)\n",ansx[0],ansy[0]);
  printf("--------------------\n");
}

void clear()
{
  memset(pre,0,sizeof(pre));
  memset(flag,0,sizeof(flag));
}

void solve(int type)
{
  clear();
  int steps=astar(Sx,Sy,Tx,Ty,type);
  printf("search done with %d steps.\n",steps);
  get_ans();
}

int main()
{
  Input();
  Find_ST();
  solve(0);
  solve(1);
  return 0;
} 
