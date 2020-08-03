#ifndef CGEN_H
#define CGEN_H

#include "trie.h"

int Lineno = 0;
char *MidCode[10000];

void AddMidCode(char* str, int pos)
{
    int len = strlen(str);
    char* _str = (char*)malloc(len);
    strcpy(_str, str);
    _str[len] = '\0';
    MidCode[pos] = _str;
}

bool IsExp(TrieNode* CurNode)
{
    if(!strcmp(CurNode->NodeName, "Exp0") || \
       !strcmp(CurNode->NodeName, "Exp1") || \
       !strcmp(CurNode->NodeName, "Expression") ) {
        return CurNode->ChildSize>=3;
    }
    return false;
}

bool IsAssign(TrieNode* CurNode)
{
    return !strcmp(CurNode->NodeName, "Assignment");
}

bool IsStmt(TrieNode* CurNode)
{
    if(!strcmp(CurNode->NodeName, "IfStmt") || \
       !strcmp(CurNode->NodeName, "ReturnStmt") || \
       !strcmp(CurNode->NodeName, "RepeatStmt") || \
       !strcmp(CurNode->NodeName, "ReadStmt") || \
       !strcmp(CurNode->NodeName, "WriteStmt") ) {
        return true;
    }
    return false;
}

void CodeGen(TrieNode*);

void GenExp(TrieNode *CurNode)
{
    CodeGen(CurNode->Child[0]);
    CodeGen(CurNode->Child[2]);
    char* P = CurNode->Place;
    char* A = CurNode->Child[0]->Place;
    char* B = CurNode->Child[1]->NodeName;
    char* C = CurNode->Child[2]->Place;
    char str[100];
    sprintf(str, "%d: %s := %s %s %s", Lineno, P, A, B, C);
    AddMidCode(str, Lineno++);
}

void GenAssign(TrieNode *CurNode)
{
    CodeGen(CurNode->Child[0]);
    CodeGen(CurNode->Child[2]);
    char* A = CurNode->Child[0]->Place;
    char* B = CurNode->Child[2]->Place;
    char str[100];
    sprintf(str, "%d: %s := %s", Lineno, A, B);
    AddMidCode(str, Lineno++);
}

void GenStmt(TrieNode *CurNode)
{
    char* _name = CurNode->NodeName;
    char *A, *B, *C, str[100];

    if(!strcmp(_name, "IfStmt")) {
        TrieNode* Exp = CurNode->Child[1];
        CodeGen(Exp->Child[0]);
        CodeGen(Exp->Child[2]);

        // if
        A = Exp->Child[0]->Place;
        B = Exp->Child[1]->NodeName;
        C = Exp->Child[2]->Place;
        sprintf(str, "%d: If %s %s %s, goto %d", Lineno, A, B, C, Lineno+2);
        AddMidCode(str, Lineno); Lineno+=2;
        int PreLine = Lineno-1;

        // then
        CodeGen(CurNode->Child[3]);

        // else
        if(!strcmp(CurNode->Child[4]->NodeName, "else")){
            int IfLine = Lineno; Lineno++;
            sprintf(str, "%d: IfFalse %s %s %s, goto %d", PreLine, A, B, C, Lineno);
            AddMidCode(str, PreLine);
            CodeGen(CurNode->Child[5]);
            sprintf(str, "%d: goto %d", IfLine, Lineno);
            AddMidCode(str, IfLine);
        }
        else {
            sprintf(str, "%d: IfFalse %s %s %s, goto %d", PreLine, A, B, C, Lineno);
            AddMidCode(str, PreLine);
        }
    }
    else if(!strcmp(_name, "ReadStmt")) {
        sprintf(str, "%d: IN %s", Lineno, CurNode->Child[1]->NodeName);
        AddMidCode(str, Lineno++);
    }
    else if(!strcmp(_name, "WriteStmt")) {
        sprintf(str, "%d: OUT %s", Lineno, CurNode->Child[1]->NodeName);
        AddMidCode(str, Lineno++);
    }
    else if(!strcmp(_name, "RepeatStmt")) {
        // repeat
        int PreLine = Lineno;
        CodeGen(CurNode->Child[1]);
        // until
        TrieNode* Exp = CurNode->Child[3];
        CodeGen(Exp->Child[0]);
        CodeGen(Exp->Child[2]);
        A = Exp->Child[0]->Place;
        B = Exp->Child[1]->NodeName;
        C = Exp->Child[2]->Place;
        sprintf(str, "%d: If %s %s %s, goto %d", Lineno, A, B, C, PreLine);
        AddMidCode(str, Lineno++);
    }
}

void CodeGen(TrieNode *CurNode)
{
    if(IsStmt(CurNode)) {
        GenStmt(CurNode); return;
    }
    if(IsExp(CurNode)) {
        GenExp(CurNode); return;
    }
    if(IsAssign(CurNode)) {
        GenAssign(CurNode); return;
    }
    for(int i=0;i<CurNode->ChildSize;i++)
        CodeGen(CurNode->Child[i]);
}

void OutputMidCode()
{
    printf("MidCode:\n");
    for(int i=0;i<Lineno;i++)
        printf("%s\n", MidCode[i]);
}

#endif