#ifndef SYMTAB_H
#define SYMTAB_H

#include "trie.h"
#include "error.h"
#include <string.h>
#define SYMSIZE 1000

typedef enum SymType{
    _int, _char, _string, _float, _bool, _Unknown
}SymType;

typedef struct Symbol
{
    SymType _type;
    char* _name;
    int _loc;
}Symbol;

typedef struct Symtab
{
    Symbol* sym[SYMSIZE]; 
    int TabSize;
    int CurLoc;
}Symtab;

void SetErrorSym(ErrType _type, char* _buf)
{
    Error.ErrTp = _type;
    Error.ErrInfo = _buf;
    Error.Posx = 0;
    Error.Posy = 0;
}

SymType GenType(char* _type)
{
    if(!strcmp(_type, "int")) return _int;
    if(!strcmp(_type, "char")) return _char;
    if(!strcmp(_type, "string")) return _string;
    if(!strcmp(_type, "float")) return _float;
    if(!strcmp(_type, "bool")) return _bool;
}

char* StrType(SymType _type)
{
    if(_type==_int) return "int";
    if(_type==_char) return "char";
    if(_type==_string) return "string";
    if(_type==_float) return "float";
    if(_type==_bool) return "bool";
}

int GetMemSize(SymType _type, char* _name)
{
    if(_type==_int) return sizeof(int);
    if(_type==_char) return sizeof(char);
    if(_type==_string) return sizeof(char) * strlen(_name);
    if(_type==_float) return sizeof(float);
    if(_type==_bool) return sizeof(bool);
}

SymType GetNumberType(char* str)
{
    int len = strlen(str);
    for(int i=0;i<len;i++) {
        if(str[i]=='.') return _float;
        if(str[i]=='e') return _float;
        if(str[i]=='E') return _float;
    }
    return _int;
}

SymType InitTable(Symtab *Table)
{
    Table->TabSize = 0;
    Table->CurLoc = 0;
}

Symbol* SymbolExist(Symtab *Table, char* _name)
{
    for(int i=0;i<Table->TabSize;i++)
        if(!strcmp(Table->sym[i]->_name, _name))
            return Table->sym[i];
    return NULL;
}
// return 1:exist, 0:non-

bool InsertNewSymbol(Symtab *Table, SymType _type, char* _name)
{
    Symbol* NewSym = (Symbol*)malloc(sizeof(Symbol));
    NewSym->_name = _name;
    NewSym->_type = _type;
    NewSym->_loc = Table->CurLoc;
    Table->CurLoc += GetMemSize(_type, _name);
    Table->sym[Table->TabSize++] = NewSym;
    return true;
}

void OutputSymtab(Symtab *Table)
{
    printf("Symbol table:\n");
    printf("  Name  |  Type  |  Location\n");
    printf("========|========|==========\n");
    for(int i=0;i<Table->TabSize;i++) {
        printf("%8s|%8s|%8d\n", Table->sym[i]->_name, \
                             StrType(Table->sym[i]->_type), \
                             Table->sym[i]->_loc );
    }
        
}

int CntPlace = 0;
char* NewPlace()
{
    char* _place = (char*)malloc(5);
    sprintf(_place, "t%d", CntPlace++);
    return _place;
}
// Place

#endif