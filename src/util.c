#include "util.h"

/*
    descr:  Return the length of a given binary file (as a long integer) without reading all it's characters.
    args:   [FILE* ] {fp} file pointer to the file. 
            NOTE that the file MUST be already opened! 
    retval: - #bytes of the file
            - -1 if error on fp 
    author: [NDP] [MZ]
*/
long getFileLength(FILE* fp){
    long ret = -1;
    if (fp) {
        if (fseek(fp,0,SEEK_END) < 0) exit(EXIT_FAILURE); /*TODO: add better error handling ! REMOVE exit(...) call*/
        ret = ftell(fp);
        if (fseek(fp, 0,SEEK_SET) < 0) exit(EXIT_FAILURE);
    }
    return ret;
}



/*
    descr:  This function returns the number of directories inside a given path,
            excluding the hidden (the ones with '.' at the beginning of their name) and nested ones 
    args:   [const char*] {path} valid absolute path where to count directories.
    retval: Non negative integer matching the number of non hidden directories in path.
    author: [NDP] 
*/
long int countDir(const char* path){
    if(!path) printf("Error %s\n", strerror(errno)); //TODO: add better error handling
    
    DIR* dir_p = opendir(path);
    
    if (!dir_p) {
        printf("Error while opening %s directory: %s\n", path, strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    long int num_dir=0;
    struct dirent* dir_data = readdir(dir_p); //Do not free this ptr (see man readdir)!
    while (dir_data) {
        if (dir_data->d_type == DT_DIR && dir_data->d_name[0] != '.')
            num_dir++;
        dir_data = readdir(dir_p);
    }

    if (closedir(dir_p) < 0)
        printf("Error while closing %s directory: %s\n", path, strerror(errno)); 

    return num_dir;
}

/*
    descr:  Works the same as countDir function except for the fact that this function 
            does not include in its count non pid-named directories
    args:   [const char*] {path} valid absolute path where to count directories.
    retval: Non negative integer matching the number of pid-named directories in path.
    author: [NDP] 
*/
long int countProcs(const char* path){
    if(!path) printf("Error %s\n", strerror(errno)); //TODO: add better error handling
    
    DIR* dir_p = opendir(path);
    
    if (!dir_p) {
        printf("Error while opening %s directory: %s\n", path, strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    long int num_procs=0;
    struct dirent* dir_data = readdir(dir_p); //Do not free this ptr (see man readdir)!
    while (dir_data) {
        if (dir_data->d_type == DT_DIR && dir_data->d_name[0] != '.' && atoi(dir_data->d_name) != 0)
            num_procs++;
        dir_data = readdir(dir_p);
    }

    if (closedir(dir_p) < 0)
        printf("Error while closing %s directory: %s\n", path, strerror(errno)); 

    return num_procs;
}



/*
    descr: 
    args:   
    retval: 
    author: [NDP] 
*/
void getAllPids(char** buf){
    DIR* dir = opendir(PROC_PATH);
    
    if (!dir) {
        printf("Error while opening %s directory: %s\n", PROC_PATH, strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    struct dirent* dir_data = readdir(dir); //Do not free this ptr (see man readdir)!
    int i=0;
    while (dir_data) {
        if (dir_data->d_type == DT_DIR && dir_data->d_name[0] != '.' && atoi(dir_data->d_name) != 0)
            buf[i++] = dir_data->d_name;
        dir_data = readdir(dir);
    }

    if (closedir(dir) < 0)
        printf("Error while closing %s directory: %s\n", PROC_PATH, strerror(errno)); 
    
    return;
}


