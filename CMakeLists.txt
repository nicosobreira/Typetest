cmake_minimum_required(VERSION 3.10)
project(typetest C)

# Define o padrão da linguagem C
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Define os modos de compilação
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

# Adiciona opções de compilação por modo
set(CMAKE_C_FLAGS_DEBUG "-g -Wall")
set(CMAKE_C_FLAGS_RELEASE "-O2")

# Encontra a biblioteca ncurses
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

set(NCURSES_LIB "ncursesw")

# Inclui diretórios com headers
include_directories(src)

# Busca recursivamente os arquivos .c na pasta src e um nível abaixo
file(GLOB_RECURSE SOURCE_FILES
    "src/*.c"
)

# Cria o executável
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Linka com a biblioteca ncurses
target_link_libraries(${PROJECT_NAME} ${NCURSES_LIB})
