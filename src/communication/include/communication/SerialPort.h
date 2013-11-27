#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H
#include <termios.h>
#include <string>
#include <vector>
#include <boost/asio/serial_port.hpp>

namespace HighFlyers
{

class SerialPort
{
public:

	SerialPort(const std::string device, int baud);
		
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
	int baud_rate;
	boost::asio::io_service io;

	std::string name;
	bool opened;
	boost::asio::serial_port port;
};

}

#endif
