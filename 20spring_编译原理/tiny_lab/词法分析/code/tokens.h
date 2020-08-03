#ifndef TOKENS_H
#define TOKENS_H  

#include<stdio.h>
#include<stdbool.h>
#define MAXN 10000

typedef struct TokenList{
    int size;
    char* list[MAXN];
}TokenList;

bool Insert(TokenList* Tokens, char* buf){
    if(Tokens->size >= MAXN){
        return false;
    }
    Tokens->list[Tokens->size] = buf;
    Tokens->size++;
    return true;
}

void OutputTokens(TokenList* Tokens){
    printf("Tokens:\n");
    for(int i=0;i<Tokens->size;i++)
        printf("%s ", Tokens->list[i]);
    printf("\n");
}

#endif