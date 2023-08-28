cmake_minimum_required(VERSION 3.22.1)
project(game)

set(CMAKE_CXX_STANDARD 23)

include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

add_executable(main main.cpp)
target_link_libraries(main ncurses)

add_executable(gameServer gameServer.cc)
target_link_libraries(gameServer WebServer)

add_subdirectory(src)
add_subdirectory(test)