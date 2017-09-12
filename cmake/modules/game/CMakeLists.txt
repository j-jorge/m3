project( libm3_game )

set( game_module_root "${modules_root}/game" )

prepend_string(
  m3_game_files
  "${game_module_root}/src/m3/game/"

  "find_adjacent_ring_gems.cpp"
  "forward_destructions.cpp"
  "game_loop.cpp"
  "gem_generator.cpp"
  "gem_ring.cpp"
  "random_ring_without_match.cpp"
  )

add_library( m3_game STATIC ${m3_game_files} )

target_include_directories(
  m3_game
  PUBLIC
  "${game_module_root}/include"
  )

target_link_libraries(
  m3_game
  PUBLIC
  m3_table
  m3_math
  m3_boost
  m3_stl
  )

install(
  TARGETS m3_game
  DESTINATION ${M3_INSTALL_LIBRARY_DIR}
  )

add_subdirectory( tests )

