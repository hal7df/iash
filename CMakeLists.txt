cmake_minimum_required (VERSION 2.6)
project(iash)

include_directories(${PROJECT_SOURCE_DIR}/lib)
add_subdirectory(lib)
link_directories(${PROJECT_BINARY_DIR}/iash)

add_executable (iashexample example.cpp)
target_link_libraries (iashexample iash)

install (TARGETS iash DESTINATION bin)
install (FILES "${PROJECT_SOURCE_DIR/iash.h" DESTINATION include)
