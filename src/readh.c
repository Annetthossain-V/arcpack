
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "readh.h"
#include "header.h"

bool read_file_header(FILE* arc_ptr) {

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
    '\0' // ignoring this btw
  };

  int header_len = strlen(header);

  char* data = (char*) malloc(header_len + 3); // really? A system call for that!

  __auto_type read = fread(data, sizeof(char), header_len, arc_ptr);
  if (read != header_len) {
    perror("file read failed");
    return false;
  }

  if (strcmp(data, header) != 0)
    return false; // cleanup is not that good tbh

  free(data);
  return true;
}
