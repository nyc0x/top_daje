#include "parser.h"
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
/*
int isValidHeader(char* token ){
    for(int i = 0; i < KEY_WORD_LEN; i++)
        if (!strcmp(token,KEY_WORDS[i]))
            if(i)
                return 1; 
            else
                return 0; 
    return -1; 
}
*/
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
/*
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

    
    //* Clean-up
    //* [NOTE]: free(token) NOT NEEDED since token is updated with NULL when there is no more token available!
    
    free(line);
    return sys_stats_values;
}
*/

/*
    descr: 
    args:   
    retval: 
    author: [MZ] 
*/
/*
int isInValidOffset(int idx){
    for(int i = 0; i < NUM_PROC_STATS ;i++)
        if(idx == PROC_STAT_OFFSET[i])
            return 0;
    return -1;
}
*/


/*
    descr: 
    args:   
    retval: 
    author: [MZ] 
*/
/*
char** getProcessStat(char* pid ,ProcUsage** rankProcUsage, int index){
    
    int lenghtPid = (int) strlen(pid);
    char formattedPath[lenghtPid+12];

    strcpy(formattedPath, "/proc/");
    strcat(formattedPath, pid);
    strcat(formattedPath, "/stat");
    
    FILE* fp = fopen(formattedPath, "r");
   
    if(fp == NULL){
        printf("Error while opening /proc/stat file: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    char** proc_stats_values = (char**)malloc(NUM_SYS_STATS*sizeof(char*));

    char* line = NULL;
    ssize_t val = 1;
    ssize_t n = getline(&line, &val , fp);
    char* token;
    
    if(n != -1){  //from kernel we know it's just a one line file.  
        
        token = strtok(line, " ");
        
        int i = 0,proc_idx = 0;
        while(token != NULL){
            if(!isInValidOffset(i)){
                int length = (int) strlen(token);
                
                proc_stats_values[proc_idx] = (char*)malloc(sizeof(char)*(length+1));
                strcpy(proc_stats_values[proc_idx], token);
              
                //printf("token %s : proc_stats : %s \n", token, proc_stats_values[proc_idx]);
                proc_idx++;
            }
            
            i++;
            token = strtok(0, " ");
            
        }
    }
    //TODO: crea init() for ProcUsage and insert functions.
    ProcUsage* item = (ProcUsage*) malloc(sizeof(ProcUsage));
    item->pid = atoi(pid);
    item->cpu_usage = (float) atoi(proc_stats_values[3]); //TODO: change this value to the correct ones. Leave it as it is just for testing.
    //Append to global rankProtUsage array.
    rankProcUsage[index] = item;
    
    fclose(fp);
    return proc_stats_values;
}
*/



/*
    descr: 
    args:   
    retval: 
    author: [NDP] 
*/
/*
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

*/



/*
    descr: 
    args:   
    retval: 
    author: [MZ] 
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
                
                int lenghtPid = (int) strlen(dir_data->d_name);
                char formattedPath[lenghtPid+12];

                strcpy(formattedPath, "/proc/");
                strcat(formattedPath, dir_data->d_name);
                strcat(formattedPath, "/stat");

                FILE* fp = fopen(formattedPath, "r");

                if(!fp){ printf("Error while opening /proc/stat file: %s\n", strerror(errno)); exit(EXIT_FAILURE); }
                /*
                //See credits and constant.h
                pid_t*               pid         =   (pid_t*) malloc(sizeof(pid_t));                                //1
                char*                comm        =   (char*) malloc(sizeof(char));                                  //2
                char*                state       =   (char*) malloc(sizeof(char));                                  //3
                long unsigned*       utime       =   (long unsigned *) malloc(sizeof(long unsigned));
                long unsigned*       stime       =   (long unsigned *) malloc(sizeof(long unsigned));
                long unsigned*       vsize       =   (long unsigned *) malloc(sizeof(long unsigned));               //14 , 15 , 23
                long*                num_threads =   (long *) malloc(sizeof(long));                                 //20
                long long unsigned*  starttime   =   (long long unsigned *) malloc(sizeof(long long unsigned));     //22
                */
                pid_t pid;         
                char* comm;
                char state;
                long unsigned utime;
                long unsigned stime;
                long unsigned vsize;              
                long num_threads;
                long long unsigned starttime;
                

                //                    1    2   3  4   5   6   7   8   9   10   11   12   13   14  15  16    17  18   19   20  21  22   23 
                //int ret = fscanf(fp, "%d (%s) %c %*d %*d %*d %*d %*d %*u %*lu %*lu %*lu %*lu %lu %lu %*ld %*ld %*ld %*ld %ld %*ld %llu %*lu ",(int*) pid,comm,state,utime,stime,num_threads,starttime,vsize);
                int ret = fscanf(fp, "%d (%s) %c %*d %*d %*d %*d %*d %*u %*lu %*lu %*lu %*lu %lu %lu %*ld %*ld %*ld %*ld %ld %*ld %llu %*lu ",(int*) &pid,comm,&state,&utime,&stime,&num_threads,&starttime,&vsize);
                
                

                //printf("%d (%s) %C %lu %lu %ld %llu \n",pid,comm,state,utime,stime,num_threads,starttime,vsize);
                if (ret == EOF) { printf("Error while reading from file %s: Error: %s\n", formattedPath, strerror(errno)); exit(EXIT_FAILURE);}
                item->data = (ProcData*) calloc(1,sizeof(ProcData));

                /*
                item->data->pid =  *pid;
                item->data->comm = comm;
                item->data->state = *state;
                item->data->utime = *utime;
                item->data->stime =  *stime;
                item->data->vsize =  *vsize;
                item->data->num_threads = *num_threads;
                item->data->starttime =  *starttime;
                */
                item->data->pid =  pid;
                item->data->comm = comm;
                item->data->state = state;
                item->data->utime = utime;
                item->data->stime = stime;
                item->data->vsize = vsize;
                item->data->num_threads = num_threads;
                item->data->starttime =  starttime;


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
    printf("pid: %d\n, name: %s\n, state: %c\n, ", data->pid, data->comm, data->state);
    printf("user_time: %lu\n, sys_time: %lu\n, n_threads: %ld\n, ", data->utime, data->stime, data->num_threads);
    printf("start_time: %llu\n, virtual_mem_size: %lu\n", data->starttime, data->vsize);
}

void procListItem_print(ListHead* head){
  ListItem* aux=head->first;
  printf("[");
  while(aux){
    ProcListItem* element = (ProcListItem*) aux;
    procData_print(element->data);
    aux=aux->next;
  }
  printf("]\n");
}


//TODO: delete main after implementing get pid stats
int main(int argc, char const *argv[]){
    ListHead head;
    
    List_init(&head);
    getAllProcData(&head);
    procListItem_print(&head);

    return 0;
}



