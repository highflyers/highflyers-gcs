#include "include/communication/SerialPort.h"

#include <exception>
#include <stdexcept>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>



using namespace std;
using namespace HighFlyers;

SerialPort::SerialPort(string dev, int baud)
{
	name = dev;
	baud_rate = baud;
	switch (baud)
	{
	case     50:	baud =     B50 ; break ;
	case     75:	baud =     B75 ; break ;
	case    110:	baud =    B110 ; break ;
	case    134:	baud =    B134 ; break ;
	case    150:	baud =    B150 ; break ;
	case    200:	baud =    B200 ; break ;
	case    300:	baud =    B300 ; break ;
	case    600:	baud =    B600 ; break ;
	case   1200:	baud =   B1200 ; break ;
	case   1800:	baud =   B1800 ; break ;
	case   2400:	baud =   B2400 ; break ;
	case   4800:	baud =   B4800 ; break ;
	case   9600:	baud =   B9600 ; break ;
	case  19200:	baud =  B19200 ; break ;
	case  38400:	baud =  B38400 ; break ;
	case  57600:	baud =  B57600 ; break ;
	case 115200:	baud = B115200 ; break ;
	case 230400:	baud = B230400 ; break ;
	default:
		throw exception();
	}
	opened = false;
}


bool SerialPort::open_port()
{
	if (port)
		return true;
	boost::system::error_code error;
	port = shared_ptr<boost::asio::serial_port>(new boost::asio::serial_port(io_service));

	port->open(name, error);
	if (error)
	{
		throw runtime_error("Couldn't open port!");
	}
	opened = true;
	port->set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
	port->set_option(boost::asio::serial_port_base::character_size(8)); //make this configurable?
	port->set_option(boost::asio::serial_port_base
			::stop_bits(boost::asio::serial_port_base::stop_bits::one));
	port->set_option(boost::asio::serial_port_base
			::parity(boost::asio::serial_port_base::parity::none));
	port->set_option(boost::asio::serial_port_base
			::flow_control(boost::asio::serial_port_base::flow_control::none));
	
	//thread init here!
	
	return true;
}
	
void SerialPort::close_port()
{
	/*if (opened)
	{
		if (close(id) != 0)
			throw runtime_error("Couldn't close device file");
	}*/
}
	
void SerialPort::flush()
{
	/*if (opened) tcflush(id, TCIOFLUSH);*/
}

void SerialPort::send_char(char c)
{
	/*if (opened) write(id, &c, 1);*/
}

void SerialPort::send_string(std::string data)
{
	/*if (opened)
	{
		write(id, data.c_str(), data.length());
	}*/
}

bool SerialPort::data_available(int& how_many)
{
	/*if (ioctl(id, FIONREAD, &how_many) == -1)
		return false;
	return true;*/
}

int SerialPort::get_char()
{
	/*unsigned char c;
	if (read(id, &c, 1) != 1)
		return -1;	
	return ((int)c) & 0xFF;*/
}

static std::vector<std::string> get_ports_names()
{
	//TODO
}



bool SerialPort::get_baud_rate( unsigned int& out_baud )
{
	if (port->is_open())
	{
		boost::asio::serial_port_base::baud_rate baud;

		if ( ! try_get_option( baud ) )
			return false;

		out_baud = baud.value();
		return true;
	}
	return false;
}

bool SerialPort::get_parity( SerialPort::Parity& out_parity )
{
	if (port->is_open())
	{
		boost::asio::serial_port_base::parity parity;

		if (!try_get_option( parity ) )
			return false;

		switch (parity.value())
		{
			case parity.even: out_parity = Parity::Even; break;
			case parity.none: out_parity = Parity::None; break;
			case parity.odd: out_parity = Parity::Odd; break;
		}
	}
	return false;
}
bool SerialPort::get_stop_bits( SerialPort::StopBits& out_stop_bits )
{
	if (port->is_open())
	{
		boost::asio::serial_port_base::stop_bits bits;

		if ( ! try_get_option( bits ) )
			return false;

		switch (bits.value())
		{
			case bits.one: out_stop_bits = StopBits::One; break;
			case bits.onepointfive: out_stop_bits = StopBits::OnePointHalf; break;
			case bits.two: out_stop_bits = StopBits::Two; break;
		}
		return true;
	}
	return false;
}
bool SerialPort::get_character_size( unsigned char& out_char_size )
{
	if (port->is_open())
	{
		boost::asio::serial_port_base::character_size size;

		if ( !try_get_option( size ) )
			return false;

		out_char_size = size.value();
		return true;
	}
	return false;
}
bool SerialPort::get_flow_control( SerialPort::FlowControl& out_flow_control )
{
	if (port->is_open())
	{
		boost::asio::serial_port_base::flow_control flow;

		if (! try_get_option( flow ))
			return false;

		switch (flow.value())
		{
			case flow.hardware: out_flow_control = FlowControl::Hardware; break;
			case flow.software: out_flow_control = FlowControl::Software; break;
			case flow.none: out_flow_control = FlowControl::None; break;
		}
		return true;
	}
	return false;
}

