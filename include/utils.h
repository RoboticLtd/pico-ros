#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdint.h>

#define PRINT printf
#define SPRINT sprintf
#define SSCAN sscanf

int32_t utils_get_int_from_bytes(const char* bytes, int nbytes);
int64_t utils_get_long_from_bytes(const char* bytes, int nbytes);

int utils_get_pid();

#endif
