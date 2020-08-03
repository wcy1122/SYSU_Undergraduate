#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<io.h>

#include "scan.h"
#include "tokens.h"
#include "trie.h"
#include "parser.h"
#include "error.h"

size_t GetSize(char* path)
{
    FILE* fp = fopen(path, "r");
    fseek(fp, 0L, SEEK_END);
    size_t length = ftell(fp);
    fclose(fp);
    return length;
}

char* LoadCode(char* path)
{
    size_t FileSize = GetSize(path);
    FILE* fp = fopen(path, "r");
    char *buf = malloc(FileSize+5);
    fread(buf, 1, FileSize, fp);
    buf[FileSize]='\0';
    fclose(fp);
    return buf;
}

int main(int argc, char* argv[])
{
    if(argc!=2)
    {
        printf("Invalid Input!\n");
        printf("test --FileName\n");
        return 0;
    }
    
    char* code = LoadCode(argv[1]);

    Scanner Scan;
    TokenList ToList;
    ErrType ret;
    InitScanner(&Scan, code);
    InitTokenList(&ToList);

    while(1)
    {
        ret = NextToken(&Scan, &ToList);
        if(ret!=Success) break;
    }
    if(ret!=CodeEnd){
        printf("Lexical Analysis Fail.\n");
        OutputError(argv[1]);
        return 0;
    }
    printf("Lexical Analysis Success.\n");
    OutputTokens(&ToList);
    // Lexical Analysis

    TrieNode* root = Parsing(&ToList);
    if(root==NULL){
        printf("Syntax Analysis Fail.\n");
        OutputError(argv[1]);
        return 0;
    }
    printf("Syntax Analysis Success.\n");
    OutputTrie(root, 0);
    // Syntax Analysis

    return 0;
}