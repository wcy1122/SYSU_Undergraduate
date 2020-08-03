#ifndef TRIE_H
#define TRIE_H 

#include "tokens.h"
#define MAXCHILD 100

typedef struct trie TrieNode;
struct trie
{
    TrieNode* Child[MAXCHILD];
    int ChildSize;
    char* NodeName;
    Token* NodeToken;
    char* Place;
};

void InitTrieNode(TrieNode *CurNode, char* Name){
    CurNode->ChildSize = 0;
    CurNode->NodeName = Name;
    CurNode->NodeToken = NULL;
}

void AddChild(TrieNode *CurNode, TrieNode *child){
    CurNode->Child[CurNode->ChildSize] = child;
    CurNode->ChildSize++;
}

void AddTerminal(TrieNode *CurNode, char* _name, Token *_token){
    TrieNode* NewNode = (TrieNode*)malloc(sizeof(TrieNode));
    NewNode->ChildSize = 0;
    NewNode->NodeName = _name;
    NewNode->NodeToken = _token;
    AddChild(CurNode, NewNode);
}

void DelChild(TrieNode *CurNode, int times){
    while(times--){
        CurNode->ChildSize--;
        CurNode->Child[CurNode->ChildSize] = NULL;
    }
}

void OutputTrie(TrieNode *CurNode, int dep){
    if(dep==0) printf("Parsing Tree : \n");
    for(int i=0;i<dep;i++) printf("  ");
    if(CurNode->ChildSize == 0) printf("-- ");
    printf("%s\n", CurNode->NodeName);
    for(int i=0;i<CurNode->ChildSize;i++)
        OutputTrie(CurNode->Child[i], dep+1);
}

#endif