#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

struct dir_metadata {
  uint8_t name_len;
  uint16_t path_len;
  char* name;
  char* base;
} __attribute__((packed));

bool read_file_header(FILE* arc_ptr);

bool read_file_metadata();
bool read_dir_metadata(FILE* arc_ptr, struct dir_metadata* info);
