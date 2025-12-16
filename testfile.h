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

int find_tests(const char* root_path);
char* stitch_path(const char* prefix, const char* name);
int search_tests(TestList* to_add, DIR* dir, Conf* config, const char* prefix);
bool is_test(const char* filename, const char* postfix);

int TestList_push(TestList* list, Test* to_push);
int find_tests(const char* root_path);
int search_dir(TestList* list_to_append, DIR* dir, Conf* config);
#endif