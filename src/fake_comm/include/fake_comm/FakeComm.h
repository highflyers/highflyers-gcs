/*
 * FakeComm.h
 *
 *  Created on: 31 gru 2013
 *      Author: Marcin Kolny
 */

#ifndef FAKECOMM_H_
#define FAKECOMM_H_

#include "core/plugin_interfaces/ICommunicationPlugin.h"
#include <thread>
#include <mutex>
#include <vector>

class FakeComm : public HighFlyers::ICommunicationPlugin
{
private:
	bool open_var;
	std::vector<int> rand_vals;
	std::thread* rand_generator_thread;
	std::mutex vals_mutex;

	void generate_data();

public:
	FakeComm();
	virtual ~FakeComm();

	QWidget* get_configuration_widget();
	bool open();
	void close();
	void send_char( char c );
	bool data_available( int& how_many );
	int get_char();
	bool is_open();
};

#endif /* FAKECOMM_H_ */
