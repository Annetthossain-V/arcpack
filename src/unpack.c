
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

static uint32_t file_len = 0; // in byte
static uint32_t curr_len = 0;

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

  if (!read_file_header(arc_ptr)) {
    fprintf(stderr, "%s invalid or corrupted file!\n", file);
    return false;
  }
  
  if (!sig_check(arc_ptr, file)) {
    fprintf(stderr, "error reading entry header!\n");
    return false;
  }

  return true;
}

bool sig_check(FILE* arc_ptr, char* name) {
  // init stuff
  if (file_len == 0) {
    
  }

  if (curr_len >= file_len)
    return true;
  

  uint16_t sig;
  FREAD_MACRO(&sig, sizeof(uint16_t), 1, arc_ptr)

  switch (sig) {
    case FILE_BEGIN:
      break;
    case DIR_BEGIN:
      break;
    default:
      return false;
  }

  return true;
}

