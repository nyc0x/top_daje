
/* *[BEGIN] PARSER CONSTANTS */

    /* *[BEGIN] SYS STAT CONSTANTS */
    #define SYS_STATS_PATH "/proc/stat" 
    #define PROC_PATH "/proc/"

    static const char* KEY_WORDS[] = {
        "cpu",
        "ctxt",
        "btime",
        "processes",
        "procs_running",
        "procs_blocked"
    };

    #define KEY_WORD_LEN sizeof(KEY_WORDS)/sizeof(char*)


    #define NUM_SYS_STATS sizeof(SYS_STATS_HEADERS)/sizeof(char*)

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
    /* *[END] SYS STAT CONSTANTS */


    /* *[BEGIN] PROC STAT CONSTANTS */
    static const char* PROC_STATS_HEADERS[] = {
        "pid",
        "name",
        "state",
        "utime",
        "stime",
        "num_threads",
        "starttime",
        "vsize"
    };

    static const int PROC_STAT_OFFSET[] = {
        1,
        2,
        3,
        14,
        15,
        20,
        22,
        23
    }
    
    #define NUM_PROC_STATS sizeof(PROC_STATS_HEADERS)/sizeof(char*)

    /* *[END] PROC STAT CONSTANTS */

/*  *[END] PARSER CONSTANTS */



    /* [DOC] Template */
/*
    descr: 
    args:   
    retval: 
    author: [MZ] [NDP] 
*/