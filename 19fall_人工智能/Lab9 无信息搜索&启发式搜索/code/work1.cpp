#include<bits/stdc++.h>
#define inf 2100000000
#define N 50
using namespace std;
int n,m,Sx,Sy,Tx,Ty,f[N][N],flag[N][N],pre[N][N]; 
char s[N][N];

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

int uni(int Sx,int Sy,int Tx,int Ty)
{
  int t1[]={-1,1,0,0};
  int t2[]={0,0,-1,1};
  int l=1,r=2,steps=0;
  int X[N*N],Y[N*N]; 
  X[1]=Sx;Y[1]=Sy;flag[Sx][Sy]=1;
  while(l<r)
  {
    int x=X[l],y=Y[l];l++;
    for(int i=0;i<4;i++)
    {
	  int xx=x+t1[i],yy=y+t2[i];
	  if(xx<1||xx>n||yy<1||yy>m)continue;
	  if(!flag[xx][yy]&&s[xx][yy]!='1')
	  {
	    X[r]=xx;Y[r]=yy;r++;steps++; 
		f[xx][yy]=f[x][y]+1;
		flag[xx][yy]=1;pre[xx][yy]=i;
		if(xx==Tx&&yy==Ty)return steps;
	  }
	}
  }
}

int iter(int Sx,int Sy,int Tx,int Ty)
{
  int t1[]={-1,1,0,0};
  int t2[]={0,0,-1,1};
  int tot1=1,tot2=0,steps=0;
  int X[N*2],Y[N*2],A[N*2],B[N*2];
  X[1]=Sx;Y[1]=Sy;flag[Sx][Sy]=1;
  for(int dep=0;;dep++)
  {
  	for(int i=1;i<=tot1;i++)
  	{
  	  int x=X[i],y=Y[i];
	  for(int j=0;j<4;j++)
      {
	    int xx=x+t1[j],yy=y+t2[j];
		if(xx<1||xx>n||yy<1||yy>m)continue;
	    if(!flag[xx][yy]&&s[xx][yy]!='1')
	    {
	      A[++tot2]=xx;B[tot2]=yy;steps++;
		  f[xx][yy]=f[x][y]+1;
		  flag[xx][yy]=1;pre[xx][yy]=j;
		  if(xx==Tx&&yy==Ty)return steps;
	    }
	  }
	}
	for(int i=1;i<=tot2;i++)X[i]=A[i],Y[i]=B[i];
	tot1=tot2;tot2=0;
  }
}

int bi(int Sx,int Sy,int Tx,int Ty)
{
  int t1[]={-1,1,0,0};
  int t2[]={0,0,-1,1};
  int l=1,r=3,steps=0;
  int X[N*N],Y[N*N]; 
  X[1]=Sx;Y[1]=Sy;flag[Sx][Sy]=1;
  X[2]=Tx;Y[2]=Ty;flag[Tx][Ty]=2;
  while(l<r)
  {
    int x=X[l],y=Y[l];l++;
    for(int i=0;i<4;i++)
    {
	  int xx=x+t1[i],yy=y+t2[i];
	  if(xx<1||xx>n||yy<1||yy>m)continue;
	  if(s[xx][yy]=='1')continue;
	  if(flag[xx][yy]==flag[x][y])continue;
	  if(!flag[xx][yy])
	  {
	    X[r]=xx;Y[r]=yy;r++;
		f[xx][yy]=f[x][y]+1;
		flag[xx][yy]=flag[x][y];
		pre[xx][yy]=i;
		steps++;
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
  memset(f,0,sizeof(f));
  memset(pre,0,sizeof(pre));
  memset(flag,0,sizeof(flag));
} 

void solve1()
{
  clear();
  int steps=uni(Sx,Sy,Tx,Ty);
  printf("search done with %d steps.\n",steps);
  get_ans();
}

void solve2()
{
  clear();
  int steps=iter(Sx,Sy,Tx,Ty);
  printf("search done with %d steps.\n",steps);
  get_ans();
}

void solve3()
{
  clear();
  int steps=bi(Sx,Sy,Tx,Ty);
  printf("search done with %d steps.\n",steps);
  get_ans();
}

int main()
{
  Input();
  Find_ST();
  solve1();
  solve2();
  solve3();
  return 0;
} 
