#define UNUSED -1

/*
  Map implemented as an arraylist of MapItem.
  every MapItem has key equal to the index of the MapItem in the Map 
  since every process has a different PID we won't have any collision, so we're highly 
  efficient.
*/
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
void freeItem(Map* m, MapItem* item);

//Frees a map
void freeMap(Map* map);

//Search for the item at index 'key' and returns its info buffer, if not NULL
char** getInfoBuf(Map* map, int key);

//Returns a MapItem** object which points to all MapItem elements in the map
MapItem** getItemsList(Map* m);

//Prints the structure
void mapPrint(Map* map);
