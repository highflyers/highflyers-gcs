qt5_wrap_ui(UIS_HDRS battery.ui)
add_library(telemetry_controls
	SHARED
	battery.cpp
	include/battery.h
	${UIS_HDRS}
)	
qt5_use_modules(telemetry_controls Widgets)