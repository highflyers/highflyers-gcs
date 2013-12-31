include_directories(include/fake_comm)

add_library(fake_comm 
	SHARED
	FakeComm.cpp
	FakeCommBuilder.cpp
	include/fake_comm/FakeComm.h)
	
qt5_use_modules(fake_comm Widgets)