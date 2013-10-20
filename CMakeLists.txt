cmake_minimum_required (VERSION 2.8)
project (GCS)
enable_testing()
set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")

include_directories(${GCS_SOURCE_DIR}/src/core/include)
include_directories(${GCS_SOURCE_DIR}/src/apconfig/include)

find_package(Qt5Widgets REQUIRED)
ADD_DEFINITIONS(-DQT_NO_KEYWORDS)

add_subdirectory (src) 
add_subdirectory (test)
add_test(NAME Test COMMAND Test)

