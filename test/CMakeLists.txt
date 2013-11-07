find_package(GTest REQUIRED)
set(SOURCE UTmain.cpp)
include_directories(${GTEST_INCLUDE_DIRS}
		${GCS_SOURCE_DIR}/src)
	
add_subdirectory(core)

add_executable(Test ${SOURCE})
target_link_libraries(Test telemetry_controls ${GTEST_BOTH_LIBRARIES} pthread)

qt5_use_modules(Test Widgets)

