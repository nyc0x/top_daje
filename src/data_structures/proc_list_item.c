#include "proc_list_item.h"

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