#include "utils.h"

int32_t string_to_int(const char* str) {
    if (!str) return -1;

    int32_t port = strtoul(str, NULL, 0);

    return errno == ERANGE ? -1 : port;
}