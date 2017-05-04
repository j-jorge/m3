project( libm3_items )

set( M3_ITEMS_LIBRARY_NAME m3_items )

find_package( RapidJSON )

prepend_string(
  M3_ITEMS_FILES
  "${source_root}/app/lib/src/m3/app/"
  gem_ring.cpp

  config/get_config.cpp
  )

if( NOT M3_ITEMS_LIBRARY_LINK_TYPE )
  set( M3_ITEMS_LIBRARY_LINK_TYPE STATIC )
endif()

add_library(
  ${M3_ITEMS_LIBRARY_NAME}
  ${M3_ITEMS_LIBRARY_LINK_TYPE}
  ${M3_ITEMS_FILES}
  )

target_include_directories(
  ${M3_ITEMS_LIBRARY_NAME}
  PUBLIC
  "${source_root}/app/lib/include"
  "${BEAR_ENGINE_INCLUDE_DIRECTORY}"
  )

target_link_libraries(
  ${M3_ITEMS_LIBRARY_NAME}
  PUBLIC
  m3_core
  )

set(
  M3_ITEMS_LIBRARY_PATH
  "${CMAKE_CURRENT_BINARY_DIR}/lib${M3_ITEMS_LIBRARY_PATH}.so"
  )
