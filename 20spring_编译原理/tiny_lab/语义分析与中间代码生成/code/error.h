#ifndef ERROR_H
#define ERROR_H  

#include<stdio.h>

typedef enum ErrType{
    Success, CodeEnd, KeyError, AnnError, SynError, \
    UknError, MemError, ExpError, NameError, TypeError
}ErrType;

typedef struct ErrCode
{
    char* ErrInfo;
    ErrType ErrTp;
    int Posx, Posy;
}ErrCode;

ErrCode Error;

void OutputError(char* FileName){
    printf("%s:%d:%d ", FileName, Error.Posx+1, Error.Posy);
    if(Error.ErrTp == KeyError) printf("Key Error: ");
    if(Error.ErrTp == AnnError) printf("Anntation Error: ");
    if(Error.ErrTp == SynError) printf("Syntex Error: ");
    if(Error.ErrTp == UknError) printf("Unknown Error: ");
    if(Error.ErrTp == MemError) printf("Memory Error: ");
    if(Error.ErrTp == ExpError) printf("Expression Error: ");
    if(Error.ErrTp == NameError) printf("Name Error: ");
    if(Error.ErrTp == TypeError) printf("Type Error: ");
    printf("%s\n", Error.ErrInfo);
}

#endif