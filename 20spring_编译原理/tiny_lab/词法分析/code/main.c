#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<io.h>

#include "scan.h"
#include "tokens.h"

size_t GetSize(char* path)
{
    FILE* fp = fopen(path, "r");
    fseek(fp, 0L, SEEK_END);
    size_t FileSize = ftell(fp);
    fclose(fp);
    return FileSize;
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
    }
    
    char* code = LoadCode(argv[1]);

    Scanner Scan;
    TokenList ToList;
    int ret;
    Init(&Scan, code);

    while(1)
    {
        ret = NextToken(&Scan, &ToList);
        if(ret) break;
    }

    if(ret==1){
        printf("Compile Success\n");
        OutputTokens(&ToList);
    }
    else OutputError(Scan.buf, Scan.pos, ret);

    free(code);
    return 0;
}