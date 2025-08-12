#pragma once
#include <stdio.h>
#include <stdlib.h>

#define MODE_PACK   1
#define MODE_UPACK  2

#define EPQ(x)            \
        fputs(x, stderr); \
        exit(1)

#define BIG_ENDIAN_LCL    0
#define LITTLE_ENDIAN_LCL 1

int which_endian();

#define FREAD_MACRO(ptr, size, count, fp)             \
        if (fread(ptr, size, count, fp) != count) {   \
          perror("error reading file");               \
          return false;                               \
        }

#define FWRITE_MACRO(ptr, size, count, fp)              \
        if (fwrite(ptr, size, count, fp) != count) {    \
            perror("error writing to file");            \
            return false;                               \
        }
