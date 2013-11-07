add_subdirectory(video_streamer)
add_subdirectory(apconfig)
add_subdirectory(gui)
add_subdirectory(core)
add_subdirectory(telemetry_controls)

add_executable(GCS main.cpp gui core)

target_link_libraries(GCS ${GSTREAMER_LIBRARIES} video_streamer_server video_streamer_client)
target_link_libraries(GCS gui core)
qt5_use_modules(GCS Widgets)

