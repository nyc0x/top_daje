#include "map.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Map* createMap(int size) {
    Map* m = (Map*)malloc(sizeof(Map));
    m->items = (MapItem**)malloc(sizeof(MapItem*)*size);
    m->size = size;
    m->used ^= m->used;
    for (int i = 0; i < m->size; i++)
        m->items[i] = NULL;
    return m;
}

MapItem* createItem(Map* m, int key, char** info, int n_str) {
    MapItem* item = (MapItem*)malloc(sizeof(MapItem));
    item->key = key;
    item->n_str = n_str;
    item->info = (char**)malloc(sizeof(char*)*n_str);
    for (int i=0; i<n_str; i++) {
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

void freeMap(Map* m) {
    for (int i = 0; i < m->size; i++)
        if (m->items[i])
            free(m->items[i]);
    free(m->items);
    free(m);
}

void insertItem(Map* m, MapItem* item) {
    m->items[item->key] = item; 
    m->used++;    
}

void mapPrint(Map* m) {
    printf("Map it's composed of %d items and %d of them are being used\n\n", m->size, m->used);
    int i, j;  
    for (i = 0; i < m->size; i++) {
        printf("Item at index %d: ", i);
        if (m->items[i]) {
            for (j = 0; j < m->items[i]->n_str - 1; j++) 
                printf("%s, ", m->items[i]->info[j]);
            printf("%s\n", m->items[i]->info[j]);
        }
        else
            printf("\n");
    }
    printf("\n");
    return;
}
