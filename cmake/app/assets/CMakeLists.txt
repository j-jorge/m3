project( m3-assets )

set( M3_DATA_DIRECTORY "${source_root}/app/data" )
set( M3_DATA_DIRECTORY "${M3_DATA_DIRECTORY}" PARENT_SCOPE )
set( M3_ASSETS_DIRECTORY "${source_root}/app/assets" )

function( sprite_sheet BASENAME )
  
  cmake_parse_arguments(
    SPRITE_SHEET_ARGS
    ""
    "WIDTH;HEIGHT"
    "OUTPUTS;FILES"
    ${ARGN}
    )

  if( NOT SPRITE_SHEET_ARGS_OUTPUTS )
    set( SPRITE_SHEET_ARGS_OUTPUTS "${BASENAME}.png" )
  endif()

  if( NOT SPRITE_SHEET_ARGS_FILES )
    message( FATAL_ERROR "You must provide a file to pack" )
  endif()

  if( NOT SPRITE_SHEET_ARGS_WIDTH )
    set( SPRITE_SHEET_ARGS_WIDTH 1024 )
  endif()

  if( NOT SPRITE_SHEET_ARGS_HEIGHT )
    set( SPRITE_SHEET_ARGS_HEIGHT 1024 )
  endif()

  set( SPRITEPOS ${BASENAME}.spritepos )
  get_filename_component( DIRECTORY ${BASENAME} DIRECTORY )
  
  add_custom_command(
    OUTPUT ${SPRITE_SHEET_ARGS_OUTPUTS} "${SPRITEPOS}"
    COMMAND ${CMAKE_COMMAND} -E make_directory ${DIRECTORY}
    COMMAND auto-pack-my-sprites --no-rotation --output="${BASENAME}"
    --size=${SPRITE_SHEET_ARGS_WIDTH}x${SPRITE_SHEET_ARGS_HEIGHT}
    --spritepos ${SPRITE_SHEET_ARGS_FILES}
    --strip-paths --strip-extensions
    DEPENDS ${SPRITE_SHEET_ARGS_FILES}
    WORKING_DIRECTORY ${M3_DATA_DIRECTORY}
  )

  add_custom_target( assets ALL DEPENDS ${SPRITEPOS} )
endfunction()

prepend_string(
  M3_GEM_FILES
  "${M3_ASSETS_DIRECTORY}/gfx/"
  gem-1.png
  gem-2.png
  gem-3.png
  gem-4.png
  gem-5.png
  gem-placeholder.png
  )

sprite_sheet( gfx/sprites FILES ${M3_GEM_FILES} )

if ( NOT M3_INSTALL_DATA_DIR )
  set( M3_INSTALL_DATA_DIR "share/m3" )
endif()

install(
  DIRECTORY "${M3_DATA_DIRECTORY}/gfx" "${M3_DATA_DIRECTORY}/config"
  DESTINATION ${M3_INSTALL_DATA_DIR}
  FILE_PERMISSIONS OWNER_READ OWNER_WRITE
  GROUP_READ
  WORLD_READ
  DIRECTORY_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
  GROUP_READ GROUP_EXECUTE
  WORLD_READ WORLD_EXECUTE
)
