#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

long int countDir(const char* path);
long int countProcs(const char* path);
long getFileLength(FILE* fp );