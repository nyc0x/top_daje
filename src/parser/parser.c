#include "parser.h"


/*
    descr:  This function evaluates a string representing an header from /proc/stat file 
            and return a corresponding non negative integer (see constants.h)
    args:   [char* ] {token} that we want to validate.
    retval: 
            1  {token} is valid and it's equal to "cpu"
            0  {token} is valid and it's NOT equal to "cpu"
            -1 {token} is not valid.
    author: [MZ]
*/
int isValidHeader(char* token ){
    for(int i = 0; i < KEY_WORD_LEN; i++)
        if (!strcmp(token,KEY_WORDS[i]))
            if(i)
                return 1; 
            else
                return 0; 
    return -1; 
}

/*
    descr:  Given a file pointer to /proc/stat file, allocates and returns an array of long integers 
            where each indexed value matches "SYS_STATS_HEADERS" semantics (see "constant.h") 
    args:   [FILE *] {fp} file pointer to the /proc/stat file, it MUST have been already opened.
    retval: array of long integer with length "NUM_SYS_STATS" that has the following semantics:
            idx  <->  semantics
             0         Time spent in user mode
             1         Time spent in system mode.
             2         Time spent in the idle task.  This value should be USER_HZ times the second entry in the /proc/uptime pseudo-file.
             3         The number of context switches that the system underwent
             4         boot time, in seconds since the Epoch, 1970-01-01 00:00:00 +0000 (UTC).
             5         Number of forks since boot.
             6         Number of processes in runnable state.  
             7         Number of processes blocked waiting for I/O to complete.
    author: [MZ]
*/
long* getSystemStat(FILE* fp){
    
    if(!fp) return NULL; /* Never trust nobody ! */
    
    char* token;
    long* sys_stats_values = (long*)calloc(NUM_SYS_STATS,sizeof(long));
      
    size_t n;
    int bytes_read,sys_idx = 0;
    char* line = NULL;

    while((bytes_read = getline(&line, &n , fp)) != -1){
        token = strtok(line, " ");
        int type = isValidHeader(token);
        if( type >= 0){
            int pos = 1;
            token = strtok(0, " ");
            while(token){
                if(type){ //IF it's not "cpu" header
                    sys_stats_values[sys_idx] = atoi(token); //TODO: add error handling atoi.
                    sys_idx++;
                }else{ //IF it's cpu header.
                    if(pos == 1 || pos == 3 || pos == 4 ){
                        sys_stats_values[sys_idx] = (long) atoi(token);
                        sys_idx++;
                    }
                }
                pos++;
                token = strtok(0, " ");
            }
        }
    }

    /*
    * Clean-up
    * [NOTE]: free(token) NOT NEEDED since token is updated with NULL when there is no more token available!*/
    
    free(line);
    return sys_stats_values;
}


/*
    TODO: CONTINUE HERE.
    descr: 
    args:   
    retval: 
    author: [MZ] [NDP] 
*/
long* getProcessStat(const char* pid ){
    char* path = "/proc/";
    FILE* fp = fopen(strcat(path, strcat(pid,"/stat")), "r");

    if(!fp){
        printf("Error while opening /proc/stat file: %d\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    long* proc_stats_values = (long*)calloc(NUM_SYS_STATS,sizeof(long));

    ssize_t n;  
    char* line = getline(&line, &n , fp);
    char* token;

    if(line != -1){
        token = strtok(line, " ");
        while(token){
            //TODO: CONTINUE HERE.
            token = strtok(0, " ");
        }
    }
    
    fclose(fp);
    return proc_stats_values;
}


//TODO: delete main after implementing get pid stats
int main(int argc, char const *argv[])
{
    /* code */
    return 0;
}



