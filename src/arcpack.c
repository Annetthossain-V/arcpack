#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#if defined(_WIN64) || defined(_WIN32) || defined(WIN32) || defined(WIN64)
  #error "Target OS not supported!"
#endif

#if defined(MACH) || defined(_APPLE_) || defined(MACHOS) || defined(__APPLE__)
  #warning "Target OS not supported!"
#endif

#if defined(linux) || defined(_linux_) || defined(__linux__)
  #warning "Target OS not tested for! Also BSD better."
#endif 

#if defined(_cplusplus)
  #error "Incompatible Compiler!"
#endif

#include "header.h"
#include "globl.h"
#include "pack.h"
#include "unpack.h"

void arg_collector(uint8_t* mode, char** file, int argc, char** argv) {

  if (argc < 2)
    exit(1);

  if (strcmp(argv[1], "--version") == 0 || strcmp(argv[1], "-v") == 0) {
    printf("ArcPack Version %d\n", VERSION);
    exit(0);
  }

  if (argc != 3) {
    fputs("Error! Argument format mismatch!\n", stderr);
    fputs("arcpack: <option> <file/dir>\n", stderr);
    fputs("option: \n", stderr);
    fputs(" -c to compress\n", stderr);
    fputs(" -z to uncompress\n", stderr);
    exit(1);
  }

  if (strcmp(argv[1], "-c") == 0)
    *mode = MODE_PACK;
  else if (strcmp(argv[1], "-z") == 0)
    *mode = MODE_UPACK;
  else if (strcmp(argv[1], "-d") == 0)
    *mode = MODE_UPACK;
  else 
    { EPQ("Unrecognised Option\n"); }

  *file = (char*) malloc(strlen(argv[2]) + 2);
  strcpy(*file, argv[2]);

  return;
}

int main(int argc, char** argv) {
  uint8_t mode = 0;
  char* file;
  
  arg_collector(&mode, &file, argc, argv);

  if (strcmp(file, "") == 0 || strcmp(file, " ") == 0)
    return 1;

  bool stat = false;
  switch (mode) {
    case MODE_PACK:
      stat = create_archive(file);
      break;
    case MODE_UPACK:
      stat = extract_archive(file);
      break;
  }

  if (!stat) {
    fputs("Error reading/creating archive\n", stderr);
    return 1;
  }

  free(file);
  return 0;
}

int which_endian() {
  volatile uint32_t i = 0x01234567;
  return (*((uint8_t*)(&i))) == 0x67;
}



