include( google-test )

function( declare_test SOURCE NAME )
  add_executable( ${NAME}-test ${SOURCE} )
  add_test( ${NAME} ${NAME}-test )

  foreach( LIB ${ARGN} )
    target_link_libraries( ${NAME}-test ${LIB} )
  endforeach()
    
  target_link_libraries( ${NAME}-test gtest gtest_main )
endfunction()
