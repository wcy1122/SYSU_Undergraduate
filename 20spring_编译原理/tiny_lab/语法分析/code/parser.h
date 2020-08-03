#ifndef PARSER_H
#define PARSER_H 

#include "trie.h"
#include "error.h"
#include "scan.h"
#include "tokens.h"

typedef struct trie TrieNode;

static TrieNode* Parsing(TokenList*);
static TrieNode* Func(TokenList*);
static TrieNode* Vars(TokenList*);
static TrieNode* ForMalParams(TokenList*);
static TrieNode* ForMalParam(TokenList*);
static TrieNode* Block(TokenList*);
static TrieNode* Statement(TokenList*);
static TrieNode* Assignment(TokenList*);
static TrieNode* IfStmt(TokenList*);
static TrieNode* ReturnStmt(TokenList*);
static TrieNode* RepeatStmt(TokenList*);
static TrieNode* ReadStmt(TokenList*);
static TrieNode* WriteStmt(TokenList*);
static TrieNode* Expression(TokenList*);
static TrieNode* Exp0(TokenList*);
static TrieNode* Exp1(TokenList*);
static TrieNode* Exp2(TokenList*);

TrieNode* Parsing(TokenList* ToList){
    Token* CurToken;
    TrieNode* ChildNode;
    TrieNode* CurNode = (TrieNode*)malloc(sizeof(TrieNode));
    InitTrieNode(CurNode, "Root");

    while(1){
        CurToken = GetCurToken(ToList);
        if(CurToken==NULL || CurToken->type != _Type) break;
        MoveToken(ToList, 1);

        CurToken = GetCurToken(ToList);
        if(CurToken==NULL || CurToken->type != _Key) {
            BackToken(ToList, 1); break;
        }
        MoveToken(ToList, 1);
        
        CurToken = GetCurToken(ToList);
        if(CurToken==NULL){
            BackToken(ToList, 2); break;
        }
        BackToken(ToList, 2);
        
        char* CurStr = CurToken->string;
        if(!strcmp(CurStr, "(")){
            ChildNode = Func(ToList);
        }
        else if(!strcmp(CurStr, ",") || !strcmp(CurStr, ";")){
            ChildNode = Vars(ToList);
        }
        else break;
        if(ChildNode == NULL) break;
        AddChild(CurNode, ChildNode);
    }
    
    CurToken = GetCurToken(ToList);
    if(CurToken == NULL) return CurNode;
    SetErrorToken(SynError, "expect ';' after vars", GetCurToken(ToList));
    return NULL;
}

TrieNode* Func(TokenList* ToList){
    Token* CurToken;
    TrieNode* ChildNode;
    TrieNode* CurNode = (TrieNode*)malloc(sizeof(TrieNode));
    InitTrieNode(CurNode, "Func");

    CurToken = GetCurToken(ToList);
    if(CurToken==NULL || CurToken->type != _Type) {
        SetErrorToken(SynError, "need return type before func", CurToken);
        return NULL;
    }
    AddTerminal(CurNode, CurToken->string, CurToken);
    MoveToken(ToList, 1);

    CurToken = GetCurToken(ToList);
    if(CurToken==NULL || CurToken->type != _Key) {
        SetErrorToken(SynError, "function without name", CurToken);
        return NULL;
    }
    AddTerminal(CurNode, CurToken->string, CurToken);
    MoveToken(ToList, 1);

    CurToken = GetCurToken(ToList);
    if(CurToken==NULL || strcmp(CurToken->string, "(")) {
        SetErrorToken(SynError, "function without args", CurToken);
        return NULL;
    }
    AddTerminal(CurNode, CurToken->string, CurToken);
    MoveToken(ToList, 1);

    ChildNode = ForMalParams(ToList);
    if(ChildNode != NULL)
        AddChild(CurNode, ChildNode);

    CurToken = GetCurToken(ToList);
    if(strcmp(CurToken->string, ")")) {
        SetErrorToken(SynError, "missing terminating /')/'character", CurToken);
        return NULL;
    }
    AddTerminal(CurNode, CurToken->string, CurToken);
    MoveToken(ToList, 1);

    ChildNode = Block(ToList);
    if(ChildNode == NULL) {
        SetErrorToken(SynError, "function without block", CurToken);
        return NULL;
    }
    AddChild(CurNode, ChildNode);

    return CurNode;
}

