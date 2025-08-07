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
