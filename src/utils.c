#include "utils.h"

/**
 * Checks if input is NULL, not an integer or too large.
 * If not, the converted number is returned.
 */
int32_t string_to_int(const char* str) {
    if (!str) return -1;
    int32_t port = (int)strtoul(str, NULL, 0);
    return errno == ERANGE || port == 0 ? -1 : port;
}