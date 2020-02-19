vector_exe = executable('vector', 'vector.c',
  dependencies : mylib_dep)

bitset_exe = executable('bitset', 'bitset.c',
  dependencies : mylib_dep)

linked_list_exe = executable('linked_list', 'linked_list.c',
  dependencies : mylib_dep)

hash_map_exe = executable('hash_map', 'hash_map.c',
  dependencies : mylib_dep)

test('vector', vector_exe, suite : 'vector')

test('bitset', bitset_exe, suite : 'bitset')

test('linked list', linked_list_exe, suite : 'linked list')

test('hash map', hash_map_exe, suite : 'hash map')
