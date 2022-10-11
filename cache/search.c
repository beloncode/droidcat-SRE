#include <string.h>
#include <fs/checks.h>
#include "context.h"

struct cache_context* NON_NULL_RET cache_collect_entries
        (enum cache_status* restrict status, struct cache_context* cache, const char* restrict cache_location) {
    if (cache_location == NULL) goto invalid;
    if (!fs_is_path(cache_location)) { *status = -CACHE_NOT_A_PATH; goto invalid; }
    if (!fs_can_read(cache_location)) { *status = -CACHE_CANT_READ; goto invalid; }

    invalid:
    memcpy(cache, &invalid_cache, sizeof(invalid_cache));
    return &invalid_cache;
}
