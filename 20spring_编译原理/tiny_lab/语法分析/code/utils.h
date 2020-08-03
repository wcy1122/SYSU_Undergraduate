#ifndef UTILS_H
#define UTILS_H  

#include<stdbool.h>
#include<string.h>

bool isletter(char ch){
    if('a'<=ch && ch<='z') return true;
    if('A'<=ch && ch<='Z') return true;
    if(ch=='_') return true;
    return false;
}

bool isToken(char ch){
    if(ch=='+' || ch=='-' || ch=='*' || ch=='/')return true;
    if(ch=='&' || ch=='|' || ch=='%') return true;
    if(ch=='<' || ch=='>' || ch=='=') return true;
    if(ch==':' || ch==';' || ch==',') return true;
    if(ch=='"' || ch=='\'' || ch==')') return true;
    return false;
}

bool isEqu(char ch){
    if(ch=='<' || ch=='>' ||  ch=='!' ||  ch==':' || ch=='=') return true;
    return false;
}

bool StrCmp(char* base, char* ref){
    int len = strlen(ref);
    for(int i=0;i<len;i++)
        if(base[i]!=ref[i])return false;
    return true;
}

#endif