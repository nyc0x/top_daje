#include "map.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>

#define CHUNK 16

int getNumberProcs(int fd) {
    char* buf = (char*)malloc(sizeof(char));
    int res = read(fd, buf, 1);
    while (res != EOF) {
        
    }
}

int main(int argc, char** argv){

    int fd = open("/proc/stat", O_RDONLY);
    int n_proc = getNumberProcs(fd);
    //managing read op from /proc directory
    const char* s = "/proc/";
    DIR* dir = opendir(s);
    char** buf = (char**)malloc(sizeof(char*)*30000);
    for (int i = 0; i < 30000; i++) {
        buf[i] = (char*)malloc(sizeof(char));
        buf[i] = NULL;
    }
    if (!dir)
        printf("Error while opening %s directory: %s\n", s, strerror(errno));

    struct dirent* ptr;
    ptr = readdir(dir);
    int i = 0;

    while (ptr) {
        strcpy(buf[i], ptr->d_name);
        i++;
        ptr = readdir(dir); //updates the pointer to the next directory's entry
    }

    //for (i = 0; i < 30000; i++)
    //    printf("buf[%d]: %s\n", i, buf[i]);

    //everything's alright, we can proceed reading the stuff inside the directory


    /*
    //setting map size
    int size = 12;

    //initializing map structure
    Map* m = createMap(size);

    //adding the items to the map
    char* buf1[] = {"ciao", "bella", "foto"}; 
    int pid1 = 1;
    int len1 = sizeof(buf1)/sizeof(char*);
    
    char* buf2[] = {"bello", "brutto", "sano", "pazzo"}; 
    int pid2 = 3;
    int len2 = sizeof(buf2)/sizeof(char*);
    
    char* buf3[] = {"fratm", "sorm", "figlm", "patm", "ciaone"}; 
    int pid3 = 9;
    int len3 = sizeof(buf3)/sizeof(char*);

    char* buf4[] = {"prr", "strgr"}; 
    int pid4 = 6;
    int len4 = sizeof(buf4)/sizeof(char*);

    MapItem* item1 = createItem(m, pid1, buf1, len1);
    MapItem* item2 = createItem(m, pid2, buf2, len2);
    MapItem* item3 = createItem(m, pid3, buf3, len3);
    MapItem* item4 = createItem(m, pid4, buf4, len4);

    insertItem(m, item1);
    insertItem(m, item2);
    insertItem(m, item3);
    insertItem(m, item4);

    //debug print for map
    printf("------- START PRINTING THE STRUCTURE --------\n\n");
    mapPrint(m);
    printf("------- END PRINTING THE STRUCTURE --------\n");

    printf("Deallocating item1 ...\n");
    freeItem(m, item1);
    printf("Deallocation completed\n");

    printf("Updated map is:\n\n");
    mapPrint(m);

    printf("Deallocating item1 ...\n");
    freeItem(m, item4);
    printf("Deallocation completed\n");

    printf("Updated map is:\n\n");
    mapPrint(m);
    */

    if (closedir(dir) < 0)
        printf("Error while closing %s directory: %s\n", s, strerror(errno));   

    return 0;

}