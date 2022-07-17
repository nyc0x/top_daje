#define UNUSED -1

typedef struct MapItem {
    int key;              //PID
    char** info;          //array string which contains process data
    int n_str;
} MapItem;

typedef struct Map {
    MapItem** items;      //pointer to pointer to map items
    int size;             //map size
    int used;             //keep track of used items
} Map;

//Creates a map
Map* createMap(int size);

//Creates an item of the map
MapItem* createItem(Map* map, int key, char** info, int n_str);

//Frees an item
void freeItem(MapItem* item);

//Frees a map
void freeMap(Map* map);

//Inserts an item into the map
void insertItem(Map* map, MapItem* item);

//Search for the item at index 'key' and returns its info buffer, if not NULL
char** getInfoBuf(Map* map, int key);

//Prints the structure
void mapPrint(Map* map);
