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

template<typename OUT_POLICY=std::ostream, LogLevel LOG_LEVEL=INFO, typename THREAD_POLICY=MtSafer>
class Logger
{
private:
	static std::map<LogLevel, std::string> log_level_str;

	OUT_POLICY* out_stream;

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
		static OP& print(OP& out)
		{
			out << generate_head_of_line<CL>();
			return out;
		}
	};

	template<LogLevel CURRENT_LEVEL>
	static std::string generate_head_of_line();

	public:
	Logger(OUT_POLICY* out_stream):out_stream(out_stream){}

	template<LogLevel CURRENT_LEVEL>
	OUT_POLICY& get();
	LogLevel get_level() const;
};

#include "logger/Logger.hpp"

#endif /* LOG_H_ */
