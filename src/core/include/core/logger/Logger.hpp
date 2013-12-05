/*
 * Logger.hpp
 *
 *  Created on: 5 gru 2013
 *      Author: Marcin Kolny
 */

#ifndef LOG_HPP_
#define LOG_HPP_

#include <sstream>
#include <chrono>
#include <ctime>

template<typename OUT_POLICY, LogLevel LOG_LEVEL, typename THREAD_POLICY>
std::map<LogLevel, std::string> Logger<OUT_POLICY, LOG_LEVEL, THREAD_POLICY>::log_level_str = {
		{ALL, "ALL"},
		{ERROR, "ERROR"},
		{WARNING, "WARNING"},
		{INFO, "WARNING"},
		{DEBUG, "WARNING"},
		{ALL, "WARNING"}
};

template<typename OUT_POLICY, LogLevel LOG_LEVEL, typename THREAD_POLICY>
template<LogLevel CURRENT_LEVEL>
OUT_POLICY& Logger<OUT_POLICY, LOG_LEVEL, THREAD_POLICY>::get()
{
	THREAD_POLICY locker;

	return Printer<CURRENT_LEVEL, OUT_POLICY,
			LOG_LEVEL>=CURRENT_LEVEL || ( LOG_LEVEL & CURRENT_LEVEL )>
	::print( *out_stream );
}

template<typename OUT_POLICY, LogLevel LOG_LEVEL, typename THREAD_POLICY>
template<LogLevel CURRENT_LEVEL>
std::string Logger<OUT_POLICY, LOG_LEVEL, THREAD_POLICY>::generate_head_of_line()
{
	auto now = std::chrono::system_clock::now();
	time_t now_c = std::chrono::system_clock::to_time_t( now );
	struct tm *parts = std::localtime( &now_c );
	std::stringstream str;

	str << "\n[" << 1900 + parts->tm_year << "-" << 1 + parts->tm_mon << "-" << parts->tm_mday
			<< " " << parts->tm_hour << ":" << parts->tm_min << ":" << parts->tm_sec << "]"
			<< "\t[" << log_level_str[CURRENT_LEVEL] << "]: ";
	return str.str();
}

template<typename OUT_POLICY, LogLevel LOG_LEVEL, typename THREAD_POLICY>
LogLevel Logger<OUT_POLICY, LOG_LEVEL, THREAD_POLICY>::get_level() const
{
	return LOG_LEVEL;
}

#endif /* LOG_HPP_ */
