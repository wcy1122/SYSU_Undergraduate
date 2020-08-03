#ifndef ERROR_H
#define ERROR_H  

#include<stdio.h>
#define KeyError -1
#define AnnError -2
#define SynError -3
#define UknError -4
#define MemError -5

void OutputError(char* buf, int pos, int type){
    int Posx = 1;
    int Posy = 0;
    for(int i=0;i<pos;i++){
        if(buf[i]=='\n') Posx++, Posy=0;
        else Posy++;
    }
    if(type == KeyError) printf("Key Error at %d:%d\n", Posx, Posy);
    if(type == AnnError) printf("Annotation Error at %d:%d\n", Posx, Posy);
    if(type == SynError) printf("Syntex Error at %d:%d\n", Posx, Posy);
    if(type == UknError) printf("Unknown Error at %d:%d\n", Posx, Posy);
    if(type == MemError) printf("Memory Error, code too long\n");
}

#endif