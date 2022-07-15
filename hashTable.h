typedef struct HashItems {
    int key;        //PID
    char* buf[];    //array string which contains PID data
} HashItems;

typedef struct HashTable {
    HashItems** ptr;    //pointer to pointer to hash items
    int size;           //hash size
    int used;           //keep track of used items
} HashTable;
