#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "symtab.h"
#include "tokens.h"

bool _GenSymTab(TrieNode *CurNode, Symtab *Table, SymType _type)
{
    char* NodeName = CurNode->NodeName;
    int _start = 0;
    Symbol* ret;
    if(!strcmp(NodeName, "Vars") || !strcmp(NodeName, "ForMalParams")){
        _type = GenType(CurNode->Child[0]->NodeName);
    }
    if(!strcmp(NodeName, "Func")) _start = 2;
    
    if(CurNode->NodeToken!=NULL){
        if(CurNode->NodeToken->type==_Key){
            if(_type==_Unknown){
                ret = SymbolExist(Table, CurNode->NodeName);
                if(ret==NULL){
                    char str[100];
                    sprintf(str, "name '%s' is not defined", CurNode->NodeName);
                    SetErrorToken(NameError, str, CurNode->NodeToken);
                    return false;
                }
                else _type = ret->_type;
            }
            else{
                InsertNewSymbol(Table, _type, CurNode->NodeName);
            }
        } else if(CurNode->NodeToken->type==_Number){
            _type = GetNumberType(CurNode->NodeName);
        } else if(CurNode->NodeToken->type==_Char){
            _type = _char;
        } else if(CurNode->NodeToken->type==_String){
            _type = _string;
        }
        SetNodeType(CurNode, _type);
    }

    for(int i=_start;i<CurNode->ChildSize;i++)
        if(!_GenSymTab(CurNode->Child[i], Table, _type)) return false;
    
    if(CurNode->NodeToken==NULL)
        CurNode->NodeToken = CurNode->Child[0]->NodeToken;

    return true;
}

bool GenSymTab(TrieNode *CurNode, Symtab *Table)
{
    return _GenSymTab(CurNode, Table, _Unknown);
}

bool CheckType(TrieNode *CurNode, Symtab *Table)
{
    for(int i=0;i<CurNode->ChildSize;i++)
        if(!CheckType(CurNode->Child[i], Table)) return false;
    // dfs
    if(!strcmp(CurNode->NodeName,"Exp0") || \
       !strcmp(CurNode->NodeName,"Exp1") || \
       !strcmp(CurNode->NodeName,"Exp2") || \
       !strcmp(CurNode->NodeName,"Expression") || \
       !strcmp(CurNode->NodeName,"Assignment") ) {
        if(CurNode->ChildSize>=3){
            if(!strcmp(CurNode->NodeName,"Exp2")) {
                CurNode->NodeType = CurNode->Child[1]->NodeType;
                CurNode->Place = CurNode->Child[1]->Place;
            }
            else if(!strcmp(CurNode->NodeName,"Expression")) {
                CurNode->NodeType = _bool;
            }
            else {
                if(CurNode->Child[0]->NodeType != CurNode->Child[2]->NodeType) {
                    SetErrorToken(TypeError, "type not equal", CurNode->NodeToken);
                    return false;
                }
                CurNode->NodeType = CurNode->Child[0]->NodeType;
                if(!strcmp(CurNode->NodeName,"Exp0") || \
                   !strcmp(CurNode->NodeName,"Exp1") ) {
                    CurNode->Place = NewPlace();
                    InsertNewSymbol(Table, CurNode->NodeType, CurNode->Place);
                }
            }
        }
        else {
            CurNode->NodeType = CurNode->Child[0]->NodeType;
            CurNode->Place = CurNode->Child[0]->Place;
        }
    }
    // exp0 exp1 exp2 expression
    if(CurNode->ChildSize==0){
        CurNode->Place = CurNode->NodeName;
    }
    // end ch
    return true;
}

#endif