TrieNode* Vars(TokenList* ToList){
    Token* CurToken;
    TrieNode* CurNode = (TrieNode*)malloc(sizeof(TrieNode));
    InitTrieNode(CurNode, "Vars");

    CurToken = GetCurToken(ToList);
    if(CurToken==NULL || CurToken->type != _Type) {
        SetErrorToken(SynError, "need return type before vars", CurToken);
        return NULL;
    }
    AddTerminal(CurNode, CurToken->string, CurToken);
    MoveToken(ToList, 1);

    CurToken = GetCurToken(ToList);
    if(CurToken==NULL || CurToken->type != _Key) {
        SetErrorToken(SynError, "vars without key", CurToken);
        return NULL;
    }
    AddTerminal(CurNode, CurToken->string, CurToken);
    MoveToken(ToList, 1);
    
    while(1){
        CurToken = GetCurToken(ToList);
        if(CurToken==NULL || strcmp(CurToken->string, ",")) break;
        AddTerminal(CurNode, CurToken->string, CurToken);
        MoveToken(ToList, 1);

        CurToken = GetCurToken(ToList);
        if(CurToken==NULL || CurToken->type!=_Key) {
            BackToken(ToList, 1); DelChild(CurNode, 1); break;
        }
        AddTerminal(CurNode, CurToken->string, CurToken);
        MoveToken(ToList, 1);
    }

    CurToken = GetCurToken(ToList);
    if(CurToken==NULL || strcmp(CurToken->string, ";")) {
        SetErrorToken(SynError, "need a ';' after statement", CurToken);
        return NULL;
    }
    AddTerminal(CurNode, CurToken->string, CurToken);
    MoveToken(ToList, 1);
    
    return CurNode;
}

TrieNode* ForMalParams(TokenList* ToList){
    Token* CurToken;
    TrieNode* ChildNode;
    TrieNode* CurNode = (TrieNode*)malloc(sizeof(TrieNode));
    InitTrieNode(CurNode, "ForMalParams");

    ChildNode = ForMalParam(ToList);
    if(ChildNode==NULL) return NULL;
    AddChild(CurNode, ChildNode);

    while(1){
        CurToken = GetCurToken(ToList);
        if(CurToken==NULL || CurToken->string!=",") break;
        AddTerminal(CurNode, CurToken->string, CurToken);
        MoveToken(ToList, 1); 

        ChildNode = ForMalParam(ToList);
        if(ChildNode==NULL){
            BackToken(ToList, 1); break;
        }
        AddChild(CurNode, ChildNode);
    }

    return CurNode;
}

TrieNode* ForMalParam(TokenList* ToList){
    Token* CurToken;
    TrieNode* CurNode = (TrieNode*)malloc(sizeof(TrieNode));
    InitTrieNode(CurNode, "ForMalParam");

    CurToken = GetCurToken(ToList);
    if(CurToken==NULL || CurToken->type!=_Type) {
        SetErrorToken(SynError, "key without type", CurToken);
        return NULL;
    }
    AddTerminal(CurNode, CurToken->string, CurToken);
    MoveToken(ToList, 1);
    CurToken = GetCurToken(ToList);
    if(CurToken==NULL || CurToken->type!=_Key) {
        SetErrorToken(SynError, "vars without key", CurToken);
        return NULL;
    }
    AddTerminal(CurNode, CurToken->string, CurToken);
    MoveToken(ToList, 1);

    return CurNode;
}

TrieNode* Block(TokenList* ToList){
    Token* CurToken;
    TrieNode* ChildNode;
    TrieNode* CurNode = (TrieNode*)malloc(sizeof(TrieNode));
    InitTrieNode(CurNode, "Block");

    while(1){
        ChildNode = Statement(ToList);
        if(ChildNode==NULL) break;
        AddChild(CurNode, ChildNode);
    }

    return CurNode;
}

