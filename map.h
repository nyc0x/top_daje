#define UNUSED -1

typedef struct MapItem {
    int key;                //PID
    char** info;           //array string which contains PID data
} MapItem;

typedef struct Map {
    MapItem** items;      //pointer to pointer to map items
    int size;           //map size
    int used;           //keep track of used items
} Map;

//creates a pointer to the main structure
Map* createMap(int size);

//creates an item of the map
MapItem* createItem(Map* map, int key, char** info);

//frees an item
void freeItem(MapItem* item);

//frees a map
void freeMap(Map* map);

//inserts an item into the map
MapItem* insertItem(Map* map, int key, char** info);

//search for the item at index 'key' and returns its info buffer, if not NULL
char** getInfoBuf(Map* map, int key);
