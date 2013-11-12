#include "include/communication/SerialPort.h"

#include <exception>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>



using namespace std;
using namespace HighFlyers;

SerialPort::SerialPort(const char* dev, int baud)
{
	name = dev;
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
}


bool SerialPort::open_port()
{
	termios settings;
	if (
		(id = open (name.c_str(), O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK))
	  	== -1
	  	)
    	return false;
    	
    fcntl(id, F_SETFL, O_RDWR);
    tcgetattr(id, &settings);
    cfmakeraw(&settings);
    cfsetispeed(&settings, baudRate);
    cfsetospeed(&settings, baudRate);
    
	settings.c_cflag |= (CLOCAL | CREAD) ;
	settings.c_cflag &= ~PARENB ;
	settings.c_cflag &= ~CSTOPB ;
	settings.c_cflag &= ~CSIZE ;
	settings.c_cflag |= CS8 ;
	settings.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG) ;
	settings.c_oflag &= ~OPOST ;
	
	settings.c_cc[VMIN] = 0;
	settings.c_cc[VTIME] = 100;
	
	tcsetattr(id, TCSANOW | TCSAFLUSH, &settings);
	
	int id_status;
	ioctl(id, TIOCMGET, &id_status);
	id_status |= TIOCM_DTR;
	id_status |= TIOCM_RTS;
	ioctl(id, TIOCMSET, &id_status);
	
	usleep(1000);
	opened = true;
	return true;    
}
	
void SerialPort::close_port()
{
	if (opened) close(id);
}
	
void SerialPort::flush()
{
	if (opened) tcflush(id, TCIOFLUSH);
}

void SerialPort::send_char(char c)
{
	if (opened) write(id, &c, 1);
}

void SerialPort::send_string(std::string data)
{
	if (opened)
	{
		write(id, data.c_str(), data.length());
	}
}

bool SerialPort::data_available(int& howMany)
{
	if (ioctl(id, FIONREAD, &howMany) == -1)
		return false;
	return true;
}

int SerialPort::get_char()
{
	unsigned char c;
	if (read(id, &c, 1) != 1)
		return -1;	
	return ((int)c) & 0xFF;
}

static std::vector<std::string> get_ports_names()
{
	//to do
}