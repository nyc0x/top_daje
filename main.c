#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define SYS_STATS_PATH "/proc/stat" //[TODO]: add to .h file
#define PROC_PATH "/proc/" //[TODO]: add to .h file

long getFileLength(FILE* fp){
    long ret = -1;
    if(fp){
        if(fseek(fp, 0, SEEK_END) < 0) 
            exit(EXIT_FAILURE);
        
        ret = ftell(fp);
        
        if(fseek(fp, 0, SEEK_SET) < 0) 
            exit(EXIT_FAILURE);
    }
    return ret;
}

int isDirectory(char *path) {
   struct stat statbuf;
   if (stat(path, &statbuf) != 0)
       return 0;
   return S_ISDIR(statbuf.st_mode);
}

long getNumDir(const char* path) {
    if (!path)
        path = PROC_PATH;
   
    DIR* dir = opendir(path);
    
    struct dirent* ptr;
    ptr = readdir(dir);
    /*while (ptr) {
        if (isDirectory(ptr->d_name))
    }*/
}

int getNumberProcs(FILE* f, char* buf) {
    if (f && buf) {

    }
}

/*
    for dir in directories:
        Open 'stat'
            fai parsing del file.
            riempi l'array con pid name....
            filli la mappa.
*/

int main(int argc, char** argv){

    FILE* f = fopen(SYS_STATS_PATH, "r");

    if (!f) {
        printf("Error while opening %s file: %s\n", SYS_STATS_PATH, strerror(errno));
        exit(EXIT_FAILURE);
    }

    //int fileLen = (int)getFileLength(f);
    //char* buf = (char*)malloc(sizeof(char)*fileLen);
    //int n_proc = getNumberProcs(f, buf);
    
    //managing read op from /proc directory
    const char* s = "./";
    DIR* dir = opendir(s);
    char** buf = (char**)malloc(sizeof(char*)*30000);
    
    for (int i = 0; i < 30000; i++) {
        buf[i] = (char*)malloc(sizeof(char));
        buf[i] = NULL;
    }
    if (!dir) {
        printf("Error while opening %s directory: %s\n", s, strerror(errno));
        exit(EXIT_FAILURE);
    }
    struct dirent* ptr;
    
    int n_dir = 0;
    ptr = readdir(dir);
    while (ptr != NULL) {
        //strcpy(buf[i++], ptr->d_name);
        printf("%s\n", ptr->d_name);
        if (ptr->d_type == 4 && ptr->d_name[0] != '.')
            n_dir++;
        ptr = readdir(dir); //updates the pointer to the next directory's entry
    }
    printf("Numero di directory: %d\n", n_dir);

    //for (i = 0; i < 30000; i++)
    //    printf("buf[%d]: %s\n", i, buf[i]);

    //everything's alright, we can proceed reading the stuff inside the directory

    if (closedir(dir) < 0)
        printf("Error while closing %s directory: %s\n", s, strerror(errno)); 

    return 0;

}