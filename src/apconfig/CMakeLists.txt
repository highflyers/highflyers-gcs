execute_process(
	COMMAND perl ${CMAKE_CURRENT_SOURCE_DIR}/ap_code_generator.pl autopilot.hfdesc ApConfig  ${CMAKE_CURRENT_SOURCE_DIR} 
)

add_library(apconfig SHARED
	ApConfig.cpp
	ApConfigBuilder.cpp
	widgets/PIDConfigurator.cpp)
	
qt5_use_modules(apconfig Widgets)