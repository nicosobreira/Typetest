cmake_minimum_required(VERSION 3.15)

project(typetest
  LANGUAGES C
)

add_library(project_flags INTERFACE)

target_compile_features(project_flags INTERFACE c_std_11)

target_compile_definitions(project_flags INTERFACE
  $<$<CONFIG:Debug>:DEBUG>
  $<$<NOT:$<CONFIG:Debug>>:NDEBUG>
)

target_compile_options(project_flags INTERFACE
  $<$<OR:$<C_COMPILER_ID:GNU>,$<C_COMPILER_ID:Clang>>:
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wformat=2
    -Wconversion
    -Wsign-conversion
    -Wfloat-conversion
    -Wnull-dereference
    -Wimplicit-fallthrough
    $<$<CONFIG:Debug>:-g>
    $<$<CONFIG:RelWithDebInfo>:-O3;-g;-fno-omit-frame-pointer>
  >

  $<$<C_COMPILER_ID:MSVC>:
    /W4
    /WX
  >
)

add_subdirectory(Core)
add_subdirectory(App)

