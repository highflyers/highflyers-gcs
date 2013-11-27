add_library(video_test_src SHARED
	VideoTestSrc.cpp
	include/video_test_src/VideoTestSrc.h
	VideoTestSrcBuilder.cpp
)

target_link_libraries(video_test_src core)

qt5_use_modules(video_test_src Widgets)
