qt5_wrap_ui(UIS_HDRS Battery.ui)
add_library(telemetry_controls
	SHARED
	Battery.cpp
	include/telemetry_controls/Battery.h
	${UIS_HDRS}
)	
qt5_use_modules(telemetry_controls Widgets)
