#ifndef __STRING_H
#define __STRING_H

int strlen(char *s)
{
  int len=0;
  while(s[len]!='\000')len++;
  return len;
}

int strcmp(char *a,char *b)
{
  int n=strlen(a),m=strlen(b),i;
  if(n!=m)return 0;
  for(i=0;i<n;i++)
    if(a[i]!=b[i])return 0;
  return 1;
}

char strcpy(char *a,char *b)
{
  int i,len=strlen(b);
  for(i=0;i<len;i++)a[i]=b[i];
  a[len]='\000';
}

int strcmp_prefix(char *a,char *b)
{
  int n=strlen(a),m=strlen(b),i;
  if(n<m)return 0;
  for(i=0;i<m;i++)
    if(a[i]!=b[i])return 0;
  return 1;
}

int string_to_int(char *s)
{
  int i,len=strlen(s),res=0;
  for(i=0;i<len;i++)res=res*10+s[i]-48;
  return res;
}

#endif
