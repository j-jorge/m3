project( libm3_table )

set( table_module_root "${modules_root}/table" )

prepend_string(
  m3_table_files
  "${table_module_root}/src/m3/table/"

  "generate_fragmented_indices.cpp"
  )

add_library( m3_table STATIC ${m3_table_files} )

target_include_directories(
  m3_table
  PUBLIC
  "${table_module_root}/include"
  )

target_link_libraries(
  m3_table
  PUBLIC
  m3_stl
  )

install(
  TARGETS m3_table
  DESTINATION ${M3_INSTALL_LIBRARY_DIR}
  )

add_subdirectory( tests )

