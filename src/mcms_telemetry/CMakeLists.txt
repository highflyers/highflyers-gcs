qt5_wrap_ui(UIS_HDRS ui_files/McmsTelemetry.ui)
add_library(mcms_telemetry SHARED
	McmsTelemetryBuilder.cpp
	McmsTelemetry.cpp
	${UIS_HDRS}
)

target_link_libraries(mcms_telemetry core)

qt5_use_modules(mcms_telemetry Widgets)
