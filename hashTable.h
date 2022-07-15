typedef struct HashItems {
    int key;        //PID
    char* buf[];    //array string which contains PID data
} HashItems;

typedef struct HashTable {
    HashItems** ptr;
    int size;
    int used;
} HashTable;
