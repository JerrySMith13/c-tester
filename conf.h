#ifndef CONF_H
#define CONF_H

// ALL PATHS SHOULD END W/O A SLASH (/).

typedef struct Conf{
  char* tests_root;
  char* testfile_postfix;
  char* comp_path; //Guaranteed to be a valid executable 
  char** comp_args; 
  int comp_argc;
  unsigned int max_time;
} Conf;

enum ConfRes{


    Ok = 0,
};

enum ConfRes read_conf(const char* path, Conf* conf);

#endif