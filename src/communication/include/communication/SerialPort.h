#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H
#include <termios.h>
#include <string>
namespace HighFlyers
{

class SerialPort
{
public:
	SerialPort(const char* dev, int baud);
	SerialPort(const std::string name, int baud)
		: SerialPort(name.c_str(), baud)
		{}
	
	bool open_port();
	void close_port();
	void flush();
	void send_char(char c);
	void send_string(std::string data);
	bool data_available(int& howMany);
	int get_char();
	
private:
	speed_t baudRate;
	std::string name;
	bool opened;
	int id;
};

}

#endif