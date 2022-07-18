#include "./parser/parser.h"
#include "util.h"

 
/*
    descr:  Call this function in order to test the system stat crawling functionality.  
    args:   None
    retval: None
    author: [MZ] 
*/

void testSysStat(){
    FILE* fp = fopen(SYS_STATS_PATH, "r");

    if(!fp){
        printf("Error while opening /proc/stat file: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    long* sys_stats_values = getSystemStat(fp);
    
    /*
        TODO: add more tests.
    */
    //TESTING
    for(int i = 0 ; i <  NUM_SYS_STATS; i++)
        printf("%s : %d \n",SYS_STATS_HEADERS[i],(int) sys_stats_values[i]);
    

    //CLEAN-UP
    fclose(fp);
    return;
}

/*
    descr:  Test function for "countDir(...)" giving current working directory as a path. 
    args:   None
    retval: None
    author: [MZ] 
*/
void testCountDir(){
    //TODO: add more tests.
    printf("[TEST]: I found %d directories in this path : %s \n",countDir("./"),"./" );
    return;
}

/*
    CMD: make test
*/
int main(int argc, char const *argv[]){
    
    printf("Start testing !\n");

    testSysStat();
    testCountDir();


    printf("End testing !\n");
    
    return 0;
}
