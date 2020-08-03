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

void InitScanner(Scanner* Scan, char* _buf)
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
    if(pos>=len)return 0;
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
        printf("%s\n", buf);
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
    Scan->pos = pos+length;

    if(pos+length>len)return 0;
    return 1;
}

void GetPos(Scanner* Scan, int* _posx, int* _posy)
{
    (*_posx) = 0; (*_posy) = 0;
    for(int i=0;i<Scan->pos;i++){
        if(Scan->buf[i]=='\n')(*_posx)++, (*_posy)=0;
        else (*_posy)++;
    }
}

void SetErrorScan(ErrType _type, char* _buf, Scanner* Scan)
{
    int Posx, Posy;
    GetPos(Scan, &Posx, &Posy);
    Error.ErrTp = _type;
    Error.ErrInfo = _buf;
    Error.Posx = Posx;
    Error.Posy = Posy;
}

ErrType NextToken(Scanner* Scan, TokenList* ToList)
{
    char ch = NextChar(Scan);
    int Length = -1;
    TokenType TokenTp;
    char* now = NULL;
    int _posx, _posy;
    if(!ch)return 1;

    GetPos(Scan, &_posx, &_posy);
    if(isdigit(ch)){
        Length = GetFullNumber(Scan);
        if(Length<0) {
            SetErrorScan(KeyError, "invalid number", Scan);
            return KeyError;
        }
        TokenTp = _Number;
    }
    else if((ch=='+' || ch=='-') && isdigit(GetNow(Scan))){
        Length = GetFullNumber(Scan);
        if(Length<0) {
            SetErrorScan(KeyError, "invalid number", Scan);
            return KeyError;
        }
        Length++;
        TokenTp = _Number;
    }
    else if(isletter(ch)){
        Length = GetFullKey(Scan);
        if(Length<0) {
            SetErrorScan(KeyError, "invalid key", Scan);
            return KeyError;
        }
        TokenTp = _Key;
    }
    else if(ch=='/' && GetNow(Scan)=='*'){
        int res = GetSpecify2(Scan, "*/");
        if(!res) {
            SetErrorScan(AnnError, "invalid annotation", Scan);
            return AnnError;
        }
        return Success;
    }
    else if(ch=='/' && GetNow(Scan)=='/'){
        int res = GetSpecify1(Scan, '\n');
        if(!res)return CodeEnd;
        return Success;
    }
    else if(ch=='('){
        int pos = Scan->pos;
        int res = GetSpecify1(Scan, ')');
        if(!res) {
            SetErrorScan(SynError, "missing terminating ')'character", Scan);
            return SynError;
        }
        Scan->pos = pos;
        Length = 1;
        TokenTp = _Reserved;
    }
    else if(ch=='\"' || ch=='\''){
        int st = Scan->pos-1;
        int res = GetSpecify1(Scan, ch);
        if(res) Length = Scan->pos - st;
        else {
            if(ch=='\"') SetErrorScan(SynError, "missing terminating '\"' character", Scan);
            else SetErrorScan(SynError, "missing terminating '\'' character", Scan);
            return SynError;
        }
        if(ch=='\'' && Length>3) {
            SetErrorScan(SynError, "multi-character character constant", Scan);
            return SynError;
        }
        if(ch=='\"') TokenTp = _String;
        else TokenTp = _Char;
    }
    else if(isToken(ch) && (isspace(GetNow(Scan)) || Scan->pos==Scan->length)){
        Length = 1;
        TokenTp = _Reserved;
    }
    else if(isEqu(ch) && GetNow(Scan)=='='){
        Move(Scan, 1);
        Length = 2;
        TokenTp = _Reserved;
    }
    else {
        SetErrorScan(UknError, "unknown error", Scan);
        return UknError;
    }

    now = Copy(Scan, Length);

    if(TokenTp==_Key){
        if(!strcmp(now,"int") || !strcmp(now,"float") || \
           !strcmp(now,"string") || !strcmp(now,"char")){
            TokenTp = _Type;
        }
        else if(!strcmp(now,"if") || !strcmp(now,"then")   || \
            !strcmp(now,"else")   || !strcmp(now,"repeat") || \
            !strcmp(now,"until")  || !strcmp(now,"write")  || \
            !strcmp(now,"read")   || !strcmp(now,"endif") ){
            TokenTp = _Reserved;
        }
    }
    
    Token* CurToken = (Token*)malloc(1);
    CurToken->string = now;
    CurToken->type = TokenTp;
    CurToken->Posx = _posx;
    CurToken->Posy = _posy;

    bool res = Insert(ToList, CurToken);
    if(!res) {
        SetErrorScan(MemError, "code too long", Scan);
        return MemError;
    }

    return Success;
}

#endif