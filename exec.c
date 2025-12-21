#include "exec.h"

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/wait.h>

int exec_test(const Test* test, int* res_code){
  // Construct outfile name
  size_t if_len = strlen(test->filename);
  size_t of_path_len = if_len + TEST_EXT_LEN + 1;
  char* of_path = (char*) malloc(of_path_len);

  if (of_path == NULL){

    free(of_path); 
    return -1; //Error handling needed
  }
  memcpy(of_path, test->filename, if_len);
  memcpy(of_path + if_len, TEST_TEMP_EXT, TEST_EXT_LEN);
  

  char** comp_args = malloc((sizeof(char*)) * (1 + 1 + 2 + 1));
  if (comp_args == NULL){                                           //path of compiler
    free(of_path);
    return -1; //Error handling needed                                   //input file
  }                                                                          //-o + output fule
  comp_args[0] = test->conf->comp_path;
  comp_args[1] =malloc(if_len + 1);
  memcpy(comp_args[1], test->filename, if_len + 1);
  comp_args[2] = "-o";
  comp_args[3] = of_path;
  comp_args[4] = NULL;

  pid_t id = fork();
  int compiler_exit = 0;
  switch (id){
    case -1:
      perror("fork");
      free(of_path);
      free(comp_args);
      _exit(127);
    case 0:
      printf("Compiling %s ...", test->filename);
      execv(test->conf->comp_path, comp_args);
      // Only runs if error occurs
      perror("execv");
      free(of_path);
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
          free(comp_args);
          _exit(127);
      }
      if (WIFEXITED(status)) {
          compiler_exit = WEXITSTATUS(status);
      }
    }

  }

  if (compiler_exit != 0){
    printf("Compiler error in test %s\n", test->filename);
    *res_code = compiler_exit;
    free(of_path);
    free(comp_args);
    return -1;
  }  
  
  id = fork();

  switch (id){
    case -1:
      perror("fork");
      free(of_path);
      free(comp_args);
      _exit(127);
    case 0: {
      char* exec_n[2] = {of_path, NULL};
      execv(of_path, exec_n);
    }
    default: {
      pid_t pid = 0;
      int status;

      do {
        pid = waitpid(pid, &status, 0);
      } while (pid == -1 && errno == EINTR);

      if (WIFEXITED(status)){
        *res_code = status;
      }
      remove(of_path);
    }
  }
  
  free(of_path);
  free(comp_args);
  
  
  return 0;

}

int free_test(Test* test){
  free((void*) test->filename);
  return 0;
}