qt5_wrap_ui(UIS_HDRS ui_files/McmsTelemetry.ui)
add_library(mcms_telemetry SHARED
	McmsTelemetryBuilder.cpp
	McmsTelemetry.cpp
	McmsTelemetryWidget.cpp
	include/mcms_telemetry/McmsTelemetry.h
	include/mcms_telemetry/McmsTelemetryWidget.h
	${UIS_HDRS}
)

qt5_use_modules(mcms_telemetry Widgets)
