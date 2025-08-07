#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

bool read_file_header(FILE* arc_ptr);

bool read_file_metadata();
bool read_dir_metadata();
