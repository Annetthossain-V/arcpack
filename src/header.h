#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#define MAGIC 124
#define VERSION 1

// using uint16_t
#define FILE_DATA_END   60890
#define FILE_DATA_BEGIN 60891
#define DIR_DATA_END    60990
#define DIR_DATA_BEGIN  60981
#define DIR_BEGIN       59881
#define FILE_BEGIN      59882

bool write_file_header(FILE* file_ptr);

bool write_file_metadata(FILE* arc_ptr, char* name, char* base, uint32_t size);
bool write_file_end(FILE* arc_ptr);

bool write_dir_metadata(FILE* arc_ptr, char* name, char* base);
bool write_dir_end(FILE* arc_ptr);
