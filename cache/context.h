#pragma once
#include <string.h>
#include <generic/types.h>
#include <generic/attrs.h>

struct cache_entry {
    uint_fast16_t cache_id;
    uint_fast32_t cache_type;
#define _CACHE_NAME_SZ_ 0x20
    char cache_name[_CACHE_NAME_SZ_];
#define _CACHE_VALUE_SZ_ 0x20
    uint_least8_t cache_value[_CACHE_VALUE_SZ_];
};

struct cache_table {
    uint32_t entries_count;
    uint32_t entries_offset;
    uint32_t entries_size;
    uint32_t entry_size;
};

struct cache_context {
#define _CACHE_CONTEXT_VERSION_ 0x1
    int_fast8_t cache_version[1];
#define _CACHE_PURPOSES_SZ 0x1f
    char cache_purposes[_CACHE_PURPOSES_SZ];
    struct cache_table cache_table_entry[1];
};

enum cache_entry_id { CACHE_INVALID = 00 };
enum cache_status { CACHE_NOT_A_PATH = 0, CACHE_CANT_READ, };

typedef struct cache_context* (cache_exist_cb)(struct cache_context* restrict cache, void* user_data);
typedef struct cache_context* (cache_none_cb)(void* user_data);

extern struct cache_context invalid_cache;

struct cache_context* NON_NULL_RET cache_collect_entries
        (enum cache_status* restrict status, struct cache_context* cache, const char* restrict cache_location);

static inline boolean_t cache_exist(struct cache_context* restrict test_cache) {
    return (boolean_t)(test_cache != &invalid_cache || memcmp(test_cache, &invalid_cache, sizeof invalid_cache) != 0);
}

struct cache_context* cache_test(struct cache_context* cache_context, cache_exist_cb call_if_exist,
                                 cache_none_cb call_if_not_exist, void* user_data);

