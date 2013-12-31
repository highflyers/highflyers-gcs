qt5_wrap_ui(UIS_HDRS ui_files/MainWindow.ui)
add_library(gui
	MainWindow.cpp
	VideoPlayer.cpp
	include/gui/MainWindow.h
	include/gui/VideoPlayer.h
	${UIS_HDRS})
	
qt5_use_modules(gui Widgets)