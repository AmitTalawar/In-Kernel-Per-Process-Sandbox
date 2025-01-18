set(LLVM_TUTOR_PLUGINS
    DummySysCall
    )

set(DummySysCall_SOURCES
  DummySysCall.cpp)

foreach( plugin ${LLVM_TUTOR_PLUGINS} )
    # Create a library corresponding to 'plugin'
    add_library(
      ${plugin}
      SHARED
      ${${plugin}_SOURCES}
      )

    # Configure include directories for 'plugin'
    target_include_directories(
      ${plugin}
      PRIVATE
      "${CMAKE_CURRENT_SOURCE_DIR}/../include"
    )
endforeach()
