add_library(core
	PluginLoader.cpp)

IF( "${CMAKE_SYSTEM}" MATCHES "Linux")
	target_link_libraries(core dl)
ENDIF( "${CMAKE_SYSTEM}" MATCHES "Linux")