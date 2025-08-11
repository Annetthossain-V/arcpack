
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "readh.h"
#include "header.h"
#include "globl.h"

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

  unsigned int header_len = strlen(header);

  char* data = (char*) malloc(header_len + 3); // really? A system call for that!

  __auto_type read = fread(data, sizeof(char), header_len, arc_ptr);
  if (read != header_len) {
    perror("file read failed");
    return false;
  }

  if (data[7] != VERSION) {
    fputs("Error! Incompatible Version!\n", stderr);
    return false;
  }

  if (strcmp(data, header) != 0)
    return false; // cleanup is not that good tbh

  free(data);
  return true;
}

bool read_dir_metadata(FILE *arc_ptr, struct dir_metadata* info) {
  FREAD_MACRO(&info->name_len, sizeof(uint8_t), 1, arc_ptr);
  FREAD_MACRO(&info->path_len, sizeof(uint16_t), 1, arc_ptr);
  
  info->name = (char*) malloc(info->name_len);
  info->base = (char*) malloc(info->path_len);

  FREAD_MACRO(info->name, sizeof(char), info->name_len, arc_ptr);
  FREAD_MACRO(info->base, sizeof(char), info->path_len, arc_ptr);

  return true;;
}

bool read_file_metadata(FILE *arc_ptr, struct file_metadata *info) {
  FREAD_MACRO(&info->name_len, sizeof(uint8_t), 1, arc_ptr);
  FREAD_MACRO(&info->path_len, sizeof(uint16_t), 1, arc_ptr);
  
  FREAD_MACRO(&info->file_size, sizeof(uint32_t), 1, arc_ptr);

  info->name = (char*) malloc(info->name_len);
  info->base = (char*) malloc(info->path_len);

  FREAD_MACRO(info->name, sizeof(char), info->name_len, arc_ptr);
  FREAD_MACRO(info->base, sizeof(char), info->path_len, arc_ptr);

  return true;
}

