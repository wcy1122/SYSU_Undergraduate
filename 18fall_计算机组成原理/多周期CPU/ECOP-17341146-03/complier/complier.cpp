#include<bits/stdc++.h>
using namespace std;

string get_num(int x,int len,int inv)
{
  string res;res.clear();
  for(;x;x>>=1,len--)res=res+(char)((x&1)+'0');
  while(len--)res=res+'0';
  reverse(res.begin(),res.end());
  if(inv)
  {
  	int p=res.size()-1;
    for(;res[p]=='0';p--);
    for(p--;p>=0;p--)res[p]=(res[p]-'0'^1)+'0';
  }
  return res;
}

int get_tp(string str)
{
  if(str=="add"||str=="sub"||str=="and"||str=="slt")return 0;
  if(str=="sll")return 1;
  if(str=="jr")return 2;
  if(str=="lw"||str=="sw")return 3;
  if(str=="bltz")return 4;
  if(str=="j"||str=="jal")return 6;
  if(str=="halt")return 7;
  return 5;
}

string get_op(string str)
{
  if(str=="add")   return "000000";
  if(str=="sub")   return "000001";
  if(str=="addiu") return "000010";
  if(str=="and")   return "010000";
  if(str=="andi")  return "010001";
  if(str=="ori")   return "010010";
  if(str=="xori")  return "010011";
  if(str=="sll")   return "011000";
  if(str=="slti")  return "100110";
  if(str=="slt")   return "100111";
  if(str=="sw")    return "110000";
  if(str=="lw")	   return "110001";
  if(str=="beq")   return "110100";
  if(str=="bne")   return "110101";
  if(str=="bltz")  return "110110";
  if(str=="j")     return "111000";
  if(str=="jr")    return "111001";
  if(str=="jal")   return "111010";
  if(str=="halt")   return "111111";
}
int get_hex(char ch){
  if('0'<=ch&&ch<='9')return ch-'0';
  return ch-'A'+10;
}
bool check(char ch){return '0'<=ch&&ch<='9';}

int main()
{
  freopen("ins.txt","r",stdin);
  freopen("input.txt","w",stdout);
  int tp,tot,now,inv,tg,num[5];string A,B,ans;
  while(cin>>A)
  {
    tp=get_tp(A);ans=get_op(A);
	if(tp<=5)
    {
	  cin>>B;tot=0;now=0;inv=0;tg=0;
	  for(int i=0;i<B.size();i++)
	  {
	  	if(check(B[i]))now=now*10+B[i]-'0',tg=1;
		else if(B[i]=='-')inv=1;
		else
		{
		  if(tg)num[++tot]=now;
		  now=0;tg=0;
		}
	  }
	  if(tg)num[++tot]=now;
	  if(tp==0)
	  {
	    ans=ans+get_num(num[2],5,0);
	    ans=ans+get_num(num[3],5,0);
	    ans=ans+get_num(num[1],5,0);
	  }
	  if(tp==1)
	  {
	    ans=ans+get_num(0,5,0);
	    ans=ans+get_num(num[2],5,0);
	    ans=ans+get_num(num[1],5,0);
	    ans=ans+get_num(num[3],5,0);
	  }
	  if(tp==2)
	  {
	    ans=ans+get_num(num[1],5,0);
	  }
	  if(tp==3)
	  {
	    ans=ans+get_num(num[3],5,0);
	    ans=ans+get_num(num[1],5,0);
	    ans=ans+get_num(num[2],16,inv);
	  }
	  if(tp==4)
	  {
	    ans=ans+get_num(num[1],5,0);
	    ans=ans+get_num(0,5,0);
	    ans=ans+get_num(num[2],16,inv);
	  }
	  if(tp==5)
	  {
	    ans=ans+get_num(num[2],5,0);
	    ans=ans+get_num(num[1],5,0);
	    ans=ans+get_num(num[3],16,inv);
	  }
	}
	if(tp==6)
	{
	  cin>>B;now=0;
	  for(int i=0;i<B.size();i++)
	  {
	    if(B[i]=='x')now=0;
	    else now=now*16+get_hex(B[i]);
	  }
	  ans=ans+get_num(now,26,0);
	}
	int len=ans.size();
	for(int i=len;i<32;i++)ans=ans+'0';
	for(int i=0;i<32;i++)
	{
	  printf("%c",ans[i]);
	  if(i%8==7)printf("  ");
	}
	//cout<<"//"<<A;if(tp<=6)cout<<"  "<<B; 
	printf("\n");
  }	
  return 0;
} 
