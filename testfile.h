#ifndef TESTFILE_H
#define TESTFILE_H

#include "exec.h"
#include "conf.h"
#include <string.h>

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>

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
int TestList_dealloc(TestList* list);


int find_tests(const char* root_path, TestList* to_push, const Conf* config);

#endif