#ifndef HASH_H
#define HASH_H

#include <cstring>
#include <cstddef>

static size_t hashCode(char c) {
    return (size_t)c;
}

static size_t hashCode(unsigned char c) {
    return (size_t)c;
}

static size_t hashCode(int k) {
    return (size_t)k;
}

static size_t hashCode(long long k) {
    return (size_t)k;
}

static size_t hashCode(const char* s) {
    size_t h = 0;
    for (size_t i = 0; s[i]; i++)
        h = (h << 5) ^ s[i] ^ (h >> 2);
    return h & 0x7FFFFFFF;
}

#endif
