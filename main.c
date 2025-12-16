#include "exec.h"
#include "conf.h"
#include "testfile.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>


int main(int nargs, char** args){
    TestList new_list;

    if (init_list(&new_list) != 0){
        printf("Error in init_list\n");
        goto end;
    }

    Test new_test;
    for(char i = 'a'; i < '9'; i++){
        const char* name = {i, '\0'};
        new_test.filename = name;
        TestList_push(&new_list, &new_test);
    }

    for(int i = 0; i < new_list.len; i++){
        printf(new_list.start[i].filename);
    }
    printf("\n%ld entries printed", new_list.len);

    end: 
    perror("errno:");
    return 0;
}