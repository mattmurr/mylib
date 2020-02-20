project('mylib', 'c', version : '0.1.0', license : 'zlib',
  default_options : ['c_std=c11'])

cc = meson.get_compiler('c')

mylib_inc = include_directories('include')

# dependencies

# We currently have no dependencies, it's possible we may have some later.

mylib_src = []
# All the source files are in src directory
subdir('src')

install_subdir('mylib', install_dir : 'include')

mylib_lib = library('mylib', mylib_src, install : true,
  #dependencies : mylib_deps,
  include_directories : mylib_inc,
  version : meson.project_version())

mylib_dep = declare_dependency(link_with : mylib_lib,
  #dependencies : mylib_deps,
  include_directories : mylib_inc,
  version : meson.project_version())

pkg = import('pkgconfig')
pkg.generate(name : 'mylib', requires : ['mylib_lib'],
  description : 'My C library providing various data structures and utilities.',
  version : meson.project_version())

if get_option('enable-tests') == true
  subdir('tests')
endif
