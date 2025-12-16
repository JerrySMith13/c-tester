#ifndef TESTFILE_H
#define TESTFILE_H

#include "exec.h"
#include "conf.h"
#include <string.h>

#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>

typedef struct TestList{
    size_t capacity;
    size_t len;
    Test* start;

} TestList;

typedef enum ReadTestRes{
    TestOk,
} ReadTestRes;

int init_list(TestList* list);

int TestList_push(TestList* list, Test* to_push);
int find_tests(const char* root_path);
int search_dir(TestList* list_to_append, DIR* dir, Conf* config);
bool is_test(const char* filename, const char* postfix);
#endif