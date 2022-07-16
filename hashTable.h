typedef struct HashItem {
    int key;        //PID
    char* buf[];    //array string which contains PID data
} HashItem;

typedef struct HashTable {
    HashItem** ptr;    //pointer to pointer to hash items
    int size;           //hash size
    int used;           //keep track of used items
} HashTable;

//creates a pointer to the main structure
HashTable* createTable(int size);

HashItem