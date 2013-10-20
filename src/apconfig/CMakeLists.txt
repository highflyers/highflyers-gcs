add_library(apconfig SHARED
	widgets/PIDConfigurator.cpp)
	
qt5_use_modules(apconfig Widgets)