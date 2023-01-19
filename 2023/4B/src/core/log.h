#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  ERROR = -2,  // abort if error
  WARNING = -1,
  INFO = 0
} VerbosityType;

#define AITISA_LOG(verbosity, message_string)                                \
  if (verbosity == ERROR) {                                                  \
    fprintf(stderr, "[%s] %s, %s(), line %d:\n\t%s\n", #verbosity, __FILE__, \
            __FUNCTION__, __LINE__, message_string);                         \
    abort();                                                                 \
  } else {                                                                   \
    fprintf(stdout, "[%s] %s, %s(), line %d:\n\t%s\n", #verbosity, __FILE__, \
            __FUNCTION__, __LINE__, message_string);                         \
  }

#define AITISA_CHECK(cond)                                              \
  if (!(cond)) {                                                        \
    fprintf(stderr, "[ERROR] %s, %s(), line %d:\n\tCHECK FAILED: %s\n", \
            __FILE__, __FUNCTION__, __LINE__, #cond);                   \
    abort();                                                            \
  }

#endif