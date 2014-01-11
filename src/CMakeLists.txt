add_subdirectory(video_streamer)
add_subdirectory(apconfig)
add_subdirectory(gui)
add_subdirectory(core)
add_subdirectory(telemetry_controls)
add_subdirectory(protocol)
add_subdirectory(communication)
add_subdirectory(video_test_src)
add_subdirectory(mcms_telemetry)
add_subdirectory(fake_comm)
add_subdirectory(base_data_player)

add_executable(GCS main.cpp gui core protocol video_streamer)

target_link_libraries(GCS ${GSTREAMER_LIBRARIES} ${GSTREAMER_VIDEO_LIBRARIES})
target_link_libraries(GCS video_streamer_client)
target_link_libraries(GCS gui core)
target_link_libraries(GCS nanopb c_protocol Protocol)

qt5_use_modules(GCS Widgets)