TrieNode* Statement(TokenList* ToList){
    Token* CurToken;
    TrieNode* ChildNode;
    TrieNode* CurNode = (TrieNode*)malloc(sizeof(TrieNode));
    InitTrieNode(CurNode, "Statement");

    CurToken = GetCurToken(ToList);
    if(CurToken==NULL) return NULL;
    char* CurStr = CurToken->string;

    if(CurToken->type==_Type){
        ChildNode = Vars(ToList);
    } else if(CurToken->type==_Key){
        ChildNode = Assignment(ToList);
    } else if(!strcmp(CurStr, "if")){
        ChildNode = IfStmt(ToList);
    } else if(!strcmp(CurStr, "return")){
        ChildNode = ReturnStmt(ToList);
    } else if(!strcmp(CurStr, "repeat")){
        ChildNode = RepeatStmt(ToList);
    } else if(!strcmp(CurStr, "read")){
        ChildNode = ReadStmt(ToList);
    } else if(!strcmp(CurStr, "write")){
        ChildNode = WriteStmt(ToList);
    } else {
        SetErrorToken(UknError, "unknown statement", CurToken);
        return NULL;
    }

    if(ChildNode==NULL) return NULL;
    AddChild(CurNode, ChildNode);

    return CurNode;
}

TrieNode* Assignment(TokenList* ToList){
    Token* CurToken;
    TrieNode* ChildNode;
    TrieNode* CurNode = (TrieNode*)malloc(sizeof(TrieNode));
    InitTrieNode(CurNode, "Assignment");

    CurToken = GetCurToken(ToList);
    if(CurToken==NULL || CurToken->type!=_Key) {
        SetErrorToken(SynError, "expected express before '=' token", CurToken);
        return NULL;
    }
    AddTerminal(CurNode, CurToken->string, CurToken);
    MoveToken(ToList, 1);

    CurToken = GetCurToken(ToList);
    if(CurToken==NULL || strcmp(CurToken->string, ":=")) {
        SetErrorToken(SynError, "expected ';' before expression", CurToken);
        return NULL;
    }
    AddTerminal(CurNode, CurToken->string, CurToken);
    MoveToken(ToList, 1);

    CurToken = GetCurToken(ToList);
    if(CurToken==NULL) {
        SetErrorToken(SynError, "expected expression after ':='", CurToken);
        return NULL;
    } else if(CurToken->type==_String) {
        AddTerminal(CurNode, CurToken->string, CurToken);
        MoveToken(ToList, 1);
    } else if(!strcmp(CurToken->string, "(") || CurToken->type==_Number || \
        CurToken->type==_Key || CurToken->type==_Char){
        ChildNode = Expression(ToList);
        if(ChildNode==NULL) return NULL;
        AddChild(CurNode, ChildNode);
    }

    CurToken = GetCurToken(ToList);
    if(CurToken==NULL || strcmp(CurToken->string, ";")) {
        SetErrorToken(SynError, "need a ';' after statement", CurToken);
        return NULL;
    }
    AddTerminal(CurNode, CurToken->string, CurToken);
    MoveToken(ToList, 1);

    return CurNode;
}

TrieNode* IfStmt(TokenList* ToList){
    Token* CurToken;
    TrieNode* ChildNode;
    TrieNode* CurNode = (TrieNode*)malloc(sizeof(TrieNode));
    InitTrieNode(CurNode, "IfStmt");

    CurToken = GetCurToken(ToList);
    if(CurToken==NULL || strcmp(CurToken->string, "if")) return NULL;
    AddTerminal(CurNode, CurToken->string, CurToken);
    MoveToken(ToList, 1);

    ChildNode = Expression(ToList);
    if(ChildNode==NULL) return NULL;
    else AddChild(CurNode, ChildNode);

    CurToken = GetCurToken(ToList);
    if(CurToken==NULL || strcmp(CurToken->string, "then")) {
        SetErrorToken(SynError, "expected a 'then' after 'if'", CurToken);
        return NULL;
    }
    AddTerminal(CurNode, CurToken->string, CurToken);
    MoveToken(ToList, 1);

    ChildNode = Block(ToList);
    if(ChildNode==NULL) return NULL;
    else AddChild(CurNode, ChildNode);

    CurToken = GetCurToken(ToList);
    if(CurToken==NULL) return CurNode;
    if(!strcmp(CurToken->string, "else")) {
        AddTerminal(CurNode, CurToken->string, CurToken);
        MoveToken(ToList, 1);

        ChildNode = Block(ToList);
        if(ChildNode==NULL) {
            SetErrorToken(SynError, "find nothing after 'else'", CurToken);
            return NULL;
        } else AddChild(CurNode, ChildNode);
    }

    CurToken = GetCurToken(ToList);
    if(CurToken==NULL || strcmp(CurToken->string, "endif")) {
        SetErrorToken(SynError, "expected 'endif' after 'if'", CurToken);
        return NULL;
    }
    MoveToken(ToList, 1);

    return CurNode;
}

