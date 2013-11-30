/*
 * Logger.cpp
 *
 *  Created on: 30 lis 2013
 *      Author: Marcin Kolny
 */

#include "Logger.h"
#include <chrono>
#include <ctime>
#include <regex>
#include <stdexcept>
#include <boost/regex.hpp>

using namespace std;

Logger::Logger(const string& file_pattern)
: info_cnt(0),
  warning_cnt(0),
  error_cnt(0),
  file_cnt(0),
  max_file_size(-1),
  use_date(true),
  write_summary(true),
  level_mask(ALL),
  file_pattern(file_pattern)
{
	reopen_file();
}

Logger::~Logger()
{
	if (write_summary)
	{
		file << endl << "Errors: " << error_cnt << ", Warnings: "
				<< warning_cnt << ", Infos: " << info_cnt << endl;
	}

	file.close();
}

void Logger::reopen_file()
{
	if (file.is_open())
		file.close();

	std::string new_filename = generate_filename();
	file.open(new_filename.c_str());

	if (file.is_open())
		throw runtime_error("Cannot open file: " + new_filename);

	file_cnt++;
}

string Logger::generate_filename()
{
	boost::regex re("%num");
	// todo std regex support in gcc not completed yet
	return boost::regex_replace(file_pattern, re, std::to_string(file_cnt));
}

void Logger::log(const string& text, LogLevel level)
{
	switch (level)
	{
	case INFO:
		file << "[INFO]";
		info_cnt++;
		break;
	case WARNING:
		file << "[WARNING]";
		warning_cnt++;
		break;
	case ERROR:
		file << "[ERROR]";
		error_cnt++;
		break;
	default:
		file << "[UNKNOWN]";
	}

	auto now = chrono::system_clock::now();
	time_t now_c = chrono::system_clock::to_time_t(now);
	struct tm *parts = std::localtime(&now_c);

	if (use_date)
	{
		file << "[" << 1900 + parts->tm_year << "-" << 1 + parts->tm_mon << "-" << parts->tm_mday
				<< parts->tm_hour << ":" << parts->tm_min << ":" << parts->tm_sec << "]";
	}

	file << ": " << text << endl;
}
