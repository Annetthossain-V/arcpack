#pragma once
#include <stdbool.h>
#include <stdio.h>

bool create_archive(char* file);

bool directory_handle(char* file, FILE* file_ptr);

bool file_handler(FILE* arc_ptr, char* file);
bool dir_handler(FILE* arc_ptr, char* dir);

extern char* base;
