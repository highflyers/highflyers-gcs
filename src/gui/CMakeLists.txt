qt5_wrap_ui(UIS_HDRS MainWindow.ui)
add_library(gui
	MainWindow.cpp
	include/gui/MainWindow.h
	${UIS_HDRS})
	
target_link_libraries(gui apconfig)
	
qt5_use_modules(gui Widgets)