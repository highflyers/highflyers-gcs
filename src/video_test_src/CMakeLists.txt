add_library(video_test_src SHARED
	VideoTestSrc.cpp
	include/video_test_src/VideoTestSrc.h
	VideoTestSrcBuilder.cpp
)

qt5_use_modules(video_test_src Widgets)
