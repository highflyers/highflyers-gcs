#include "include/communication/SerialPort.h"

#include <exception>
#include <stdexcept>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <thread>


using namespace std;
using namespace HighFlyers;

SerialPort::SerialPort(string dev, int baud)
{
	name = dev;
	baud_rate = baud;
	opened = false;
	port = shared_ptr<boost::asio::serial_port>(new boost::asio::serial_port(io_service));
}


bool SerialPort::open_port()
{
	if (port->is_open())
		return true;
	boost::system::error_code error;


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
	
	thread t(bind(&boost::asio::io_service::run, &io_service));
	
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
		boost_parity parity;

		if (!try_get_option( parity ) )
			return false;

		switch (parity.value())
		{
			case boost_parity::even: out_parity = Parity::Even; break;
			case boost_parity::none: out_parity = Parity::None; break;
			case boost_parity::odd: out_parity = Parity::Odd; break;
		}
	}
	return false;
}
bool SerialPort::get_stop_bits( SerialPort::StopBits& out_stop_bits )
{
	if (port->is_open())
	{
		boost_bits bits;

		if ( ! try_get_option( bits ) )
			return false;

		switch (bits.value())
		{
			case boost_bits::one: out_stop_bits = StopBits::One; break;
			case boost_bits::onepointfive: out_stop_bits = StopBits::OnePointHalf; break;
			case boost_bits::two: out_stop_bits = StopBits::Two; break;
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
		boost_flow flow;

		if (! try_get_option( flow ))
			return false;

		switch (flow.value())
		{
			case boost_flow::hardware: out_flow_control = FlowControl::Hardware; break;
			case boost_flow::software: out_flow_control = FlowControl::Software; break;
			case boost_flow::none: out_flow_control = FlowControl::None; break;
		}
		return true;
	}
	return false;
}

void SerialPort::set_baud_rate( unsigned int baud_rate )
{
	if (!port)
		throw_init_error();

	try_set_option( boost::asio::serial_port_base::baud_rate( baud_rate ) );
}

void SerialPort::set_parity(Parity parity)
{
	if (!port)
		throw_init_error();

	boost_parity b_parity;

	switch (parity)
	{
		case Parity::Even : b_parity = boost_parity(boost_parity::even); break;
		case Parity::Odd : b_parity = boost_parity(boost_parity::odd); break;
		case Parity::None : b_parity = boost_parity(boost_parity::none); break;
	}
	try_set_option( b_parity );
}

void SerialPort::set_stop_bits( StopBits stop_bits )
{
	if(!port)
		throw_init_error();

	boost_bits b_bits;

	switch (stop_bits)
	{
		case StopBits::One : b_bits = boost_bits(boost_bits::one); break;
		case StopBits::OnePointHalf : b_bits = boost_bits(boost_bits::onepointfive); break;
		case StopBits::Two : b_bits = boost_bits(boost_bits::two); break;
	}
	try_set_option( b_bits );
}

void SerialPort::set_character_size(unsigned char char_size)
{
	if (!port)
		throw_init_error();

	try_set_option(boost::asio::serial_port_base::character_size(char_size));
}

void SerialPort::set_flow_control(FlowControl flow_control)
{
	if (!port)
		throw_init_error();

	boost_flow b_flow;

	switch(flow_control)
	{
		case FlowControl::Hardware : b_flow = boost_flow(boost_flow::hardware); break;
		case FlowControl::Software : b_flow = boost_flow(boost_flow::software); break;
		case FlowControl::None : b_flow = boost_flow(boost_flow::none); break;
	}

	try_set_option(b_flow);
}

