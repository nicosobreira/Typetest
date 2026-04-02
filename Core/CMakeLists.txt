# Encontra a biblioteca ncurses
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

set(NCURSES_LIB "ncursesw")

file(GLOB_RECURSE CORE_SOURCES CONFIGURE_DEPENDS
  src/Core/*.c
)

add_library(Core)

target_sources(Core PRIVATE ${CORE_SOURCES})

target_link_libraries(Core PUBLIC
  project_flags
  ${NCURSES_LIB}
)

target_include_directories(Core PUBLIC src)
