#include "parser.h"
//#include "map.h"
#include "../util.h"

/*
    descr:  This function evaluates a string representing an header from /proc/stat file 
            and return a corresponding non negative integer (see constants.h)
    args:   [char* ] {token} that we want to validate.
    retval: 
            1  {token} is valid and it's NOT equal to "cpu"
            0  {token} is valid and it's equal to "cpu"
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
                    if(pos == 1 || pos == 3 || pos == 4 ){ //TODO: add constants to fixed positions
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
    descr: 
    args:   
    retval: 
    author: [MZ] 
*/
int isInValidOffset(int idx){
    for(int i = 0; i < NUM_PROC_STATS ;i++)
        if(idx == PROC_STAT_OFFSET[i])
            return 0;
    return -1;
}

/*
    descr: 
    args:   
    retval: 
    author: [MZ] 
*/
char** getProcessStat(char* pid ){
    
    int lenghtPid = (int)strlen(pid);
    
    char formattedPath[lenghtPid+12];

    strcpy(formattedPath, "/proc/");
    strcat(formattedPath, pid);
    strcat(formattedPath, "/stat");

    FILE* fp = fopen(formattedPath, "r");
    
    if(!fp){
        printf("Error while opening /proc/stat file: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    char** proc_stats_values = (char**)malloc(NUM_SYS_STATS*sizeof(char*));

    char* line;
    ssize_t n = getline(&line, &n , fp);
    char* token;
    
    if(n != -1){  /* from kernel we know it's just a one line file.  */
        token = strtok(line, " ");
        int i = 0,proc_idx = 0;
        while(token){
            
            if(!isInValidOffset(i)){
                int length = (int) strlen(token);
                proc_stats_values[proc_idx] = (char*)malloc(sizeof(char)*(length+1));
                strcpy(proc_stats_values[proc_idx], token);
                proc_idx++;
            }
            i++;
            token = strtok(0, " ");
        }
    }
    
    fclose(fp);
    return proc_stats_values;
}


/*
    descr: 
    args:   
    retval: 
    author: [NDP] 
*/
void getAllPids(char** buf){
    DIR* dir = opendir(PROC_PATH);
    
    if (!dir) {
        printf("Error while opening %s directory: %s\n", PROC_PATH, strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    struct dirent* dir_data = readdir(dir); //Do not free this ptr (see man readdir)!
    int i=0;
    while (dir_data) {
        if (dir_data->d_type == DT_DIR && dir_data->d_name[0] != '.' && atoi(dir_data->d_name) != 0)
            buf[i++] = dir_data->d_name;
        dir_data = readdir(dir);
    }

    if (closedir(dir) < 0)
        printf("Error while closing %s directory: %s\n", PROC_PATH, strerror(errno)); 
    
    return;
}

/*
    descr: 
    args:   
    retval: 
    author: [NDP] 
*/
/*Map* populateMapProcesses(){
    //crei una mappa "map"
    
    //for pid in pids
        //values[i] = getProcessStat(pid);
        //"map".addItem(pid, values[i]).
    
    //return Map
    return NULL;
}*/


//TODO: delete main after implementing get pid stats
int main(int argc, char const *argv[]){
    
    //Testing
    char** a = getProcessStat("102");
    for(int i = 0 ; i < NUM_PROC_STATS; i++){
        printf("%s : %s \n", PROC_STATS_HEADERS[i], a[i]);
    }

    long int n_procs = countProcs(PROC_PATH);
    //Map* m = createMap(n_procs);

    char** pids = (char**)malloc(sizeof(char*)*n_procs);
    getAllPids(pids);

    for (int i=0; i<n_procs; i++)
        printf("pid[%s]\n", pids[i]);    
    
    return 0;
}



