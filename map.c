#include "map.h"
#include <stdlib.h>
#include <string.h>

Map* createMap(int size) {
    Map* m = (Map*)malloc(sizeof(Map));
    m->items = (MapItem**)malloc(sizeof(MapItem*));
    m->size = size;
    m->used ^= m->used;
    for (int i = 0; i < m->size; i++)
        m->items[i] = NULL;
    return m;
}

MapItem* createItem(Map* m, int key, char** info) {
    MapItem* item = (MapItem*)malloc(sizeof(MapItem));
    item->key = key;
    int len = sizeof(info)/sizeof(char*);
    item->info = (char**)malloc(sizeof(char*)*len);
    for (int i=0; i<len; i++) {
        item->info[i] = (char*)malloc(sizeof(char));
        strcpy(item->info[i], info[i]);
    }
    m->used++;
    return item;
}

void freeItem(MapItem* item) {
    int len = sizeof(item->info)/sizeof(char*);
    for (int i = 0; i < len; i++)
        free(item->info[i]);
    free(item);
}

void freeMap(Map* m) {
    for (int i = 0; i < m->size; i++)
        if (m->items[i])
            free(m->items[i]);
    free(m->items);
    free(m);
}

MapItem* insertItem(Map* m, int key, char** info) {
    MapItem* item = createItem(m, key, info);
    m->items[m->used] = item; /*
                                m->used tells us there are m->used items in the map, so the last used index will be
                                m->used-1, so we will link the new item at m->used index
                              */
    m->used++;    
}

