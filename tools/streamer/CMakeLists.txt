add_executable(streamer streamer.cpp)

target_link_libraries(streamer
	video_streamer_server
	${GSTREAMER_LIBRARIES}
	${GLIB2_LIBRARIES} 
)


qt5_use_modules(streamer Widgets)