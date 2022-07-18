#include <stdlib.h>
#include "map.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define SYS_STATS_PATH "/proc/stat" //[TODO]: add to .h file
#define PROC_PATH "/proc/" //[TODO]: add to .h file
#define NUM_SYS_STATS 8

static const char* KEY_WORDS[] = {
    "cpu",
    "ctxt",
    "btime",
    "processes",
    "procs_running",
    "procs_blocked"
};

#define KEY_WORD_LEN sizeof(KEY_WORDS)/sizeof(char*)

static const char* SYS_STATS_HEADERS[] = {
    "user",
    "system",
    "idle",
    "ctxt",
    "btime",
    "processes",
    "procs_running",
    "procs_blocked"
};


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


/*
    descr:
    args:
    retval: 
    author:
*/
int isValidHeader(char* token ){
    for(int i = 0; i < KEY_WORD_LEN; i++)
        if (!strcmp(token,KEY_WORDS[i]))
            if(i)
                return 1; //-> It's not cpu header
            else
                return 0; //-> It's cpu header.
    return -1; //error
}


int main(int argc, char const *argv[]){

    FILE* fp = fopen(SYS_STATS_PATH, "r");

    if(!fp){
        printf("Error while opening /proc/stat file: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    long length = getFileLength(fp);
    char* buf = (char*) malloc(sizeof(char)*length);
    char* token;

    long* sys_stats_values = (long*)calloc(NUM_SYS_STATS,sizeof(long));
    
    int bytes_read,n,sys_idx = 0;
    char* line;
    while((bytes_read = getline(&line, &n , fp)) != -1){
        //append line to buf
        //Tokenize
        token = strtok(line, " ");
        int type = isValidHeader(token);

        if( type < 0){
            ; //No need to tokenize
        }

        int pos = 0;
        while(token){
            token = strtok(buf, " ");
            if(type){ //IF it's not cpu header
                sys_stats_values[sys_idx] = atoi(token); //TODO: add error handling atoi.
                sys_idx++;
            }else{ //IF it's cpu header.
                if(pos == 1 || pos == 3 || pos == 4 ){
                    sys_stats_values[sys_idx] = (long) atoi(token);
                    sys_idx++;
                }
            }
            pos++;
        }

        
    }

    //TESTING
    for(int i =0 ; i <  NUM_SYS_STATS; i++)
        printf("%s : %d \n",SYS_STATS_HEADERS[i],(int) sys_stats_values[i]);
    


    //CLEAN-UP
    fclose(fp);

    return 0;
}

