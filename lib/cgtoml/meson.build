cgtoml_lib_sources = [
  'array.cpp',
  'table.cpp',
  'file.cpp',
]

cgtoml_lib_headers = [
  'cgtoml.h',
  'array.h',
  'table.h',
  'file.h',
]

cgtoml_lib = static_library('cgtoml-' + cgtoml_api_version,
  cgtoml_lib_sources,
  cpp_args : [
    '-D_GNU_SOURCE',
    '-DG_LOG_USE_STRUCTURED',
    '-DG_LOG_DOMAIN="libcgtoml"',
  ],
  install: true,
  include_directories: cgtoml_lib_include_dir,
  dependencies : [gobject_dep, cpptoml_dep],
)

cgtoml_dep = declare_dependency(
  link_with: cgtoml_lib,
  include_directories: cgtoml_lib_include_dir,
  dependencies: [gobject_dep]
)
