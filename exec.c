#include "exec.h"

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int exec_test(const Test* test, Result* surrogate){
  // Construct outfile name
  size_t if_len = strlen(test->filename);
  size_t root_len = strlen(test->conf->tests_root);
  size_t of_path_len = if_len + root_len + TEST_EXT_LEN + 1;
  char* of_path = (char*) malloc(of_path_len);
  char* if_path = (char*) malloc(root_len + if_len + 1);
  if (of_path == NULL || if_path == NULL){
    if (if_path != NULL) { free(if_path); }
    if (of_path != NULL) { free(of_path); }
    return -1; //Error handling needed
  }
  memcpy(of_path, test->conf->tests_root, root_len);
  memcpy(of_path + root_len, test->filename, if_len);

  memcpy(if_path, of_path, root_len + if_len);
  if_path[root_len + if_len] = '\0';

  memcpy(of_path + root_len + if_len, TEST_TEMP_EXT, TEST_EXT_LEN);
  of_path[of_path_len - 1] = '\0';


  char** comp_args = malloc((sizeof(char*)) * (test->conf->comp_argc + 1 + 1 + 2 + 1));
  if (comp_args == NULL){                                           //path of compiler
    free(of_path);
    free(if_path);
    return -1; //Error handling needed                                   //input file
  }                                                                          //-o + output fule
  comp_args[0] = test->conf->comp_path;
  comp_args[1] = if_path;
  memcpy(comp_args + 2, test->conf->comp_args, (sizeof(char*)) * test->conf->comp_argc); 
  comp_args[test->conf->comp_argc + 2] = "-o";
  comp_args[test->conf->comp_argc + 3] = of_path;
  comp_args[test->conf->comp_argc + 4] = NULL;
  
  pid_t id = fork();
  int compiler_exit = 0;
  switch (id){
    case -1:
      perror("fork");
      free(of_path);
      free(if_path);
      free(comp_args);
      _exit(127);
    case 0:
      printf("Compiling %s ...", test->filename);
      int ret = execv(test->conf->comp_path, comp_args);
      perror("execv");
      free(of_path);
      free(if_path);
      free(comp_args);
      _exit(127);
      break;
    default: {
      int status;
      pid_t w;
      do {
          w = waitpid(id, &status, 0);
      } while (w == -1 && errno == EINTR);
      if (w == -1) {
          perror("waitpid");
          free(of_path);
          free(if_path);
          free(comp_args);
          _exit(127);
      }
      if (WIFEXITED(status)) {
          compiler_exit = WEXITSTATUS(status);
      }
    }

  }

  if (compiler_exit != 0){
    surrogate->status = -1;
    surrogate->test_name = test->filename;
    surrogate->test_out = NULL;
    free(of_path);
    free(if_path);
    free(comp_args);
    return -1;
  }  
  
  id = fork();

  switch (id){
    case -1:
      perror("fork");
      free(of_path);
      free(if_path);
      free(comp_args);
      _exit(127);
    case 0:
      execv(of_path, NULL);
    default: {
      pid_t pid;
      int status;

      do {
        pid = waitpid(pid, &status, 0);
      } while (pid == -1 && errno == EINTR);

      if (WIFEXITED(status)){
        surrogate->status = status;
        surrogate->test_name = test->filename;
        surrogate->test_out = NULL;
      }
      remove(of_path);
    }
  }
  
  free(of_path);
  free(if_path);
  free(comp_args);
  
  
  return 0;

}