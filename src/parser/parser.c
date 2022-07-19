#include "parser.h"
#include "../data_structures/proc_list_item.h"


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
        if (!strcmp(token,KEY_WORDS[i])) {
            if(i)
                return 1; 
            else
                return 0; 
        }
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
//TODO : improve current implementation performances
long* getSystemStat(FILE* fp){
    
    if(!fp) return NULL; //Never trust nobody ! 
    
    char* token;
    long* sys_stats_values = (long*)calloc(NUM_SYS_STATS,sizeof(long));
      
    size_t n;
    int bytes_read,sys_idx = 0;
    char* line = NULL;

    while((bytes_read = getline(&line, &n , fp)) != -1){
        token = strtok(line, " ");
        int type = isValidHeader(token);
        if (type >= 0){
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
    //* Clean-up
    //* [NOTE]: free(token) NOT NEEDED since token is updated with NULL when there is no more token available!
    free(line);
    
    return sys_stats_values;
}


/*
    descr: 
    args:   
    retval: 
    author: [MZ] [NDP]
*/
void getAllProcData(ListHead* head){
    if(!head) printf("Error list head vuota; %s", strerror(errno)); 
    
    DIR* dir = opendir(PROC_PATH);
    
    if (!dir) {
        printf("Error while opening %s directory: %s\n", PROC_PATH, strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    struct dirent* dir_data = readdir(dir); //Do not free this ptr (see man readdir)!
    int i=0;
    while (dir_data) {
        if (dir_data->d_type == DT_DIR && dir_data->d_name[0] != '.' && atoi(dir_data->d_name) != 0){ // IF IS A PID DIRECTORY
              
            //Creiamo list item con i campi di /proc/[pid]/stat

            ProcListItem* item = (ProcListItem*) malloc(sizeof(ProcListItem));
            item->list.prev = 0;
            item->list.next = 0;
            
            int length = (int)(strlen(dir_data->d_name)+strlen(PROC_PATH)+strlen(STAT_PATH));
            char formattedPath[length];

            strcpy(formattedPath, PROC_PATH);
            strcat(formattedPath, dir_data->d_name);
            strcat(formattedPath, STAT_PATH);

            FILE* fp = fopen(formattedPath, "r");

            if(!fp){ printf("Error while opening /proc/stat file: %s\n", strerror(errno)); exit(EXIT_FAILURE); }
          
            pid_t pid;         
            char* comm;
            char state;
            long unsigned utime, stime, vsize;            
            long num_threads;
            long long unsigned starttime;
             
            char* line = NULL;
            size_t n = 1;

            size_t bytes_read = getline(&line,&n,fp);
            if(bytes_read != -1){
                if(line){
                    int token_num = 1;
                    char* token = strtok(line, " ");
                    while (token){
                        switch (token_num){
                            case 1:
                                pid = atoi(token);
                                break;
                            case 2:
                                comm = token; //TODO: add function to strip the parenthesis.
                                break;
                            case 3:
                                state = token[0];
                                break;
                            case 14 :
                                utime = (long unsigned) atol(token);
                                break;
                            case 15:
                                stime = (long unsigned) atol(token);
                                break;
                            case 20:
                                vsize = (long unsigned) atol(token);
                                break;
                            case 22:
                                num_threads = atol(token);
                                break;
                            case 23:
                                starttime = (long long unsigned) atoll(token);
                                break;
                            default:
                                break;
                        }
                        token_num++;
                        token = strtok(0, " ");
                    }
                        
                    }
                }
                
                item->data = (ProcData*) malloc(sizeof(ProcData));
                fillData(item->data, pid, comm, state, utime, stime, vsize, num_threads, starttime);

                fclose(fp);
                
            List_insert(head, head->last, (ListItem*)item );
        }
        dir_data = readdir(dir);
    }


    if (closedir(dir) < 0)
        printf("Error while closing %s directory: %s\n", PROC_PATH, strerror(errno)); 
    return;
}

void procData_print(ProcData* data) {
    printf("\n[pid: %d]\n- name: %s\n- state: %c\n- ", data->pid, data->comm, data->state);
    printf("user_time: %lu\n- sys_time: %lu\n- n_threads: %ld\n- ", data->utime, data->stime, data->num_threads);
    printf("start_time: %llu\n- virtual_mem_size: %lu\n", data->starttime, data->vsize);
}

void procListItem_print(ListHead* head){
  ListItem* aux=head->first;
  while(aux){
    ProcListItem* element = (ProcListItem*) aux;
    procData_print(element->data);
    aux=aux->next;
  }
  printf("\n");
}


