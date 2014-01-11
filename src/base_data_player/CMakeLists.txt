include_directories(include/base_data_player)

add_library(base_data_player
	SHARED
	BaseDataPlayer.cpp
	BaseDataPlayerBuilder.cpp
	include/base_data_player/BaseDataPlayer.h)
	
qt5_use_modules(base_data_player Widgets)