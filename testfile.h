#ifndef TESTFILE_H
#define TESTFILE_H

#include "exec.h"

#include <stdlib.h>
#include <dirent.h>

typedef struct TestList{
    size_t capacity;
    size_t len;
    Test* start;

} TestList;

typedef enum ReadTestRes{

} ReadTestRes;

int init_list(TestList* list){
    list->start = (Test*) malloc(sizeof(Test) * 10);
    list->capacity = 10;
    list->len = 0;
    if (list->start == NULL){
        return -1;
    }
    else return 0;
}

int find_tests(const char* root_path){
    DIR* dir = opendir(root_path);
    dirent* entry;
    if (dir == NULL){
        return -1; //Error handling needed
    }
    while ((entry = readdir(dir)) != NULL){
        if (entry->d_type )
    };
}

bool is_test(const char* filename, const char* postfix){

}

#endif