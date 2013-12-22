add_library(mcms_telemetry SHARED
	McmsTelemetryBuilder.cpp
)

target_link_libraries(mcms_telemetry core)

qt5_use_modules(mcms_telemetry Widgets)
