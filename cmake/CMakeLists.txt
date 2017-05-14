cmake_minimum_required( VERSION 3.0 )

enable_testing()

set( CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-helper" )

include( declare-test )
include( prepend-string )

set( source_root "${CMAKE_CURRENT_SOURCE_DIR}/.." )
set( CMAKE_CXX_STANDARD 11 )

if( NOT M3_INSTALL_LIBRARY_DIR )
  set( M3_INSTALL_LIBRARY_DIR lib )
endif()

add_subdirectory( "modules" )
add_subdirectory( "app" )
