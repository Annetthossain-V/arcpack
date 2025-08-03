#include "pack.h"
#include "header.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>
#include <dirent.h>

char* base = NULL;

bool create_archive(char *file) {
  bool stat = false;
  char* file_archive = (char*) malloc(strlen(file) + 5);

  // append .arc to file name
  strcpy(file_archive, file);
  strcat(file_archive, ".arc");

  FILE* file_ptr = fopen(file_archive, "wb"); // create the file
  if (file_ptr == NULL)
    return false;

  stat = write_file_header(file_ptr); // write header
  if (!stat)
    return false;

  stat = directory_handle(file, file_archive, file_ptr);
  if (!stat)
    return false;

  free(file_archive);
  fclose(file_ptr);
  return true;
}

bool directory_handle(char* file, char* file_archive, FILE* file_ptr) {
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

  closedir(dir);
  return true;
}

bool file_handler(FILE *arc_ptr, char *file) {

  return true;
}

bool dir_handler(FILE *arc_ptr, char *dir) {


  return true;
}
