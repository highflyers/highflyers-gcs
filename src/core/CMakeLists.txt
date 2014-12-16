add_library(core
	PluginLoader.cpp
	CoreController.cpp
	PluginSetLoader.cpp
	Image.cpp
	DataMaster.cpp
	logger/MultiOutPolicy.cpp)

include_directories(include/core)

IF( "${CMAKE_SYSTEM}" MATCHES "Linux")
	target_link_libraries(core dl)
ENDIF( "${CMAKE_SYSTEM}" MATCHES "Linux")

target_link_libraries(core ${Boost_LIBRARIES})

qt5_use_modules(core Widgets)