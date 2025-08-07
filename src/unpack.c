
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include "unpack.h"
#include "globl.h"
#include "pack.h"
#include "header.h"
#include "readh.h"

bool extract_archive(char* file) {
  if (file == NULL)
    return false;

  FILE* arc_ptr = fopen(file, "rb");
  if (arc_ptr == NULL) {
    fprintf(stderr, "file %s does not exist!\n", file);
    return false;
  }

  // check if file is a dir
  struct stat statbuf;
  if (stat(file, &statbuf) == -1) {
    perror("stat");
    return false;
  }

  if (S_ISDIR(statbuf.st_mode)) {
    fprintf(stderr, "%s is a directory!\n", file);
    return false;
  }

  

  return true;
}
