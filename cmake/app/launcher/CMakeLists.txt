project( m3-launcher )

set( M3_LAUNCHER_TARGET_NAME m3 )

prepend_string(
  M3_LAUNCHER_FILES
  "${source_root}/app/launcher/src/m3/app/"
  launcher.cpp
  main.cpp
  )

add_executable(
  ${M3_LAUNCHER_TARGET_NAME}
  ${M3_LAUNCHER_FILES}
  )

target_include_directories(
  ${M3_LAUNCHER_TARGET_NAME}
  PRIVATE
  "${source_root}/app/launcher/include"
  ${BEAR_ENGINE_INCLUDE_DIRECTORY}
  )

link_directories(
  ${M3_LAUNCHER_TARGET_NAME}
  PRIVATE
  ${BEAR_ENGINE_LINK_DIRECTORY}
)

target_link_libraries(
  ${M3_LAUNCHER_TARGET_NAME}

  m3_items
  bear_engine
  ${CLAW_APPLICATION_LIBRARIES}
  )

set_target_properties(
  ${M3_LAUNCHER_TARGET_NAME}
  PROPERTIES
  INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${M3_INSTALL_CUSTOM_LIBRARY_DIR}"
  )

if( NOT CMAKE_BUILD_TYPE STREQUAL "release" )
  set(
    M3_COMPILE_DEFINITIONS
    "M3_DATA_DIRECTORY=${M3_DATA_DIRECTORY}"
    )
else()
  set(
    M3_COMPILE_DEFINITIONS
    "${M3_COMPILE_DEFINITIONS}"
    "M3_DATA_DIRECTORY=${CMAKE_INSTALL_PREFIX}/${M3_INSTALL_DATA_DIR}"
    )
endif()

set_target_properties(
  ${M3_LAUNCHER_TARGET_NAME}
  PROPERTIES
  COMPILE_DEFINITIONS
  "${M3_COMPILE_DEFINITIONS}"
  )

if( NOT M3_INSTALL_EXECUTABLE_DIR )
  set( M3_INSTALL_EXECUTABLE_DIR bin )
endif()

install(
  TARGETS ${M3_LAUNCHER_TARGET_NAME}
  DESTINATION ${M3_INSTALL_EXECUTABLE_DIR}
  )
