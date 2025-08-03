#include "header.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

bool write_file_header(FILE *file_ptr) {

  char header[] = {
    'A',
    'r',
    'c',
    'P',
    'a',
    'c',
    'k',
    VERSION,
    MAGIC,
    '\0'
  };

  int header_len = strlen(header);

  fwrite(header, sizeof(char), header_len, file_ptr);

  return true;
}

bool write_file_metadata() {

  return true;
}
