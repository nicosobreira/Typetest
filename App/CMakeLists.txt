# Encontra a biblioteca ncurses
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

set(NCURSES_LIB "ncursesw")

file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS
  Source/*.c
)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE ${APP_SOURCES})

target_link_libraries(${PROJECT_NAME}
  Core
)

target_include_directories(${PROJECT_NAME} PRIVATE Source)
