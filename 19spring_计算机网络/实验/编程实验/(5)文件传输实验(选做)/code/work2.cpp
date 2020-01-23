#include<bits/stdc++.h>
#define M 300
#include<windows.h>
#include<fcntl.h>
#include<io.h>
using namespace std;
int size,len;
FILE *srcfile,*destfile;
char buf[20],srcfname[M],str[M],filename[M],des[M];
char sss[100000];
char destfname[M]="FileSet.pak";

void adjust(char *srcfname,char *filename)
{
  int len=strlen(srcfname),pos=len-1,cnt=0;
  for(;pos>=0;pos--)if(srcfname[pos]=='/')break;
  for(int i=pos+1;i<len;i++)filename[cnt++]=srcfname[i];
  filename[cnt]='\0';
}

void compress()
{
  destfile=fopen(destfname,"wb"); 
  if(destfile==NULL) 
  {
	printf("Write file no found!\n");
	printf("Press any key to continue...");
	getchar();exit(1);
  }
  //open the destfile 
  printf("Please input the file name\n");
  printf("Press Ctrl+z to stop\n");
  while(~scanf(" %s",srcfname))
  {
	srcfile=fopen(srcfname,"rb");
	if(srcfile==NULL)
	{
	  printf("Read file no found!\n");
	  printf("Press any key to continue...");
	  getchar();continue;
	}
	adjust(srcfname,filename);
	size=filelength(fileno(srcfile));
	fwrite(filename,sizeof(filename),1,destfile);
	fwrite(&size,sizeof(int),1,destfile);
	while((len=fread(buf,1,20,srcfile))>=20)
	  fwrite(buf,1,20,destfile);
	//input file 
	fwrite(buf,1,len,destfile);
	fclose(srcfile);
	//compress file 
  }
  fclose(destfile);
  printf("Compress finished!\n");
  printf("Press any key to continue...");
  getchar();printf("\n");
}
//compress

void get_filename(char *des,char *str,char *filename)
{
  FILE *fp;char type[M];
  int pos=strlen(str),size=0;
  sprintf(filename,"%s%s",des,str);
  fp=fopen(filename,"rb");
  if(fp==NULL)return;
  //no confilct
  while(str[pos]!='.')type[size++]=str[pos--];
  str[pos]='\0';type[size]='\0';
  reverse(type,type+size);
  //get type and filename 
  for(int i=2;;i++)
  {
	sprintf(filename,"%s%s(%d).%s",des,str,i,type);
	fp=fopen(filename,"rb");
    if(fp==NULL)return;
  }
}

void umcompress() 
{
  char ch;
  printf("input the folder name\n");
  scanf(" %s",str);
  sprintf(des,".\\%s\\",str);mkdir(str);
  //set up folder 
  srcfile=fopen(destfname,"rb");
  while(fread(str,sizeof(str),1,srcfile)==1)
  {
  	get_filename(des,str,filename);
	destfile=fopen(filename,"wb");
    fread(&size,sizeof(int),1,srcfile);
	while(size--)
    {
	  fread(&ch,1,1,srcfile);
	  fwrite(&ch,1,1,destfile);
	}
	fclose(destfile);
  }
  fclose(srcfile);
  printf("Uncompress finished!\r\n");
  printf("Press any key to continue...");
  getchar();
}
//umcompress

int main()
{
  int ch; 
  printf("input 1 for compress only\n");
  printf("input 2 for umcompress only\n");
  printf("input 3 for both compress and uncompress\n");
  scanf("%d",&ch);
  if(ch&1)compress(); 
  if((ch>>1)&1)umcompress();
  return 0;
}
