cmake_minimum_required( VERSION 3.0 )

enable_testing()

set( CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-helper" )

include( declare-test )

set( source_root "${CMAKE_CURRENT_SOURCE_DIR}/../" )

add_subdirectory( "modules" )
