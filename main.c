#include "exec.h"
#include "conf.h"

#include <stdio.h>
#include <fcntl.h>

char* find_conf(){

}

int main(int nargs, char** args){
    Conf conf;
    if (nargs < 2){

    }
    else {
        if(read_conf(args[1], &conf) != Ok){
            printf("conf: error reading file");
            return -1;
        }
    }

    

    return 0;
}