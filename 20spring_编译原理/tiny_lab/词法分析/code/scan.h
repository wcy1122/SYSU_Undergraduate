#ifndef SCAN_H
#define SCAN_H   

#include<string.h>
#include<stdio.h>
#include<ctype.h>

#include "utils.h"
#include "error.h"
#include "tokens.h"

typedef struct Scanner
{
    int pos;
    int length;
    char* buf;
}Scanner;

void Init(Scanner* Scan, char* _buf)
{
    Scan->pos = 0;
    Scan->length = strlen(_buf);
    Scan->buf = _buf;
}

char NextChar(Scanner* Scan)
{
    int pos = Scan->pos;
    int len = Scan->length;
    char* buf = Scan->buf;
    for(;pos<len;pos++)
        if(!isspace(buf[pos]))break;
    Scan->pos = pos+1;
    if(pos>len)return 0;
    return buf[pos];
}

int GetFullNumber(Scanner* Scan){
    int pos = Scan->pos - 1;
    int len = Scan->length;
    char* buf = Scan->buf;
    if(buf[pos]=='+' || buf[pos]=='-') pos++;
    int st = pos;
    int flag = 0;
	
	if(buf[pos]=='0' && !(isspace(buf[pos+1]) || buf[pos+1]=='.' || buf[pos+1]==';'))
		return -1;

    for(;pos<len;pos++)
    {
        if(isspace(buf[pos])) break;
        if(buf[pos] == ';') break;
        if(isdigit(buf[pos])) continue;
        if(buf[pos]=='.' && flag==0){
			if(!isdigit(buf[pos-1]) || !isdigit(buf[pos+1]))
				return -1;
            flag=1; continue;
        }
        if(flag<2 && (buf[pos]=='e' || buf[pos]=='E')){
			if(!isdigit(buf[pos-1])) return -1;
            if(buf[pos+1]!='+' && buf[pos+1]!='-') return -1;
            if(!isdigit(buf[pos+2])) return -1;
            flag=2; pos+=2; continue;
        }
        return -1;
    }
	
	Scan->pos = pos;
    int NumLen = pos - st;
    return NumLen;
}

int GetFullKey(Scanner* Scan){
    int pos = Scan->pos - 1;
    int st = pos;
    int len = Scan->length;
    char* buf = Scan->buf;

    for(;pos<len;pos++)
    {
        if(isspace(buf[pos])) break;
        if(buf[pos] == ';') break;
        if(isletter(buf[pos])) continue;
        if(isdigit(buf[pos])) continue;
        return -1;
    }
    Scan->pos = pos;

    int NumLen = pos - st;
    return NumLen;
}

char GetNow(Scanner* Scan){
    return Scan->buf[Scan->pos];
}

void Move(Scanner* Scan, int num){
    Scan->pos+=num;
}

char* Copy(Scanner* Scan, int len){
    int pos = Scan->pos;
    char *now = malloc(len);
    for(int i=pos-len,j=0;i<pos;i++,j++){
        now[j] = Scan->buf[i];
    }
    now[len] = '\0';
    return now;
}

bool GetSpecify1(Scanner* Scan, char ch){
    int pos = Scan->pos;
    int st = pos;
    int len = Scan->length;
    char* buf = Scan->buf;

    for(;pos<len;pos++)
        if(buf[pos] == ch) break;
    Scan->pos = pos+1;

    if(pos>=len)return 0;
    return 1;
}

bool GetSpecify2(Scanner* Scan, char* str){
    int pos = Scan->pos;
    int st = pos;
    int len = Scan->length;
    char* buf = Scan->buf;
    int length = strlen(str);

    for(;pos+length<len;pos++)
        if(StrCmp(buf+pos, str)) break;
    Scan->pos = pos+length+1;

    if(pos+length>=len)return 0;
    return 1;
}

int NextToken(Scanner* Scan, TokenList* ToList)
{
    char ch = NextChar(Scan);
    int ret = -1;
    char* now = NULL;
    if(!ch)return 1;

    if(isdigit(ch)){
        ret = GetFullNumber(Scan);
        if(ret<0)return KeyError;
    }
    else if((ch=='+' || ch=='-') && isdigit(GetNow(Scan))){
        ret = GetFullNumber(Scan);
        if(ret<0) return KeyError;
        ret++;
    }
    else if(isletter(ch)){
        ret = GetFullKey(Scan);
        if(ret<0) return KeyError;
    }
    else if(ch=='/' && GetNow(Scan)=='*'){
        int res = GetSpecify2(Scan, "*/");
        if(!res)return AnnError;
        return 0;
    }
    else if(ch=='/' && GetNow(Scan)=='/'){
        int res = GetSpecify1(Scan, '\n');
        if(!res)return 1;
        return 0;
    }
    else if(ch=='('){
        int pos = Scan->pos;
        int res = GetSpecify1(Scan, ')');
        if(!res)return SynError;
        Scan->pos = pos;
        ret = 1;
    }
    else if(ch=='\"' || ch=='\''){
        int st = Scan->pos-1;
        int res = GetSpecify1(Scan, ch);
        if(res) ret = Scan->pos - st;
        if(ch=='\'' && ret>3) return SynError;
    }
    else if(isToken(ch) && isspace(GetNow(Scan))){
        ret = 1;
    }
    else if(isEqu(ch) && GetNow(Scan)=='='){
        Move(Scan, 1);
        ret = 2;
    }
    else return UknError;
    
    now = Copy(Scan, ret);
	printf("%s\n", now);
    bool res = Insert(ToList, now);
    if(!res) return 0;

    return 0;
}
// 1:end 0:normal -1:error

#endif