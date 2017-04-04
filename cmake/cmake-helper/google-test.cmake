if ( CMAKE_VERSION VERSION_LESS 3.2 )
  set( UPDATE_DISCONNECTED_IF_AVAILABLE "" )
else()
  set( UPDATE_DISCONNECTED_IF_AVAILABLE "UPDATE_DISCONNECTED 1" )
endif()

include( DownloadProject )

download_project(
  PROJ google-test
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG master
  ${UPDATE_DISCONNECTED_IF_AVAILABLE}
  )

add_subdirectory( ${google-test_SOURCE_DIR} ${google-test_BINARY_DIR} )
