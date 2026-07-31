# Encontra a biblioteca ncurses
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

set(NCURSES_LIB "ncursesw")

file(GLOB_RECURSE CORE_SOURCES CONFIGURE_DEPENDS
  src/*.c
)

add_library(Core ${CORE_SOURCES})

target_link_libraries(Core
  PUBLIC
    ${NCURSES_LIB}
  INTERFACE
    project_flags
)

target_include_directories(Core PUBLIC include)