TrieNode* ReturnStmt(TokenList* ToList){
    Token* CurToken;
    TrieNode* ChildNode;
    TrieNode* CurNode = (TrieNode*)malloc(sizeof(TrieNode));
    InitTrieNode(CurNode, "ReturnStmt");

    CurToken = GetCurToken(ToList);
    if(CurToken==NULL || strcmp(CurToken->string, "return")) return NULL;
    AddTerminal(CurNode, CurToken->string, CurToken);
    MoveToken(ToList, 1);

    ChildNode = Expression(ToList);
    if(ChildNode==NULL) return NULL;
    else AddChild(CurNode, ChildNode);

    CurToken = GetCurToken(ToList);
    if(CurToken==NULL || strcmp(CurToken->string, ";")) {
        SetErrorToken(SynError, "need a ';' after statement", CurToken);
        return NULL;
    }
    AddTerminal(CurNode, CurToken->string, CurToken);
    MoveToken(ToList, 1);

    return CurNode;
}

TrieNode* RepeatStmt(TokenList* ToList){
    Token* CurToken;
    TrieNode* ChildNode;
    TrieNode* CurNode = (TrieNode*)malloc(sizeof(TrieNode));
    InitTrieNode(CurNode, "RepeatStmt");

    CurToken = GetCurToken(ToList);
    if(CurToken==NULL || strcmp(CurToken->string, "repeat")) return NULL;
    AddTerminal(CurNode, CurToken->string, CurToken);
    MoveToken(ToList, 1);

    ChildNode = Block(ToList);
    if(ChildNode==NULL) return NULL;
    AddChild(CurNode, ChildNode);

    CurToken = GetCurToken(ToList);
    if(CurToken==NULL || strcmp(CurToken->string, "until")) {
        SetErrorToken(SynError, "expected a 'until' after 'repeat'", CurToken);
        return NULL;
    }
    AddTerminal(CurNode, CurToken->string, CurToken);
    MoveToken(ToList, 1);

    ChildNode = Expression(ToList);
    if(ChildNode==NULL) return NULL;
    AddChild(CurNode, ChildNode);

    return CurNode;
}

TrieNode* ReadStmt(TokenList* ToList){
    Token* CurToken;
    TrieNode* ChildNode;
    TrieNode* CurNode = (TrieNode*)malloc(sizeof(TrieNode));
    InitTrieNode(CurNode, "ReadStmt");

    CurToken = GetCurToken(ToList);
    if(CurToken==NULL || strcmp(CurToken->string, "read")) return NULL;
    AddTerminal(CurNode, CurToken->string, CurToken);
    MoveToken(ToList, 1);

    CurToken = GetCurToken(ToList);
    if(CurToken==NULL || CurToken->type!=_Key) {
        SetErrorToken(SynError, "need a key after 'read'", CurToken);
        return NULL;
    }
    AddTerminal(CurNode, CurToken->string, CurToken);
    MoveToken(ToList, 1);

    CurToken = GetCurToken(ToList);
    if(CurToken==NULL || strcmp(CurToken->string, ";")) {
        SetErrorToken(SynError, "need a ';' after statement", CurToken);
        return NULL;
    }
    AddTerminal(CurNode, CurToken->string, CurToken);
    MoveToken(ToList, 1);

    return CurNode;
}

TrieNode* WriteStmt(TokenList* ToList){
    Token* CurToken;
    TrieNode* ChildNode;
    TrieNode* CurNode = (TrieNode*)malloc(sizeof(TrieNode));
    InitTrieNode(CurNode, "WriteStmt");

    CurToken = GetCurToken(ToList);
    if(CurToken==NULL || strcmp(CurToken->string, "write")) return NULL;
    AddTerminal(CurNode, CurToken->string, CurToken);
    MoveToken(ToList, 1);

    CurToken = GetCurToken(ToList);
    if(CurToken==NULL || CurToken->type!=_Key) return NULL;
    AddTerminal(CurNode, CurToken->string, CurToken);
    MoveToken(ToList, 1);

    CurToken = GetCurToken(ToList);
    if(CurToken==NULL || strcmp(CurToken->string, ";")) {
        SetErrorToken(SynError, "need a ';' after statement", CurToken);
        return NULL;
    }
    AddTerminal(CurNode, CurToken->string, CurToken);
    MoveToken(ToList, 1);

    return CurNode;
}

