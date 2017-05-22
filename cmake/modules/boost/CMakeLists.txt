project( libm3_boost )

set( boost_module_root "${modules_root}/boost" )

add_library(
  m3_boost STATIC 

  "${boost_module_root}/src/m3/boost/signals.cpp"
  )

install(
  TARGETS m3_boost
  DESTINATION ${M3_INSTALL_LIBRARY_DIR}
  )

find_package( Boost 1.60 )

target_include_directories(
  m3_boost
  PUBLIC
  "${boost_module_root}/include"
  ${BOOST_INCLUDE_DIR}
  )

target_link_libraries(
  m3_boost
  PUBLIC
  m3_stl
  )
