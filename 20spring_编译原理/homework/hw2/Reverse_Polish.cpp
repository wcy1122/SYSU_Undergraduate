#include<bits/stdc++.h>
#define N 666072
using namespace std;
int len,tot;
char ans[N],str[N];

void solve(int l,int r)
{
  int sum=0,pos_add=0,pos_mul=0;
  for(int i=l;i<=r;i++)
  {
    if(str[i]=='(')sum++;
    else if(str[i]==')')sum--;
    else if(str[i]=='+'&&sum==0)pos_add=i;
	else if(str[i]=='*'&&sum==0)pos_mul=i;
  }
  if(pos_add){
    ans[++tot]='+';ans[++tot]=',';
	solve(pos_add+1,r);solve(l,pos_add-1);
  }
  else if(pos_mul){
  	ans[++tot]='*';ans[++tot]=',';
    solve(pos_mul+1,r);solve(l,pos_mul-1);
  }
  else if(str[l]=='(' && str[r]==')'){
    solve(l+1,r-1);
  }
  else{
    for(int i=r;i>=l;i--)
      ans[++tot]=str[i];
    ans[++tot]=',';
  }
}

int main()
{
  scanf("%s",str+1);
  len=strlen(str+1);
  solve(1,len);
  for(int i=tot-1;i;i--)printf("%c",ans[i]);
  printf("\n");
  return 0;
} 
