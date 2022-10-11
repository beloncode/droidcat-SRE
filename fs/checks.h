#pragma once

#include <string.h>
#include <generic/types.h>

#if defined(_WIN64)
#include <windows.h>
#include <assert.h>
#elif defined(__unix__)
#include <unistd.h>
#endif

static inline boolean_t fs_is_path(const char* restrict prob_path) {
    return strpbrk(prob_path, "!@#$%&^~") == NULL;
}

static inline boolean_t fs_is(const char* restrict path_is) {
    #if defined(_WIN64)
    assert(strlen(path_is) < MAX_PATH);
    DWORD W_file_attr = GetFileAttributesA((LPCSTR)path_is);
    return W_file_attr != INVALID_FILE_ATTRIBUTES && !(W_file_attr & FILE_ATTRIBUTE_DIRECTORY);
    #elif defined(__unix__)
    return access(path_is, F_OK);
    #endif
}
static inline boolean_t fs_can_read(const char* restrict path_test) {
#if defined(_WIN64)
    return fs_is(path_test);
#elif defined(__unix__)
    return access(path_test, F_OK | R_OK);
#endif
}
