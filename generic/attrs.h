#pragma once

#if defined(__unix__)
#define NON_NULL_RET __attribute__((__returns_nonnull__))
#else
#define NON_NULL_RET
#endif
