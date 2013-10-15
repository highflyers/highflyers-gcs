cmake_minimum_required (VERSION 2.8)
project (GCS)

set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")

include_directories(${GCS_SOURCE_DIR}/src/core/include)

add_subdirectory (src) 
add_subdirectory (test)

