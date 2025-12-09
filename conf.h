#ifndef CONF_H
#define CONF_H
#include <limits.h>

#define CONF_ARG_MAXSIZE 20
// ALL PATHS SHOULD END W/O A SLASH (/).

typedef struct Conf{
  char* tests_root; //Free
  char* testfile_postfix; //Free
  char* comp_path; //Free
  char** comp_args; 
  int comp_argc;
  unsigned int max_time;
} Conf;

enum ConfRes{
    AllocErr = -3,
    FileErr,
    ParsErr,
    Ok,
};

enum ConfRes read_conf(const char* path, Conf* conf);

void clear_conf(Conf* conf);
void free_conf(Conf* conf);

#endif