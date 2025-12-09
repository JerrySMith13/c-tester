#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <string.h>

#include "conf.h"

int proc_line(char* line, size_t len, Conf* conf);

void clear_conf(Conf* conf){
    conf->comp_args = NULL;
    conf->comp_path = NULL;
    conf->testfile_postfix = NULL;
    conf->tests_root = NULL;
    conf->comp_argc = 0;
    conf->max_time = 0;
}

enum ConfRes read_conf(const char* path, Conf* conf){
    int fd;
    struct stat sizestat;
    char* file;
    clear_conf(conf);
    if ((fd = open(path, O_RDONLY)) < 0){
        perror("Error reading config file:");
        return FileErr;
    }

    if(fstat(fd, &sizestat) < 0){
        perror("Error reading config file:");
        return FileErr;
    }

    if((file = malloc(sizestat.st_size)) == NULL){
        printf("Error allocating memory to read file");
        return FileErr;
    }

    if(pread(fd, file, sizestat.st_size, 0) < 0){
        perror("Error reading file:");
        free(file);
        return FileErr;
    }
    close(fd);

    size_t line_len = 0;
    size_t line_cnt = 1;

    for(int i = 0; i < sizestat.st_size; i++){
        if(file[i] == '\n' || i == sizestat.st_size - 1){
            if (proc_line(file + (i - line_len), line_len, conf) < 0){
                printf("Error processing line %zu", line_cnt);
                free(file);
                return ParsErr;
            }
            line_len = 0;
        }
        line_len++;
    }


    free(file);
    return Ok;
}
//Warning: weird parsing shit goes down here
int proc_line(char* line, size_t len, Conf* conf){
    char arg[CONF_ARG_MAXSIZE + 1];
    char* val;
    size_t argsize = 0;
    size_t val_offset = -1;
    for(size_t i = 0; i < len; i++){
        if (line[i] == '='){
            val_offset = i + 1;
            arg[argsize] = '\0';
            break;
        }
        //Ignore spaces, even if in-between word
        else if (line[i] == ' '){
            continue;
        }
        //This indicates that the argument is too big
        else if(argsize > CONF_ARG_MAXSIZE){
            arg[CONF_ARG_MAXSIZE] = '\0';
            printf("Error: argument not valid: %s", arg);
            return ParsErr; //Error handling needed
        }

        arg[argsize] = line[i];
        argsize++;
    }
    if (val_offset == -1) {
        return ParsErr; //Error handling needed
    }
    if ((val = malloc(len - argsize + 1)) == NULL){
        return AllocErr;
    };
    memcpy(val, line + val_offset, len - argsize - 1);
    val[len - argsize] = '\0';

    if (strcmp(arg, "root_path") == 0){
        conf->tests_root = realpath(val, NULL);
        free(val);
        printf("root: %s\n", val);
        if (conf->tests_root == NULL){
            perror("Error was: ");
            return AllocErr;
        }
    }
    else if(strcmp(arg, "suffix") == 0){
        conf->tests_root = val;
    }
    else if(strcmp(arg, "compiler") == 0){
        conf->comp_path = realpath(val, NULL);
        free(val);
        if (conf->comp_path == NULL){
            printf("Error reading root_path in conf file");
            return AllocErr;
        }
    }
    else if(strcmp(arg, "max_runtime") == 0){
        conf->max_time = strtol(val, NULL, 0);
        free(val);
        if (errno == EINVAL){
            printf("Error reading max_runtime in conf file!");
            return ParsErr;
        }
    }
    else {
        printf("Error reading arg \"%s\": invalid argument", arg);
        free(val);
        return ParsErr;
    }

    return Ok;
    
    
}

void free_conf(Conf* conf){
    free(conf->comp_path);
    free(conf->testfile_postfix);
    free(conf->tests_root);
}