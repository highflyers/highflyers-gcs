cmake_minimum_required (VERSION 2.8.9)
project (GCS)
enable_testing()
set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")

include_directories(${GCS_SOURCE_DIR}/src/core/include)
include_directories(${GCS_SOURCE_DIR}/src/apconfig/include)
include_directories(${GCS_SOURCE_DIR}/src/video_streamer/client/include)
include_directories(${GCS_SOURCE_DIR}/src/video_streamer/server/include)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${GCS_SOURCE_DIR}/cmake/modules/")

find_package(Qt5Widgets REQUIRED)
ADD_DEFINITIONS(-DQT_NO_KEYWORDS)

find_package(GLIB2 REQUIRED)
include_directories(${GLIB2_INCLUDE_DIRS})

find_package(GStreamer REQUIRED)
include_directories(${GSTREAMER_INCLUDE_DIRS})

add_subdirectory (src) 
add_subdirectory (test)
add_test(NAME Test COMMAND Test)

