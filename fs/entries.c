#include "entries.h"
#include "checks.h"

/* Foreach every entry node */
#define FOR_EACH_ENTRY(entries, entry_node) \
    for (const struct fs_entry* entry_node = entry; *entry->entry_name != '\0'; entry++)

/* Checks and verify if all entries already exist, if all entries exist, NULL is returned, otherwise the entry node that
 * is missing is returned
*/
const struct fs_entry* fs_checks_entry(const char* base_directory, const struct fs_entry* restrict entries) {
    #define _PATH_FS_ENTRY_STR_SZ_ 0x50
    char path_entry[_PATH_FS_ENTRY_STR_SZ_];
    FOR_EACH_ENTRY(entries, entry) {
        if (entry->entry_type == ENTRY_DIR || entry->entry_type == ENTRY_FILE)
            if (!fs_can_read(entry->entry_name))
                return entry;
    }
    return NULL;
}
