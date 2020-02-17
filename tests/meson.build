inc = include_directories('../src')

vector_exe = executable('vector', 'vector.c',
  link_with : mylib_lib,
  include_directories : inc)

bitset_exe = executable('bitset', 'bitset.c',
  link_with : mylib_lib,
  include_directories : inc)

linked_list_exe = executable('linked_list', 'linked_list.c',
  link_with : mylib_lib,
  include_directories : inc)

hash_map_exe = executable('hash_map', 'hash_map.c',
  link_with : mylib_lib,
  include_directories : inc)

test('vector', vector_exe, suite : 'vector')

test('bitset', bitset_exe, suite : 'bitset')

test('linked list', linked_list_exe, suite : 'linked list')

test('hash map', hash_map_exe, suite : 'hash map')
