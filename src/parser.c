#include <stdlib.h>
#include "map.h"
#include <stdio.h>
#include <string.h>

#define SYS_STATS_PATH "/proc/stat" //[TODO]: add to .h file
#define PROC_PATH "/proc/"; //[TODO]: add to .h file


/*
    descr:
    args:
    retval: 
    author:
*/
long getFileLength(FILE* fp ){
    long ret = -1;
    if(fp){
        if(fseek(fp,0,SEEK_END) < 0 ) exit(EXIT_FAILURE);
        ret = ftell(fp);
        if(fseek(fp, 0,SEEK_SET) < 0 ) exit(EXIT_FAILURE);
    }
    return ret;
}

int main(int argc, char const *argv[]){

    FILE* fp = fopen(SYS_STATS_PATH, "r");

    if(!fp){
        exit(EXIT_FAILURE);
    }
    long length = getFileLength(fp);
    char* buf = (char*) malloc(sizeof(char)*length);
    
    char ch;
    while((ch = getc(fp)) != EOF){
        
    }
    



    return 0;
}

