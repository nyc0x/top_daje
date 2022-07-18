#pragma once

#include "../data_structures/linked_list.h"
#include <sys/types.h>

/*type assigned from man 5 proc section /proc/[pid]/stat*/
typedef struct ProcData{
    pid_t                   pid;
    char*                   comm;
    char                    state;
    long unsigned           utime;
    long unsigned           stime;
    long                    num_threads;
    long long unsigned      starttime;
    long unsigned           vsize;
} ProcData;

typedef struct ProcListItem{
  ListItem list;
  ProcData *data;
} ProcListItem;

void fillData(ProcData*,
              pid_t, 
              char*, 
              char, 
              long unsigned, 
              long unsigned, 
              long unsigned, 
              long, 
              long long unsigned);

