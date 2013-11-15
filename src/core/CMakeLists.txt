add_library(core
	PluginLoader.cpp
	CoreController.cpp)

include_directories(include/core)

IF( "${CMAKE_SYSTEM}" MATCHES "Linux")
	target_link_libraries(core dl)
ENDIF( "${CMAKE_SYSTEM}" MATCHES "Linux")

qt5_use_modules(core Widgets)