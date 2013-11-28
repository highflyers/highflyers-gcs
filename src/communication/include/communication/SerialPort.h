#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H
#include <termios.h>
#include <string>
#include <vector>
#include <boost/asio/serial_port.hpp>
#include <memory>

namespace HighFlyers
{

class SerialPort
{
public:

	enum class Parity
	{
		Even,
		Odd,
		None
	};

	enum class StopBits
	{
		One,
		OnePointHalf,
		Two
	};

	enum class FlowControl
	{
		Software,
		Hardware,
		None
	};

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
	bool data_available(int& how_many);
	int get_char();
	bool is_open() { return opened; }
	static std::vector<std::string> get_ports_names();
	
	bool get_baud_rate( unsigned int& out_baud );
	bool get_parity( Parity& out_parity );
	bool get_stop_bits( StopBits& out_stop_bits );
	bool get_character_size( unsigned char & out_char_size );
	bool get_flow_control( FlowControl & out_flow_control );

	void set_baud_rate(unsigned int baud_rate);
	void set_parity(Parity parity);
	void set_stop_bits(StopBits stop_bits);
	void set_character_size(unsigned char char_size);
	void set_flow_control(FlowControl flow_control);

private:
	typedef boost::system::system_error boost_error;
	typedef boost::asio::serial_port_base::stop_bits boost_bits;
	typedef boost::asio::serial_port_base::flow_control boost_flow;
	typedef boost::asio::serial_port_base::parity boost_parity;

	int baud_rate;
	boost::asio::io_service io;

	std::string name;
	bool opened;
	boost::asio::io_service io_service;
	std::shared_ptr<boost::asio::serial_port> port;

	void translate_exception( const boost_error& ex )
	{
		throw std::runtime_error( "Exception code: " + std::to_string( ex.code().value() ) + ". " + ex.what() );
	}

	void throw_init_error()
	{
		throw std::runtime_error("SerialPort object has not been correctly initialized");
	}

	template <typename OPT>
	bool try_get_option( OPT& opt)
	{
		try
		{
			port->get_option(opt);
		}
		catch (boost_error& ex)
		{
				return false;
		}
	}

	template <typename OPT>
	void try_set_option(const OPT& opt)
	{
		try
		{
			port->set_option(opt);
		}
		catch(boost_error& ex)
		{
			translate_exception(ex);
		}
	}
};

}

#endif
