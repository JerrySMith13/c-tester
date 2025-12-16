#include "testfile.h"

#include <dirent.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

static bool entry_is_directory(const char* parent, struct dirent* entry){
#ifdef DT_DIR
    if (entry->d_type == DT_DIR){
        return true;
    }
    if (entry->d_type != DT_UNKNOWN){
        return false;
    }
#endif
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
        return false;
    }
    char* full_path = stitch_path(parent, entry->d_name);
    if (full_path == NULL){
        return false;
    }
    struct stat st;
    int stat_res = stat(full_path, &st);
    free(full_path);
    if (stat_res < 0){
        return false;
    }
    return S_ISDIR(st.st_mode);
}
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
        if (entry_is_directory(root_path, entry)){
            continue;
        }
    }
    closedir(dir);
    return 0;
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
    return new_prefix;
}

int search_tests(TestList* to_add, DIR* dir, Conf* config, const char* prefix){
    struct dirent* entry = readdir(dir);
    DIR* found_dir;
    size_t added = 0;
    int numread = -1;
    do{
        Test newtest;
        if (entry && entry_is_directory(prefix, entry)){
            if ((found_dir = opendir(entry->d_name)) == NULL){
                printf("Error opening directory: %s\n", entry->d_name);
                perror("Errno val: ");
                return -1;
            }
            char* new_prefix = stitch_path(prefix, entry->d_name);
            if (new_prefix == NULL){
                printf("Error stitching path\n");
            }
            
            if((numread = search_tests(to_add, found_dir, config, new_prefix)) < 0){
                closedir(found_dir);
                free(new_prefix);
                return -1;
            }
            added += numread;
            free(new_prefix);
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
    size_t len_filename = strlen(filename);
    size_t len_postfix = strlen(postfix);
    if (len_postfix == 0 || len_filename < len_postfix){
        return false;
    }
    return strcmp(filename + (len_filename - len_postfix), postfix) == 0;
}
