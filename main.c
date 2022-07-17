#include "map.h"
#include <stdio.h>

int main(int argc, char** argv){

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

    return 0;

}