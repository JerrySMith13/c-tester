#include "testfile.h"

#include <dirent.h>
#include <stdlib.h>
#include <fcntl.h>
int init_list(TestList* list){
    list->start = (Test*) malloc(sizeof(Test) * 10);
    list->capacity = 10;
    list->len = 0;
    if (list->start == NULL){
        return -1;
    }
    else return 0;
}

int TestList_push(TestList* list, Test* to_push){
    if (list->len + 1 >= list->capacity){
        size_t new_cap = (int) ((double) list->capacity * 1.5);
        list->start = (Test*) realloc(list->start, list->capacity);
        if (list->start == NULL){
            return -1;
        }
        list->capacity = new_cap;
    }
    memcpy(list->start + list->len, to_push, sizeof(Test));
    return 0;
}

int find_tests(const char* root_path){
    DIR* dir = opendir(root_path);
    struct dirent* entry;
    if (dir == NULL){
        return -1; //Error handling needed
    }
    while ((entry = readdir(dir)) != NULL){
        
    };
    return 0;
}

int search_dir(TestList* list_to_append, DIR* dir, Conf* config){
    struct dirent* entry;

    do{
        entry = readdir(dir);

        if (entry->d_type == DT_REG && is_test(entry->d_name, config->testfile_postfix)){

        }
    }
    while(entry != NULL);

    return 0;
}

bool is_test(const char* filename, const char* postfix){
    return false;
}
