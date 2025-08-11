
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "unpack.h"
#include "globl.h"
#include "pack.h"
#include "header.h"
#include "readh.h"

#ifndef CHUNK_SIZE
  // 1 kb
  #define CHUNK_SIZE 512
#endif

static long size;

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
 
  size = fsize(file);
  if (!sig_check(arc_ptr)) {
    fprintf(stderr, "error reading entry header!\n");
    return false;
  }

  return true;
}

bool sig_check(FILE* arc_ptr) {
  
  if (feof(arc_ptr))
    return true;

  const long curr_read = ftell(arc_ptr);
  if (curr_read >= size)
    return true;

  uint16_t sig;
  FREAD_MACRO(&sig, sizeof(uint16_t), 1, arc_ptr)
  bool stat = false;

  switch (sig) {
    case FILE_BEGIN:
      stat = read_file(arc_ptr);
      break;
    case DIR_BEGIN:
      stat = read_dir(arc_ptr);
      break;
    case DIR_DATA_END:
    case FILE_DATA_END:
      fseek(arc_ptr, -2, SEEK_CUR);
      return true;
    default:
      return false;
  }

  if (!stat)
    return false;

  return true;
}

bool read_dir(FILE *arc_ptr) {
 
  // read dir header
  struct dir_metadata dir_header;
  if(!read_dir_metadata(arc_ptr, &dir_header)) {
    fputs("unable to read dir header!\n", stderr);
    return false;
  }

  uint16_t dir_magic = 0;
  FREAD_MACRO(&dir_magic, sizeof(uint16_t), 1, arc_ptr)
  if (dir_magic != DIR_DATA_BEGIN) {
    fputs("invalid dir magic!\n", stderr);
    return false;
  }
 
  // print name to console
  printf("x %s\n", dir_header.base);

  // make the dir 
  const mode_t permission = S_IRWXU | S_IRWXG | S_IRWXO;
  if (mkdir(dir_header.base, permission) != 0) {
    perror("unable to create directory!");
    return false;
  }

  // make the call
  if (!sig_check(arc_ptr)) {
    fputs("unpack.c::sigcheck something went wrong!\n", stderr);
    return false;
  }

  // read dir data end
  uint16_t dir_end_magic;
  FREAD_MACRO(&dir_end_magic, sizeof(uint16_t), 1, arc_ptr)
  if (dir_end_magic != DIR_DATA_END) {
    fputs("invalid dir end magic!\n", stderr);
    return false;
  }

  free(dir_header.name);
  free(dir_header.base);
  return true;
}

bool read_file(FILE *arc_ptr) {

  // read file header
  struct file_metadata file_header;
  if (!read_file_metadata(arc_ptr, &file_header)) {
    fputs("unable to read file header!\n", stderr);
    return false;
  }

  // read magic
  uint16_t file_magic;
  FREAD_MACRO(&file_magic, sizeof(uint16_t), 1, arc_ptr)
  if (file_magic != FILE_DATA_BEGIN) {
    fputs("invalid file magic!\n", stderr);
    return false;
  }
 
  // read file data
  FILE* file_handle = fopen(file_header.base, "wb");
  if (file_handle == NULL) {
    fprintf(stderr, "unable to create file %s\n", file_header.base);
    return false;
  }
  
  uint32_t file_size = file_header.file_size; // in bytes
  uint8_t* data_chunk = (uint8_t*) malloc(CHUNK_SIZE + 2);

  while (file_size > 0) {
    
    unsigned long size = CHUNK_SIZE;
    if (file_size < CHUNK_SIZE)
      size = file_size;

    unsigned long read = fread(data_chunk, sizeof(uint8_t), size, arc_ptr);
    if (fwrite(data_chunk, sizeof(uint8_t), read, file_handle) != read) {
      perror("unable to write to newly created file");
      return false;
    }
   // write(1, data_chunk, read);
    file_size -= read;
  }

  free(data_chunk);
  fclose(file_handle);

  // read file data_end 
  uint16_t file_end_magic;
  FREAD_MACRO(&file_end_magic, sizeof(uint16_t), 1, arc_ptr)
  if (file_end_magic != FILE_DATA_END) {
    fputs("invalid file data end!\n", stderr);
    return false;
  }

  // make the call
  if (!sig_check(arc_ptr)) {
    fprintf(stderr, "sigcheck: recursive call failed");
    return false;
  }

  free(file_header.name);
  free(file_header.base);
  return true;
}
