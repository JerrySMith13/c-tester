#include "exec.h"
#include "conf.h"


int main(int nargs, char** args){
    
    Conf conf = (Conf) {
        .tests_root = "/home/jeremy/projects/c-tester",
        .comp_path = "/usr/bin/gcc",
        .comp_args = args,
        .comp_argc = nargs,
        .max_time = 0,
        .testfile_postfix = "",
    };

    Test test = (Test) {
        .conf = &conf,
        .filename = "/test.c",
    };

    Result res;

    exec_test(&test, &res);

    return 0;
}