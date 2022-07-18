#include "util.h"

/*
    descr:  Return the length of a given binary file (as a long integer) without reading all it's characters.
    args:   [FILE* ] {fp} file pointer to the file. 
            NOTE that the file MUST be already opened! 
    retval: - #bytes of the file
            - -1 if error on fp 
    author:
*/
long getFileLength(FILE* fp ){
    long ret = -1;
    if(fp){
        if(fseek(fp,0,SEEK_END) < 0 ) exit(EXIT_FAILURE); /*TODO: add better error handling ! REMOVE exit(...) call*/
        ret = ftell(fp);
        if(fseek(fp, 0,SEEK_SET) < 0 ) exit(EXIT_FAILURE);
    }
    return ret;
}



/*
    descr:  This function return the number of directories inside a given path,
             excludind the hidden (the ones with '.' at the beginning of their name ) and nested ones 
    args:   [const char *] {path} valid absolute path where to count directories.
    retval: Non negative integer matching the number of non hidden directories in path.
    author: [NDP] 
*/
int countDir(const char* path ){
    if(!path) printf("Error \n", strerror(errno)); //TODO: add better error handling
    
    DIR* dir_p = opendir(path);
    
    if (!dir_p) {
        printf("Error while opening %s directory: %s\n", path, strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    int num_dir=0;
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
