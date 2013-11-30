/*
 * Logger.h
 *
 *  Created on: 30 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <fstream>

class Logger
{
public:
	enum LogLevel
	{
		NONE = 0,
		INFO = 1,
		WARNING = 2,
		ERROR = 4,
		ALL = 7
	};

private:
	int info_cnt;
	int warning_cnt;
	int error_cnt;
	int file_cnt;

	bool use_date;
	bool write_summary;
	LogLevel level_mask;
	std::string file_pattern;
	int max_file_size;

	std::ofstream file;

	void reopen_file();
	std::string generate_filename();

public:
	Logger(const std::string& file_pattern);
	virtual ~Logger();

	void log(const std::string& text, LogLevel level);
};

#endif /* LOGGER_H_ */