TrieNode* Expression(TokenList* ToList){
    Token* CurToken;
    TrieNode* ChildNode;
    TrieNode* CurNode = (TrieNode*)malloc(sizeof(TrieNode));
    InitTrieNode(CurNode, "Expression");

    ChildNode = Exp0(ToList);
    if(ChildNode==NULL) return NULL;
    AddChild(CurNode, ChildNode);

    CurToken = GetCurToken(ToList);
    if(CurToken==NULL) return CurNode;
    char* CurStr = CurToken->string;
    if(!strcmp(CurStr, "==") || !strcmp(CurStr, "!=") || \
       !strcmp(CurStr, ">=") || !strcmp(CurStr, "<=") || \
       !strcmp(CurStr, ">")  || !strcmp(CurStr, "<") ) {
        AddTerminal(CurNode, CurToken->string, CurToken);
        MoveToken(ToList, 1);
        ChildNode = Exp0(ToList);
        if(ChildNode==NULL) {
            SetErrorToken(ExpError, "invalid expression", CurToken);
            return NULL;
        }
        AddChild(CurNode, ChildNode);
    }
    else return CurNode;

    return CurNode;
}

TrieNode* Exp0(TokenList* ToList){
    Token* CurToken;
    TrieNode* ChildNode;
    TrieNode* CurNode = (TrieNode*)malloc(sizeof(TrieNode));
    InitTrieNode(CurNode, "Exp0");

    ChildNode = Exp1(ToList);
    if(ChildNode!=NULL)
        AddChild(CurNode, ChildNode);

    while(1) {
        CurToken = GetCurToken(ToList);
        if(CurToken==NULL) break;
        char* CurStr = CurToken->string;
        if(!strcmp(CurStr, "+") || !strcmp(CurStr, "-")){
            AddTerminal(CurNode, CurToken->string, CurToken);
            MoveToken(ToList, 1);
            ChildNode = Exp1(ToList);
            if(ChildNode==NULL) {
                SetErrorToken(ExpError, "invalid expression", CurToken);
                return NULL;
            }
            AddChild(CurNode, ChildNode);
        } else break;
    }

    return CurNode;
}

TrieNode* Exp1(TokenList* ToList){
    Token* CurToken;
    TrieNode* ChildNode;
    TrieNode* CurNode = (TrieNode*)malloc(sizeof(TrieNode));
    InitTrieNode(CurNode, "Exp1");

    ChildNode = Exp2(ToList);
    if(ChildNode==NULL) return NULL;
    AddChild(CurNode, ChildNode);

    while(1) {
        CurToken = GetCurToken(ToList);
        if(CurToken==NULL) break;
        char* CurStr = CurToken->string;
        if(!strcmp(CurStr, "*") || !strcmp(CurStr, "/")){
            AddTerminal(CurNode, CurToken->string, CurToken);
            MoveToken(ToList, 1);
            ChildNode = Exp2(ToList);
            if(ChildNode==NULL) {
                SetErrorToken(ExpError, "invalid expression", CurToken);
                return NULL;
            }
            AddChild(CurNode, ChildNode);
        } else break;
    }

    return CurNode;
}

TrieNode* Exp2(TokenList* ToList){
    Token* CurToken;
    TrieNode* ChildNode;
    TrieNode* CurNode = (TrieNode*)malloc(sizeof(TrieNode));
    InitTrieNode(CurNode, "Exp2");

    CurToken = GetCurToken(ToList);
    if(CurToken==NULL) return NULL;

    if(CurToken->type==_Number || \
       CurToken->type==_Key || CurToken->type==_Char) {
        AddTerminal(CurNode, CurToken->string, CurToken);
        MoveToken(ToList, 1);
    } else if (!strcmp(CurToken->string, "(")) {
        AddTerminal(CurNode, CurToken->string, CurToken);
        MoveToken(ToList, 1);

        ChildNode = Expression(ToList);
        if(ChildNode==NULL) return NULL;
        AddChild(CurNode, ChildNode);

        CurToken = GetCurToken(ToList);
        if(strcmp(CurToken->string, ")")) return NULL;
        AddTerminal(CurNode, CurToken->string, CurToken);
        MoveToken(ToList, 1);
    } else return NULL;

    return CurNode;
}

#endif