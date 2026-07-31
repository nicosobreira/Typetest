file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS
  src/*.c
)

add_executable(${PROJECT_NAME} ${APP_SOURCES})

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    Core
)

target_include_directories(${PROJECT_NAME} PRIVATE src)
