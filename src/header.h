#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#define MAGIC 124
#define VERSION 1

#define FILE_DATA_END   8800
#define FILE_DATA_BEGIN 8910
#define DIR_DATA_END    9900
#define DIR_DATA_BEGIN  9810

struct file_medata {
  uint8_t name_len;
  uint16_t path_len;
  uint32_t file_size;
  // char* name;
  // char* path;
} __attribute__((packed));

bool write_file_header(FILE* file_ptr);

bool write_file_metadata();
bool write_file_end();

bool write_dir_metadata();
bool write_dir_end();
