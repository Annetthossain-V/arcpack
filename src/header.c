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
    '\0' // ignoring this btw
  };

  int header_len = strlen(header);

  unsigned long written = fwrite(header, sizeof(char), header_len, file_ptr);
  if (written < (unsigned long) header_len) {
    perror("error writting to file");
    return false;
  }

  return true;
}

#define CHK_MACRO(a, b, c) \
        if (a < b)         \
          goto c

bool write_file_metadata(FILE* arc_ptr, char* name, char* base, uint32_t size) {
  if (arc_ptr == NULL || name == NULL || base == NULL) {
    fputs("NULL ptr!\n", stderr);
    return false;
  }

  unsigned long write = 0;

  uint16_t File_Begin_Sig = FILE_BEGIN;
  uint8_t name_len = (uint8_t) strlen(name);
  uint16_t path_len = (uint16_t) strlen(base);
  uint32_t file_size = size;

  // write metadata
  write = fwrite(&File_Begin_Sig, sizeof(uint16_t), 1, arc_ptr);
  CHK_MACRO(write, 1, err);

  write = fwrite(&name_len, sizeof(uint8_t), 1, arc_ptr);
  CHK_MACRO(write, 1, err);

  write = fwrite(&path_len, sizeof(uint16_t), 1, arc_ptr);
  CHK_MACRO(write, 1, err);

  write = fwrite(&file_size, sizeof(uint32_t), 1, arc_ptr);
  CHK_MACRO(write, 1, err);

  // write name and base/path 
  write = fwrite(name, sizeof(char), name_len, arc_ptr); // this is writting 
  CHK_MACRO(write, name_len, err);

  write = fwrite(base, sizeof(char), path_len, arc_ptr); // the null terminator?
  CHK_MACRO(write, path_len, err);                       // no, it is not

  uint16_t data_begin = FILE_DATA_BEGIN;
  write = fwrite(&data_begin, sizeof(uint16_t), 1, arc_ptr);
  CHK_MACRO(write, 1, err);

  // return 
  return true;

err:
  perror("Unable to write to file");
  return false;
}

bool write_dir_metadata(FILE *arc_ptr, char *name, char *base) {
  if (arc_ptr == NULL || name == NULL || base == NULL) {
    fputs("Error null ptr!\n", stderr);
    return false;
  }

  unsigned long write = 0;


  return true;
err:
  perror("unable to write dir metadata");
  return false;
}


bool write_file_end(FILE* arc_ptr) {
  uint16_t data_end = FILE_DATA_END;
  unsigned long i = fwrite(&data_end, sizeof(uint16_t), 1, arc_ptr);
  if (i < 1) {
    perror("unable to write to file");
    return false;
  }
  return true;
}

bool write_dir_end(FILE *arc_ptr) {
  uint16_t magic = DIR_DATA_END;
  unsigned long i = fwrite(&magic, sizeof(uint16_t), 1, arc_ptr);
  if (i < 1) {
    perror("unable to write to file");
    return false;
  }
  return true;
}
