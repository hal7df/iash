cmake_minimum_required (VERSION 2.6)
project(IASH)

list(APPEND CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

include_directories(${PROJECT_SOURCE_DIR}/iash)
add_subdirectory(iash)
link_directories(${PROJECT_BINARY_DIR}/iash)

add_executable (iashexample example.cpp)
target_link_libraries (iashexample iash)

install (TARGETS iashexample DESTINATION bin)
install (FILES "${PROJECT_SOURCE_DIR}/iash.h" DESTINATION include)
