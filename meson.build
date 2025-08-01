project('textquill', 'cpp',
  version : '0.1.0',
  default_options : ['warning_level=3', 'cpp_std=c++23'])

tesseract_dep = dependency('tesseract', required : true)
leptonica_dep = dependency('lept', required: true)

cpptoml_proj = subproject('cpptoml')
cpptoml_dep = cpptoml_proj.get_variable('cpptoml_dep')

cxxopts_proj = subproject('cxxopts')
cxxopts_dep = cxxopts_proj.get_variable('cxxopts_dep')

executable('textquill', 'src/main.cpp',
  dependencies : [tesseract_dep, cpptoml_dep, cxxopts_dep, leptonica_dep],
  install : true)