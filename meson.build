project('cgtoml', ['c', 'cpp'],
  version : '1.0.0',
  license : 'MIT',
  meson_version : '>= 0.51.0',
  default_options : [
    'buildtype=debugoptimized',
    'cpp_std=c++11',
  ]
)

cgtoml_api_version = '1.0'
cgtoml_lib_include_dir = include_directories('lib')

cmake = import('cmake')
cpptoml = cmake.subproject('cpptoml')
cpptoml_dep = cpptoml.dependency('cpptoml')

gobject_dep = dependency('gobject-2.0', version : '>= 2.58')

subdir('lib')
if get_option('test')
  subdir('test')
endif
