#pragma once

/* Entries file system, hold information of one or more directory tree(s) */
enum entry_type { ENTRY_DIR = 00, ENTRY_FILE };
struct fs_entry {
    #define _ENTRY_NAME_SZ_ 0x30
    char entry_name[_ENTRY_NAME_SZ_];
    enum entry_type entry_type;
};

const struct fs_entry* fs_checks_entry(const char* base_directory, const struct fs_entry* restrict entries);

