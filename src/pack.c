#include "pack.h"
#include "header.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/dirent.h>
#include <sys/stat.h>
#include <dirent.h>

// 1kb
#define CHUNK_SIZE 512 

char* base = NULL;

bool create_archive(char *file) {
  bool stat = false;
  char* file_archive = (char*) malloc(strlen(file) + 5);
  base = (char*) malloc(strlen(file) + 2);

  // append .arc to file name
  strcpy(file_archive, file);
  strcat(file_archive, ".arc");

  // set base
  strcpy(base, file);
  strcat(base, "/");

  FILE* file_ptr = fopen(file_archive, "wb"); // create the file
  if (file_ptr == NULL)
    return false;

  stat = write_file_header(file_ptr); // write header
  if (!stat)
    return false;

  // write dir name
  // uint8_t len = (uint8_t) strlen(file);
  // fwrite(&len, sizeof(uint8_t), 1, file_ptr);
  // fwrite(file, sizeof(char), strlen(file), file_ptr);

  stat = directory_handle(file, file_ptr);
  if (!stat)
    return false;

  free(file_archive);
  free(base);
  fclose(file_ptr);
  return true;
}

bool directory_handle(char* file, FILE* file_ptr) {
  struct stat statbuf;
  int status = stat(file, &statbuf);
  if (status != 0) {
    perror("Error getting file status");
    return false;
  }

  if (!S_ISDIR(statbuf.st_mode)) {
    fputs("input argument not a directory!\n", stderr);
    return false;
  }

  DIR* dir;
  dir = opendir(file);

  struct dirent* dir_entry;
  while((dir_entry = readdir(dir)) != NULL) {
    if (strcmp(dir_entry->d_name, ".") == 0 || strcmp(dir_entry->d_name, "..") == 0)
      continue;
   
    bool status = false;

    switch (dir_entry->d_type) {
      case DT_REG:
        status = file_handler(file_ptr, dir_entry->d_name);
        break;
      case DT_DIR:
        status = dir_handler(file_ptr, dir_entry->d_name);
        break;
      case DT_LNK:
        fprintf(stderr, "symbolic links are not suppoerted yet! ignored: %s\n", dir_entry->d_name);
        continue;
        break;
      case DT_UNKNOWN:
        fputs("Error getting file type", stderr);
        return false;
        break; // just for the love of the game
    }

    if (!status)
      return false;
  }

  closedir(dir);
  return true;
}

bool file_handler(FILE *arc_ptr, char *file) { 
  // set new base
  char* new_base = (char*) malloc(strlen(file) + strlen(base));
  strcpy(new_base, base);
  strcat(new_base, file);

  printf("a %s\n", new_base);
  
  // get the size
  long size = fsize(new_base);
  if (size < 0)
    return false;

  if (size > UINT32_MAX) {
    fputs("file size larger than u32\n", stderr);
    return false;
  }

  // write header 
  if(!write_file_metadata(arc_ptr, file, new_base, (uint32_t)size))
    return false;

  // read/write content chunk by chunk
  FILE* file_handle = fopen(new_base, "rb");
  long bytes_read = 0;
  uint16_t* data = NULL;
  do {
    // reading u16 meaning 2 bytes at a time
    

  } while (bytes_read < size || bytes_read != size);

  fclose(file_handle);

  // write end
  if (!write_file_end(arc_ptr))
    return false;

  free(new_base);
  return true;
}

bool dir_handler(FILE *arc_ptr, char *dir) {
  // update base
  

  // reset base
  return true;
}

long fsize(char* file) {
  FILE* ptr = fopen(file, "rb");
  if (ptr == NULL) {
    perror("Error opening file");
    return -1;
  }

  fseek(ptr, 0, SEEK_END); // set position to end 
  long size = ftell(ptr); // get current position 

  fclose(ptr);
  return size;
}
