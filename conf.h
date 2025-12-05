typedef struct Conf{
  char* tests_root;
  char* testfile_postfix;
  char* comp_path;
  char* comp_options;
  unsigned int max_time;
} ConfObject;

enum ConfRes{


    Ok = 0,
};

ConfRes read_conf(const char* path, Conf* conf);
