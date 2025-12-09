#include "exec.h"
#include "conf.h"


int main(int nargs, char** args){
    
    Conf conf;
    read_conf("./conffile", &conf);
    printf("Compiler path: %s", conf.comp_path);
    printf("Root of tests: %s", conf.tests_root);
    printf("Test postfix: %s", conf.testfile_postfix);
    free_conf(&conf);

    return 0;
}