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

add_subdirectory(Core)
add_subdirectory(App)
