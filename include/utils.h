#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#define PRINT printf
#define SPRINT sprintf
#define SSCAN sscanf

int utils_get_int_from_bytes(const char* bytes, int nbytes);
long utils_get_long_from_bytes(const char* bytes, int nbytes);

int utils_get_pid();

#endif
