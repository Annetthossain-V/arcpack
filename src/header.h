#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#define MAGIC 124
#define VERSION 1

#define FILE_DATA_END
#define FILE_METADATA

struct file_medata {
  char file_name[124];
  char path[512];
  uint8_t name_len;
  uint16_t path_len;
  uint32_t file_size;
} __attribute__((packed));

bool write_file_header(FILE* file_ptr);

bool write_data_header();
bool write_data_end();
