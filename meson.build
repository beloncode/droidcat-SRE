project('droidcat', ['c'], version: '000a0')
fs_src = files('fs/entries.c')
cache_src = files('cache/region.c', 'cache/search.c', 'cache/test.c')
root_src = files('main_thread.c')
executable(meson.project_name(), sources: [fs_src, cache_src, root_src], c_args: ['-std=c11'])

