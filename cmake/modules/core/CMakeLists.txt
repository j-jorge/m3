project( libm3_core )

set( core_module_root "${modules_root}/core" )

add_library(
  m3_core STATIC 

  "${core_module_root}/src/m3/gem_ring.cpp"
  "${core_module_root}/src/m3/find_adjacent_ring_gems.cpp"
  )

target_include_directories(
  m3_core
  PUBLIC
  "${core_module_root}/include"
  )

target_link_libraries(
  m3_core
  PUBLIC
  m3_math
  m3_stl
  )

install(
  TARGETS m3_core
  DESTINATION ${M3_INSTALL_LIBRARY_DIR}
  )

add_subdirectory( tests )

