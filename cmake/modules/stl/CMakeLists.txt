project( libm3_stl )

set( stl_module_root "${modules_root}/stl" )

add_library(
  m3_stl STATIC 

  "${stl_module_root}/src/m3/stl/vector.cpp"
  )

install(
  TARGETS m3_stl
  DESTINATION ${M3_INSTALL_LIBRARY_DIR}
  )

target_include_directories(
  m3_stl
  PUBLIC
  "${stl_module_root}/include"
  )
