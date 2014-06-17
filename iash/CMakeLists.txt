set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_library(iash iash.cpp crosslib.cpp)

if(UNIX)
    target_link_libraries(iash tinfo)
endif()
