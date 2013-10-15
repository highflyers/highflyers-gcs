find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS}
		${GCS_SOURCE_DIR}/src)
	
add_subdirectory(${GCS_SOURCE_DIR}/test/core)
		
add_executable(test UTmain.cpp core)
target_link_libraries(test ${GTEST_BOTH_LIBRARIES} core)
