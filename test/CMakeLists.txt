find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS}
		${GCS_SOURCE_DIR}/src)
add_executable(test UTmain.cpp)
target_link_libraries(test ${GTEST_BOTH_LIBRARIES})
