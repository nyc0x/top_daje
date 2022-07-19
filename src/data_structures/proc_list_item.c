#include "proc_list_item.h"

/*
    descr:  This function simply fills all the fields of the ProcData* item given
    args:   [ProcData*]           {data}           the info buffer to be filled
            [pid_t]               {pid}            pid of the process to which the info refer to
            [char*]               {comm}           name of the process
            [char]                {state}          current state of the process
            [long unsigned]       {utime}          the time the process has spent in user time
            [long unsigned]       {stime}          the time the process has spent in system time
            [long unsigned]       {vsize}          virtual memory size of the process 
            [long]                {num_threads}    number of thread created by the process
            [long long unsigned]  {start_time}     the time the process started after system boot
    retval: None
    author: [NDP] 
*/
void fillData(ProcData* data,
              pid_t pid,
              char* comm,
              char state,
              long unsigned utime,
              long unsigned stime,
              long unsigned vsize,
              long num_threads,
              long long unsigned start_time) {
              
              data->pid = pid;
              data->comm = comm;
              data->state = state;
              data->utime = utime;
              data->stime = stime;
              data->vsize = vsize;
              data->num_threads = num_threads;
              data->starttime =  start_time;
}