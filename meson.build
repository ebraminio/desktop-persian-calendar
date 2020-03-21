project('desktop-persian-calendar', 'c', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3'])

cpp = meson.get_compiler('cpp')

add_project_arguments(cpp.get_supported_arguments([
  '-fno-rtti',
  '-fno-exceptions',
  '-fno-threadsafe-statics',
]), language : 'cpp')

appindicator_dep = dependency('appindicator3-0.1', required: true)
icu_dep = dependency('icu-i18n', required: true)
cairo_dep = dependency('cairo', required: true)

exe = executable('desktop_persian_calendar', 'source.cc',
  dependencies : [appindicator_dep, icu_dep, cairo_dep],
  install : true)

test('basic', exe)
