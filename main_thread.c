#include <stdint.h>
#include <string.h>

#if defined(_WIN64)
#include <windows.h>
#include <assert.h>
#elif defined(__unix__)
#include <unistd.h>
#endif

typedef int_fast8_t boolean_t;
enum program_exec_status { EXEC_SUCCESS = 00 };

static inline boolean_t fs_is_path(const char* restrict prob_path) {
    return strpbrk(prob_path, "!@#$%&*") == NULL;
}

static inline boolean_t fs_is(const char* restrict path_is) {
    #if defined(_WIN64)
    assert(strlen(path_is) < MAX_PATH);
    DWORD W_file_attr = GetFileAttributesA((LPCSTR)path_is);
    return W_file_attr != INVALID_FILE_ATTRIBUTES && !W_file_attr & FILE_ATTRIBUTE_DIRECTORY;
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
static const char* fs_get_execdir() {
    return NULL;
}
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
static struct cache_context invalid_cache = { .cache_version[0] = -_CACHE_CONTEXT_VERSION_ };

#if defined(__unix__)
#define NON_NULL_RET __attribute__((__returns_nonnull__))
#else
#define NON_NULL_RET
#endif

struct cache_context* NON_NULL_RET cache_collect_entries
        (enum cache_status* restrict status, const char* restrict cache_location) {
    if (cache_location == NULL) goto invalid;
    if (!fs_is_path(cache_location)) { *status = -CACHE_NOT_A_PATH; goto invalid; }
    if (!fs_can_read(cache_location)) { *status = -CACHE_CANT_READ; goto invalid; }

    invalid:
    return &invalid_cache;
}

static inline boolean_t cache_exist(struct cache_context* restrict test_cache) {
    return (boolean_t)(test_cache != &invalid_cache || memcmp(test_cache, &invalid_cache, sizeof invalid_cache) != 0);
}
typedef struct cache_context* (cache_exist_cb)(struct cache_context* restrict cache);
typedef struct cache_context* (cache_none_cb)();

static struct cache_context* main_cache_present(struct cache_context* restrict main_cache) {
    /* Continues like cache already exist (values obtained from the cache isn't reliable), because the user maybe
     * was modified the original content where's the cache has produce from, but it's a good tip for performance
     * purposes!
    */
    return main_cache;
}
static struct cache_context* main_cache_create() {
    return NULL;
}

static struct cache_context* cache_test(struct cache_context* cache_context, cache_exist_cb call_if_exist,
                                        cache_none_cb call_if_not_exist) {
    if (cache_exist(cache_context)) return call_if_exist(cache_context);
    return call_if_not_exist();
}

struct droidcat_ctx {
    #define _DEFAULT_MAX_SEARCH_PATH_ 4
    #define _SEARCH_PATH_SZ_ 0x45
    char default_search_path[_DEFAULT_MAX_SEARCH_PATH_][_SEARCH_PATH_SZ_];
};
enum search_path_type { SEARCH_PATH_EXEC_DIR };
enum droidcat_status { DROIDCAT_IS_OK = 00, DROIDCAT_EXEC_DIR_NOT_FOUND, DROIDCAT_FILE_TREE_EXIST };

static boolean_t droidcat_checks_filetree(enum droidcat_status* status, const char* restrict filetree_path) {
    *status = -DROIDCAT_FILE_TREE_EXIST;
    return *status == -DROIDCAT_FILE_TREE_EXIST;
}

static boolean_t droidcat_ctx_init(enum droidcat_status* status, struct droidcat_ctx* restrict droidcat_context) {
    #if 0
    #elif 0
    #else
    const char* exec_current_dir = fs_get_execdir();
    char* exec_path = droidcat_context->default_search_path[SEARCH_PATH_EXEC_DIR];
    if (exec_current_dir == NULL) {}
    else {
        if (strncmp(exec_path, exec_current_dir, SEARCH_PATH_EXEC_DIR)) { *status = -DROIDCAT_EXEC_DIR_NOT_FOUND; }
        else if (droidcat_checks_filetree(status, exec_path)) {}
    }
    #endif
    *status = DROIDCAT_IS_OK;
    return *status == DROIDCAT_IS_OK;
}
static boolean_t droidcat_ctx_deinit(enum droidcat_status* status, struct droidcat_ctx* restrict droidcat_context) {
    return 1;
}
int main() {
    struct droidcat_ctx main_context;
    enum droidcat_status main_status;
    droidcat_ctx_init(&main_status, &main_context);

    #define _MAIN_CACHE_COLLECTION_PATH_ "droidcat.cache"
    enum cache_status main_cache_status;
    struct cache_context* main_cache = cache_collect_entries(&main_cache_status, _MAIN_CACHE_COLLECTION_PATH_);
    main_cache = cache_test(main_cache, main_cache_present, main_cache_create);

    droidcat_ctx_deinit(&main_status, &main_context);

    return -EXEC_SUCCESS;
}
