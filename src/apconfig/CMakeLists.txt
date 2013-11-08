execute_process(
	COMMAND perl ${CMAKE_CURRENT_SOURCE_DIR}/ap_code_generator.pl autopilot.hfdesc ApConfig  ${CMAKE_CURRENT_SOURCE_DIR} 
)

set(HEADER_PATH include/apconfig)

add_library(apconfig SHARED
	ApConfig.cpp
	${HEADER_PATH}/ApConfig.h
	ApConfigBuilder.cpp
	widgets/PIDConfigurator.cpp
	${HEADER_PATH}/widgets/PIDConfigurator.h)
	
qt5_use_modules(apconfig Widgets)