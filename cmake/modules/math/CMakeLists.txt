project( libm3_math )

set( math_module_root "${modules_root}/math" )

add_library(
  m3_math STATIC 

  "${math_module_root}/src/m3/math/circle_section_index.cpp"
  )

target_include_directories(
  m3_math
  PUBLIC
  "${math_module_root}/include"
  )

add_subdirectory( tests )
