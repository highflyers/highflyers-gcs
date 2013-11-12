#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H
#include <termios.h>
#include <string>
#include <vector>

namespace HighFlyers
{

class SerialPort
{
public:
	SerialPort(const char* dev, int baud);
	SerialPort(const std::string name, int baud)
		: SerialPort(name.c_str(), baud)
		{}
		
	~SerialPort()
	{
		if (opened) close_port();
	}
	
	bool open_port();
	void close_port();
	void flush();
	void send_char(char c);
	void send_string(std::string data);
	bool data_available(int& howMany);
	int get_char();
	bool is_open() { return opened; }
	static std::vector<std::string> get_ports_names();
	
private:
	speed_t baudRate;
	std::string name;
	bool opened;
	int id;
};

}

#endif