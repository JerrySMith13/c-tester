#include "exec.h"
#include "conf.h"
#include "testfile.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>


int main(int nargs, char** args){
    TestList new_list;
    Conf conf;

    if (init_list(&new_list) < 0){
        perror("Malloc error: ");
        return -1;
    }

    if (read_conf("./conffile", &conf) != Ok){
        return -1;
    }
    int num_read = find_tests(conf.tests_root, &new_list, &conf);

    if (num_read < 0){
        return -1;
    }
    else {
        return 0;
    }
    

    
}