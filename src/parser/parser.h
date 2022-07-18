#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "../constant.h"

typedef struct ProcUsage{
    long int pid;
    float cpu_usage;
}ProcUsage;

typedef struct RankMapItem {
    int key;              //PID
    char** info;          //array string which contains process data
    int n_str;
    int r_idx;            //position in rank_idx global array
} RankMapItem;

typedef struct RankMap {
    RankMapItem** items;      //pointer to pointer to map items
    int size;             //map size
} RankMap;


long* getSystemStat(FILE* fp);