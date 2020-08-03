#include<bits/stdc++.h>
#define N 666072
using namespace std;
int len,top;
char sgn[N],str[N];

int level(char ch)
{
  if(ch=='(')return 0;
  if(ch=='*'||ch=='/')return 2;
  if(ch=='+'||ch=='-')return 1;
  printf("--%c--",ch);
  while(1);
  assert(0);
}

int main()
{
  scanf("%s",str+1);
  len=strlen(str+1);
  for(int i=1;i<=len;i++)
  {
    if('0'<=str[i] && str[i]<='9')
    {
	  for(;'0'<=str[i]&&str[i]<='9';i++)
	  	printf("%c",str[i]);
	  printf(",");
	  i--;
	}
	else if(str[i]=='(')
	{
	  sgn[++top]=str[i];
	}
	else if(str[i]==')')
	{
	  for(;sgn[top]!='(';top--)
	    printf("%c,",sgn[top]);
	  top--;
	}
	else
	{
	  for(;top && level(str[i])<=level(sgn[top]);top--)
	  	printf("%c,",sgn[top]);
	  sgn[++top]=str[i];
	}
  }
  for(;top;top--)
  {
  	printf("%c",sgn[top]);
    if(top)printf(",");
  }
  printf("\n");
  return 0;
} 
