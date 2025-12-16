#ifndef TESTFILE_H
#define TESTFILE_H

#include "exec.h"
#include "conf.h"
#include <string.h>

#include <stdlib.h>
<<<<<<< HEAD
#include <stdbool.h>
=======
#include <stdio.h>
>>>>>>> 1cbdd5aae1fd42e544f522d2366a7122a26edbcf
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

int init_list(TestList* list){
    list->start = (Test*) malloc(sizeof(Test) * 10);
    list->capacity = 10;
    list->len = 0;
    if (list->start == NULL){
        return -1;
    }
    else return 0;
}

int TestList_push(TestList* list, Test* to_push);

int find_tests(const char* root_path){
    DIR* dir = opendir(root_path);
    dirent* entry;
    if (dir == NULL){
        return -1; //Error handling needed
    }
    while ((entry = readdir(dir)) != NULL){
        if (entry->d_type ==DT_DIR){

        }
    };
}

char* stitch_path(const char* prefix, const char* name){
    size_t len_prefix = strlen(prefix);
    size_t len_name = strlen(name);
    char* new_prefix = (char*) malloc(len_prefix + len_name + 2);
    if (new_prefix == NULL){
        printf("Malloc error\n");
        perror("errno: ");
        return NULL;
    }
    memcpy(new_prefix, prefix, len_prefix);
    new_prefix[len_prefix] = '/';
    memcpy(new_prefix + len_prefix + 1, name, len_name);
    new_prefix[len_prefix + len_name + 1] = '\0';
}

int search_tests(TestList* to_add, DIR* dir, Conf* config, const char* prefix){
    struct dirent* entry = readdir(dir);
    DIR* found_dir;
    size_t added = 0;
    int numread = -1;
    do{
        Test newtest;
        if (entry && entry->d_type == DT_DIR){
            if ((found_dir = opendir(entry->d_name)) == NULL){
                printf("Error opening directory: %s\n", entry->d_name);
                perror("Errno val: ");
                return -1;
            }
            size_t len_prefix = strlen(prefix);
            size_t len_name = strlen(entry->d_name);

            char* new_prefix = stitch_path(prefix, entry->d_name);
            if (new_prefix == NULL){
                printf("Error stitching path\n");
            }
            
            if((numread = search_tests(to_add, found_dir, config, new_prefix)) < 0){
                closedir(found_dir);
                return -1;
            }
            added += numread;
            closedir(found_dir);
        }
        else if(entry && entry->d_type == DT_REG){
            if (is_test(entry->d_name, config->testfile_postfix)){
                newtest = (Test) {
                    .conf = config,
                    .filename = stitch_path(prefix, entry->d_name),
                };
                if (TestList_push(to_add, &newtest) < 0){
                    printf("Malloc error with TestList, stopping test search...");
                    perror("errno: ");
                }
                added++;
            }
        }
        entry = readdir(dir);
    }
    while (entry != NULL);

    return 0;
}

bool is_test(const char* filename, const char* postfix){

}

int TestList_push(TestList* list, Test* to_push);
int find_tests(const char* root_path);
int search_dir(TestList* list_to_append, DIR* dir, Conf* config);
bool is_test(const char* filename, const char* postfix);
#endif