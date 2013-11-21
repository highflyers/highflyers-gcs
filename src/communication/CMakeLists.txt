find_package(Boost)
add_library(communication 
			SerialPort.cpp
			include/communication/SerialPort.h
			include/communication/communication.h)

target_link_libraries(communication ${Boost_LIBRARIES})