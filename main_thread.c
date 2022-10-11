#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include <cache/context.h>
#include <fs/entries.h>

static const char* fs_get_execdir() {
    return NULL;
}

static struct cache_context* main_cache_present(struct cache_context* restrict main_cache, void* cache_location) {
    /* Continues like cache already exist (values obtained from the cache isn't reliable), because the user maybe
     * was modified the original content where's the cache has produce from, but it's a good tip for performance
     * purposes!
    */
    (void)cache_location;
    return main_cache;
}
static struct cache_context* main_cache_create(void* cache_location) {
    (void)cache_location;
    return NULL;
}

struct droidcat_ctx {
    #define _DEFAULT_MAX_SEARCH_PATH_ 4
    #define _SEARCH_PATH_SZ_ 0x45
    char default_search_path[_DEFAULT_MAX_SEARCH_PATH_][_SEARCH_PATH_SZ_];
    const char* runtime_dir;
    struct cache_context* cache_ctx;
};

enum droidcat_status { DROIDCAT_IS_OK = 00, DROIDCAT_EXEC_DIR_NOT_FOUND, DROIDCAT_FILE_TREE_EXIST };

static boolean_t droidcat_checks_filetree(enum droidcat_status* status, const char* restrict filetree_path) {
    #define _RUNTIME_FILETREE_PLUGINS_ "plugins"
    #define _RUNTIME_FILETREE_MODULES_ "modules"
    #define _RUNTIME_FILETREE_CONF_FILE_ "settings.toml"

    static struct fs_entry runtime_filetree[4] = {
            [0] = { .entry_type = ENTRY_DIR },
            [1] = { .entry_type = ENTRY_DIR },
            [2] = { .entry_type = ENTRY_FILE }
    };
    #if defined(_WIN64)
    strncpy_s(runtime_filetree[0].entry_name, _RUNTIME_FILETREE_PLUGINS_, sizeof runtime_filetree[0].entry_name);
    strncpy_s(runtime_filetree[1].entry_name, _RUNTIME_FILETREE_MODULES_, sizeof runtime_filetree[1].entry_name);
    strncpy_s(runtime_filetree[2].entry_name, _RUNTIME_FILETREE_CONF_FILE_, sizeof runtime_filetree[2].entry_name);
    #else
    strncpy(runtime_filetree[0].entry_name, _RUNTIME_FILETREE_PLUGINS_, sizeof runtime_filetree[0].entry_name);
    strncpy(runtime_filetree[1].entry_name, _RUNTIME_FILETREE_MODULES_, sizeof runtime_filetree[1].entry_name);
    strncpy(runtime_filetree[2].entry_name, _RUNTIME_FILETREE_CONF_FILE_, sizeof runtime_filetree[2].entry_name);
    #endif

    if (fs_checks_entry(filetree_path, runtime_filetree))
        *status = -DROIDCAT_FILE_TREE_EXIST;
    return *status == -DROIDCAT_FILE_TREE_EXIST;
}

enum search_path_type { SEARCH_PATH_EXEC_DIR };

static boolean_t droidcat_ctx_init(enum droidcat_status* status, struct droidcat_ctx* restrict droidcat_context) {
    #if defined(_WIN64)
    #elif defined(__unix__)
    static const char* default_dirs[] = {
            ".", "/etc/droidcat", NULL
    };
    #endif

    char* exec_path = droidcat_context->default_search_path[SEARCH_PATH_EXEC_DIR];
    #define _EXEC_CURRENT_DIR_ 0x30
    char exec_current_dir[_EXEC_CURRENT_DIR_];
    if (fs_get_execdir(exec_current_dir, default_dirs[0])) {
    #if defined(_WIN64)
        strncpy_s(exec_path, exec_current_dir, _SEARCH_PATH_SZ_);
    #else
        strncpy(exec_path, exec_current_dir, _SEARCH_PATH_SZ_);
    #endif
        if (droidcat_checks_filetree(status, exec_path)) {
            if (*status == -DROIDCAT_FILE_TREE_EXIST) {
                droidcat_context->runtime_dir = exec_path;
            }
        }
    } else {
        /* event_info(NULL, "Can't get the current executable running directory!\n"); */
    }

    enum cache_status cache_s;
    droidcat_context->cache_ctx = calloc(1, sizeof (struct cache_context));
    char cache_file_location[0x40];

    #define _MAIN_CACHE_COLLECTION_PATH_ "droidcat.cache"
    snprintf(cache_file_location, sizeof cache_file_location, "%s/%s", droidcat_context->runtime_dir,
             _MAIN_CACHE_COLLECTION_PATH_);

    cache_collect_entries(&cache_s, droidcat_context->cache_ctx, cache_file_location);
    cache_test(droidcat_context->cache_ctx, main_cache_present, main_cache_create,
               (void*)cache_file_location);

    return *status == DROIDCAT_IS_OK;
}
static boolean_t droidcat_ctx_deinit(enum droidcat_status* status, struct droidcat_ctx* restrict droidcat_context) {
    if (droidcat_context->cache_ctx != NULL)
        free((void*)droidcat_context->cache_ctx);
    *status = -DROIDCAT_IS_OK;
    return true;
}

int main() {
    struct droidcat_ctx* main_context = calloc(1, sizeof (struct droidcat_ctx));
    enum droidcat_status main_status;
    droidcat_ctx_init(&main_status, main_context);

    droidcat_ctx_deinit(&main_status, main_context);
    free((void*)main_context);

    return -EXEC_SUCCESS;
}
