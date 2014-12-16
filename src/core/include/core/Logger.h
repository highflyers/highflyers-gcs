/*
 * Logger.h
 *
 *  Created on: 5 gru 2013
 *      Author: Marcin Kolny
 */

#ifndef LOG_H_
#define LOG_H_

#include <string>
#include <ostream>
#include <map>
#include <mutex>

namespace HighFlyers {

enum LogLevel
{
	NONE = 0,
	ERROR = 1,
	WARNING = 2,
	INFO = 4,
	DEBUG = 8,
	ALL = 15
};

class NoMtSafer
{};

class MtSafer
{
private:
	std::mutex m;
public:
	MtSafer() { m.lock(); }
	virtual ~MtSafer() { m.unlock(); }
};

template<typename OUT_POLICY=std::ostream, typename THREAD_POLICY=MtSafer>
class Logger
{
private:
	static std::map<LogLevel, std::string> log_level_str;

	OUT_POLICY& out_stream;
	const LogLevel level;

	template<LogLevel CL, typename OP, bool val>
	struct Printer;

	struct DummyOutput : public OUT_POLICY
	{
		template<typename T>
		DummyOutput& operator<<( T ) { return *this; }
	};

	template<LogLevel CL, typename OP>
	struct Printer<CL, OP, false>
	{
		static OP& print( OP& out )
		{
			static DummyOutput dummy;
			return dummy;
		}
	};

	template<LogLevel CL, typename OP>
	struct Printer<CL, OP, true>
	{
		static OP& print( OP& out )
		{
			out << generate_head_of_line<CL>();
			return out;
		}
	};

	template<LogLevel CURRENT_LEVEL>
	static std::string generate_head_of_line();

	void print_impl()
	{
		out_stream << std::endl;
	}

	template<typename First, typename...Rest >
	void print_impl( First parm1, Rest...parm )
	{
		out_stream << parm1;
		print_impl( parm... );
	}

public:
	Logger( LogLevel level, OUT_POLICY& out_stream ):out_stream( out_stream ), level( level ) {}

	template<LogLevel CURRENT_LEVEL>
	OUT_POLICY& get();
	LogLevel get_level() const;

	template<LogLevel CURRENT_LEVEL, typename...Args>
	void print( Args...args )
	{
		THREAD_POLICY mts;
		out_stream << generate_head_of_line<CURRENT_LEVEL>();
		print_impl( args... );
	}
};
}

#define ERROR_LOG(...) logger.print<HighFlyers::LogLevel::ERROR>( __VA_ARGS__ )
#define WARNING_LOG(...) logger.print<HighFlyers::LogLevel::WARNING>( __VA_ARGS__ )
#define INFO_LOG(...) logger.print<HighFlyers::LogLevel::INFO>( __VA_ARGS__ )
#define DEBUG_LOG(...) logger.print<HighFlyers::LogLevel::DEBUG>( __VA_ARGS__ )

#include "logger/Logger.hpp"

#endif /* LOG_H_ */
