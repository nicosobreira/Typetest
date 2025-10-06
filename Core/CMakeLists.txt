# Encontra a biblioteca ncurses
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

set(NCURSES_LIB "ncursesw")

# Busca recursivamente os arquivos .c na pasta src e um nível abaixo
file(GLOB_RECURSE SOURCES
  "Source/Core/*.c"
)

# Cria o executável
add_library(Core STATIC)

target_sources(Core PRIVATE ${SOURCES})

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})

# Linka com a biblioteca ncurses
target_link_libraries(Core ${NCURSES_LIB})

target_include_directories(Core PUBLIC "Source")
