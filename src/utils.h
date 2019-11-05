#ifndef s3kOwqfd8K_UTILS_H
#define s3kOwqfd8K_UTILS_H

#include <stdint.h>
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>

/**
 * Attempt to convert a string to an integer. 
 * 
 * Returns the integer if successful, -1 otherwise.
 */
int32_t string_to_int(const char* str);

#endif