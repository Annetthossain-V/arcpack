#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "header.h"
#include "pack.h"
#include "globl.h"

bool extract_archive(char* file);

bool sig_check(FILE* arc_ptr);

bool read_file(FILE* arc_ptr);
bool read_dir(FILE* arc_ptr);
