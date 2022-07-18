#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "../constant.h"
#include "../data_structures/linked_list.h"
#include "../util.h"

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


long* getSystemStat(FILE* fp);

void getAllProcData(ListHead* head);

void procListItem_print(ListHead* head);