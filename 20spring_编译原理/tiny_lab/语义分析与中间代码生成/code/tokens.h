#ifndef TOKENS_H
#define TOKENS_H  

#include<stdio.h>
#include<stdbool.h>
#include "scan.h"
#define MAXN 10000

typedef enum TokenType{
    _Key, _Number, _String, _Char, _Type, _Reserved
}TokenType;

typedef struct Token{
    char* string;
    TokenType type;
    int Posx, Posy;
}Token;

typedef struct TokenList{
    int size;
    int CurPos;
    Token* list[MAXN];
}TokenList;

void InitTokenList(TokenList *ToList){
    ToList->size = 0;
    ToList->CurPos = 0;
}

// parse

Token* GetCurToken(TokenList* Tokens){
    if(Tokens->CurPos >= Tokens->size) return NULL;
    return Tokens->list[Tokens->CurPos];
}

void MoveToken(TokenList* Tokens, int times){
    Tokens->CurPos+=times;
}

void BackToken(TokenList* Tokens, int times){
    Tokens->CurPos-=times;
}

// Generate
bool Insert(TokenList* Tokens, Token* CurToken){
    if(Tokens->size >= MAXN){
        return false;
    }
    Tokens->list[Tokens->size] = CurToken;
    Tokens->size++;
    return true;
}

void OutputTokens(TokenList* Tokens){
    printf("Tokens:\n");
    for(int i=0;i<Tokens->size;i++)
        printf("%s ", Tokens->list[i]->string);
    printf("\n");
}

void SetErrorToken(ErrType _type, char* _buf, Token* tok)
{
    Error.ErrTp = _type;
    Error.ErrInfo = _buf;
    Error.Posx = tok->Posx;
    Error.Posy = tok->Posy;
}

#endif