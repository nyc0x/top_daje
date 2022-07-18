#include "map.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


/*
    descr:
    args:
    retval: 
    author: [NDP]
*/
Map* createMap(int size) {
    Map* m = (Map*)malloc(sizeof(Map));
    m->items = (MapItem**)malloc(sizeof(MapItem*)*size);
    m->size = size;
    m->used ^= m->used;
    for (int i = 0; i < m->size; i++)
        m->items[i] = NULL;
    return m;
}

//NOTA: non puoi creare un item con info NULL <-> ossia con 0 informazioni.
/*
    descr:
    args:
    retval: 
    author: [NDP]
*/
MapItem* createItem(Map* m, int key, char** info, int n_str) {
    MapItem* item = NULL;
    if(!(m || info)){
        item = (MapItem*)malloc(sizeof(MapItem));
        item->key = key;
        item->n_str = n_str;
        item->info = (char**)malloc(sizeof(char*)*n_str);
        for (int i=0; i<n_str; i++) {
            item->info[i] = (char*)malloc(sizeof(char));
            if(info[i])
                strcpy(item->info[i], info[i]);
        }
        if(item->key < m->size){
            m->items[item->key] = item; 
            m->used++;
        }
    }
    return item;
}

//self-explanatory function
MapItem** getItemsList(Map* m) {
    return m ? m->items: NULL;
}

/*
    descr:
    args:
    retval: 
    author: [NDP]
*/
void freeItem(Map* m, MapItem* item) {
    if(! (item || m)){
        int key = item->key;
        for (int i = 0; i < item->n_str; i++){
            if(item->info[i])
                free(item->info[i]);
        }
        free(item->info);
        free(item);
        m->items[key] = NULL;
        m->size--;
        m->used--;
    }
}

/*
    descr:
    args:
    retval: 
    author: [NDP]
*/
void freeMap(Map* m) {
    if(m && m->items){
        for (int i = 0; i < m->size; i++)
            if (m->items[i])
                free(m->items[i]);
        free(m->items);
        free(m);
    }
}


/*
    descr:
    args:
    retval: 
    author: [NDP]
*/
void mapPrint(Map* m) {
    if(m && m->items){
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
    }
    return;
}
