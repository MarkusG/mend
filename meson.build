project(
  'mend',
  'c',
  version: '0.1.0'
)

version = '-DVERSION="@0@"'
version = version.format(meson.project_version())
add_global_arguments(version, language : 'c')

version_conf = configuration_data()

version_conf.set(
  'MEND_VERSION',
  meson.project_version()
)

version_conf.set(
  'MEND_DB_VERSION',
  '0.4.0'
)

deps = [
  dependency('libpq')
]

sources = []
headers = []

subdir('lib')
subdir('src')
subdir('doc')

exe = executable(meson.project_name(),
  sources + headers,
  dependencies: deps,
  link_with: lib,
  install: true
)
