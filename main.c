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
        printf("Conf error");
        return -1;
    }
    int num_read = find_tests(conf.tests_root, &new_list, &conf);

    if (num_read < 0){
        printf("Error searching for tests");
        return -1;
    }
    
    Test test;
    int exit_code;
    int test_exit;
    printf("TestList len: %ld\n", new_list.len); 
    for (size_t i = 0; i < new_list.len; i++){
        test = new_list.start[i];
        exit_code = exec_test(&test, &test_exit);
        if (exit_code < 0){
            printf("Error in test %s", test.filename);
            goto end;
        }
        printf("Test %s returned %d\n", test.filename + strlen(conf.tests_root) + 1, test_exit);

    }
    
    end:{
        TestList_dealloc(&new_list);
        free_conf(&conf);

        return 0;
    }

    
}