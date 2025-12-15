#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>


int main(int nargs, char** args){
    DIR* dir = opendir(".");

    for (struct dirent* entry = readdir(dir); entry != NULL; entry = readdir(dir)){
        printf("%s\n", entry->d_name);
    }
    closedir(dir);
    

    return 0;
}