#ifndef s3kOwqfd8K_UTILS_H
#define s3kOwqfd8K_UTILS_H

#include <stdint.h>
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>

/**
 * Function:
 *  string_to_int
 * 
 * Purpose:
 *  Converts a string to a positive integer.
 * 
 * Parameters:
 * - str: 
 *      The string to convert
 * 
 * returns: 
 *  The converted integer on success, -1 otherwise.
 */
int32_t string_to_int(const char* str);

#endif