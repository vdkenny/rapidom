project(
  'rapidom',
  'c',
  meson_version: '>= 1.3.0',
  version: '0.0',
  default_options: [
    'c_std=gnu23',
    'warning_level=3',
    'buildtype=release',
    'optimization=3',
    'strip=true',
  ],
)

dependencies = []

sources = ['src/main.c']

executable(
  'rapidom',
  sources,
  dependencies: dependencies,
)
