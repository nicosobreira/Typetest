cmake_minimum_required(VERSION 3.15)

project(typetest
  LANGUAGES C
)

# Compiler flags and properties
add_library(project_flags INTERFACE)

add_subdirectory(Core)
add_subdirectory(App)

set(DEBUG_FLAGS
  -g
  -DDEBUG
)

set(RELEASE_FLAGS
  -O3
  -DNDEBUG
)

set(RELWITHDEFINFO_FLAGS
  -O3
  -g
  -fno-omit-frame-pointer
  -DNDEBUG
)

if (MSVC)
  target_compile_options(project_flags INTERFACE /W4)
else()
  target_compile_options(project_flags INTERFACE
    $<$<CONFIG:Debug>:${DEBUG_FLAGS}>
    $<$<CONFIG:Release>:${RELEASE_FLAGS}>
    $<$<CONFIG:RelWithDebInfo>:${RELWITHDEBINFO_FLAGS}>
  )

  target_compile_options(project_flags INTERFACE
    -Wall
    -Wextra
    -pedantic
    -Wpedantic
    -Wshadow
    # -Wconversion
    -Wformat=2
    -Werror
  )
endif()

target_compile_features(project_flags INTERFACE c_std_11)
