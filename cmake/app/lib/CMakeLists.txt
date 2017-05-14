project( libm3_items )

set( M3_ITEMS_LIBRARY_NAME m3_items )

find_package( RapidJSON )

prepend_string(
  M3_ITEMS_FILES
  "${source_root}/app/lib/src/m3/app/"
  camera.cpp
  create_level.cpp
  gem_ring.cpp
  input_controller.cpp

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
  "${BEAR_ITEMS_INCLUDE_DIRECTORY}"
  "${CLAW_INCLUDE_DIRECTORY}"
  )

link_directories(
  ${M3_ITEMS_LIBRARY_NAME}
  PUBLIC
  ${BEAR_ENGINE_LINK_DIRECTORY}
)

target_link_libraries(
  ${M3_ITEMS_LIBRARY_NAME}
  PUBLIC
  m3_core
  bear_generic_items
  bear_input
  bear_text_interface
  bear_universe
  bear_visual
  ${CLAW_TWEEN_LIBRARIES}
  )

install(
  TARGETS ${M3_ITEMS_LIBRARY_NAME}
  DESTINATION ${M3_INSTALL_LIBRARY_DIR}
  )
