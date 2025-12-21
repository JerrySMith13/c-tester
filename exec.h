#ifndef EXEC_H
#define EXEC_H

#include "conf.h"

#define TEST_TEMP_EXT ".temp"
#define TEST_EXT_LEN 6

typedef struct Test{

  char* filename; // Includes postfix + any additional paths before it
  const Conf* conf;

} Test;


/*
  Process for running a test
  1. compile file into tempfile using specified compiler
  2. run tempfile and capture stdout + stderr.
    -stdin can be specified in conf file or captured directly by terminal
  3. Capture status code and generate string to describe test results


  Step 1: compile tempfile:
    - First, we get the compiler path + options from our confobject.
    - Second, we stitch together our filename + rooot dir for an absolute path
    - Third, we use execv to execute compiler with args we can pass to it + output spec (which will be generated in test root)
        - note: output file should conatin postfix which can be used to detect temp files to delete

  Step 2: run tempfile:
    - First, we 
*/

int exec_test(const Test* test, int* res_code);

int free_test(Test* test);

#endif