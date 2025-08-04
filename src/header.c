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

bool write_file_metadata(FILE* arc_ptr, char* name, char* base, uint32_t size) {
  if (arc_ptr == NULL || name == NULL || base == NULL)
    return false;

  unsigned long write = 0;

  uint8_t name_len = (uint8_t) strlen(name);
  uint16_t path_len = (uint16_t) strlen(base);
  uint32_t file_size = size;

  // write metadata
  write = fwrite(&name_len, sizeof(uint8_t), 1, arc_ptr);
  if (write < 1)
    goto err;

  write = fwrite(&path_len, sizeof(uint16_t), 1, arc_ptr);
  if (write < 1)
    goto err;

  write = fwrite(&file_size, sizeof(uint32_t), 1, arc_ptr);
  if (write < 1)
    goto err;

  // write name and base/path 
  write = fwrite(name, sizeof(char), name_len, arc_ptr); // this is writting 
  if (write < name_len)
    goto err;

  write = fwrite(base, sizeof(char), path_len, arc_ptr); // the null terminator?
  if (write < path_len)
    goto err;

  uint16_t data_begin = FILE_DATA_BEGIN;
  write = fwrite(&data_begin, sizeof(uint16_t), 1, arc_ptr);
  if (write < 1)
    goto err;

  goto ret;
err:
  perror("Unable to write to file");
  return false;

ret:
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
