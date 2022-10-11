#include "context.h"

struct cache_context* cache_test(struct cache_context* cache_context, cache_exist_cb call_if_exist,
                                 cache_none_cb call_if_not_exist, void* user_data) {
    if (cache_exist(cache_context))
        return call_if_exist(cache_context, user_data);
    return call_if_not_exist(user_data);
}

