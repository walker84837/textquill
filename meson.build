project('textquill', 'cpp',
  version : '0.1.0',
  default_options : ['warning_level=3', 'cpp_std=c++23'])

cmake = import('cmake')

leptonica_proj = cmake.subproject('leptonica')
leptonica_dep = leptonica_proj.dependency('leptonica')

tesseract_proj = cmake.subproject('tesseract',
  cmake_options: [
    '-DBUILD_TRAINING_TOOLS=OFF',
    '-DDISABLED_LEGACY_ENGINE=ON'
  ]
)
tesseract_dep = tesseract_proj.dependency('libtesseract')

cpptoml_proj = subproject('cpptoml')
cpptoml_dep = cpptoml_proj.get_variable('cpptoml_dep')

cxxopts_proj = subproject('cxxopts')
cxxopts_dep = cxxopts_proj.get_variable('cxxopts_dep')

executable('textquill', 'src/main.cpp',
  dependencies: [tesseract_dep, cpptoml_dep, cxxopts_dep, leptonica_dep],
  install: true)