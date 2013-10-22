qt5_wrap_ui(UIS_HDRS MainWindow.ui)
add_library(gui
	MainWindow.cpp
	${UIS_HDRS})
	
qt5_use_modules(gui Widgets)