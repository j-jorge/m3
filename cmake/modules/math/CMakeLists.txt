project( libm3_math )

set( math_module_root "${modules_root}/math" )

add_library(
  m3_math STATIC 

  "${math_module_root}/src/m3/math/circle_section_index.cpp"
  "${math_module_root}/src/m3/math/linear_range_interpolation.cpp"
  )

target_include_directories(
  m3_math
  PUBLIC
  "${math_module_root}/include"
  )

install(
  TARGETS m3_math
  DESTINATION ${M3_INSTALL_LIBRARY_DIR}
  )

add_subdirectory( tests )
