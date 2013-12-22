cmake_minimum_required (VERSION 2.8.9)
project (GCS)
enable_testing()
set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")


include_directories(${GCS_SOURCE_DIR}/src/gui/include)
include_directories(${GCS_SOURCE_DIR}/src/core/include)
include_directories(${GCS_SOURCE_DIR}/src/apconfig/include)
include_directories(${GCS_SOURCE_DIR}/src/video_streamer/include)
include_directories(${GCS_SOURCE_DIR}/src/telemetry_controls/include)
include_directories(${GCS_SOURCE_DIR}/src/protocol/include)
include_directories(${GCS_SOURCE_DIR}/src/protocol/c_protocol/include)
include_directories(${GCS_SOURCE_DIR}/src/protocol/c_protocol/nanopb)
include_directories(${GCS_SOURCE_DIR}/src/communication/include)
include_directories(${GCS_SOURCE_DIR}/src/video_test_src/include)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${GCS_SOURCE_DIR}/cmake/modules/")

find_package(Qt5Widgets REQUIRED)
ADD_DEFINITIONS(-DQT_NO_KEYWORDS)

find_package(GLIB2 REQUIRED)
include_directories(${GLIB2_INCLUDE_DIRS})

find_package(GStreamer REQUIRED)
include_directories(${GSTREAMER_INCLUDE_DIRS})
include_directories(${GSTREAMER_BASE_INCLUDE_DIRS})

add_subdirectory (src) 
add_subdirectory (test)
add_subdirectory (tools)
add_test(NAME Test COMMAND Test)

