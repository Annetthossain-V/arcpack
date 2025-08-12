#include "header.h"
#include "globl.h"
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


bool write_file_metadata(FILE* arc_ptr, char* name, char* base, uint32_t size) {
  if (arc_ptr == NULL || name == NULL || base == NULL) {
    fputs("NULL ptr!\n", stderr);
    return false;
  }

  uint16_t File_Begin_Sig = FILE_BEGIN;
  uint8_t name_len = (uint8_t) strlen(name);
  uint16_t path_len = (uint16_t) strlen(base);
  uint32_t file_size = size;

  // write metadata
  FWRITE_MACRO(&File_Begin_Sig, sizeof(uint16_t), 1, arc_ptr);

  FWRITE_MACRO(&name_len, sizeof(uint8_t), 1, arc_ptr);
  FWRITE_MACRO(&path_len, sizeof(uint16_t), 1, arc_ptr);
  FWRITE_MACRO(&file_size, sizeof(uint32_t), 1, arc_ptr);

  // write name and base/path 
  FWRITE_MACRO(name, sizeof(char), name_len, arc_ptr);
  FWRITE_MACRO(base, sizeof(char), path_len, arc_ptr);

  uint16_t data_begin = FILE_DATA_BEGIN;
  FWRITE_MACRO(&data_begin, sizeof(uint16_t), 1, arc_ptr);

  return true;
}

bool write_dir_metadata(FILE *arc_ptr, char *name, char *base) {
  if (arc_ptr == NULL || name == NULL || base == NULL) {
    fputs("Error null ptr!\n", stderr);
    return false;
  }


  uint16_t Dir_Begin_Sig = DIR_BEGIN;
  uint8_t name_len = (uint8_t) strlen(name);
  uint16_t path_len = (uint16_t) strlen(base);

  // write metadata
  FWRITE_MACRO(&Dir_Begin_Sig, sizeof(uint16_t), 1, arc_ptr);  

  FWRITE_MACRO(&name_len, sizeof(uint8_t), 1, arc_ptr);
  FWRITE_MACRO(&path_len, sizeof(uint16_t), 1, arc_ptr);

  FWRITE_MACRO(name, sizeof(char), name_len, arc_ptr);

  FWRITE_MACRO(base, sizeof(char), path_len, arc_ptr);

  uint16_t data_start = DIR_DATA_BEGIN;
  FWRITE_MACRO(&data_start, sizeof(uint16_t), 1, arc_ptr);

  return true;
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
