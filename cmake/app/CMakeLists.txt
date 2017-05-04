if( NOT BEAR_ROOT_DIRECTORY )
  message( FATAL_ERROR "BEAR_ROOT_DIRECTORY is not set" )
endif()

set( CMAKE_MODULE_PATH "${BEAR_ROOT_DIRECTORY}/cmake-helper" )

include( bear-config )

find_package( libclaw 1.7.4 )

add_subdirectory( "assets" )
add_subdirectory( "lib" )
add_subdirectory( "launcher" )
