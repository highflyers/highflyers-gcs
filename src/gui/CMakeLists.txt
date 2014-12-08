qt5_wrap_ui(UIS_HDRS 
	ui_files/MainWindow.ui
	ui_files/CommunicationWidget.ui)
add_library(gui
	MainWindow.cpp
	VideoPlayer.cpp
	CommunicationWidget.cpp
	ColorWidget.cpp
	include/gui/MainWindow.h
	include/gui/VideoPlayer.h
	include/gui/CommunicationWidget.h
	include/gui/ColorWidget.h
	${UIS_HDRS})
	
qt5_use_modules(gui Widgets)