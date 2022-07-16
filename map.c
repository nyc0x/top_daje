#include "map.h"
#include <stdlib.h>
#include <string.h>

Map* createMap(int size) {
    Map* m = (Map*)malloc(sizeof(Map));
    m->ptr = (MapItem**)malloc(sizeof(MapItem*));
    m->size = size;
    m->used ^= m->used;
    for (int i = 0; i < m->size; i++)
        m->ptr[i] = NULL;
    return m;
}

MapItem* createItem(int key, char** info) {
    MapItem* item = (MapItem*)malloc(sizeof(MapItem));
    item->key = key;
    int len = sizeof(info)/sizeof(char*);
    item->info = (char**)malloc(sizeof(char*)*len);
    for (int i=0; i<len; i++) {
        item->info[i] = (char*)malloc(sizeof(char));
        strcpy(item->info[i], info[i]);
    }
    return item;
}

void freeItem(MapItem* item) {
    int len = sizeof(item->info)/sizeof(char*);
    for (int i = 0; i < len; i++)
        free(item->info[i]);
    free(item);
}

