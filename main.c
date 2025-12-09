#include "exec.h"
#include "conf.h"

int main(int nargs, char** args){
    
    Conf conf;
    read_conf("./conffile", &conf);
    printf("Compiler path: %s\n", conf.comp_path);
    printf("Root of tests: %s\n", conf.tests_root);
    printf("Test postfix: %s\n", conf.testfile_postfix);
    printf("Max runtime: %zu", conf.max_time);
    free_conf(&conf);

    return 0;
}