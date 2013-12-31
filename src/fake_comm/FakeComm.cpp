/*
 * FakeComm.cpp
 *
 *  Created on: 31 gru 2013
 *      Author: Marcin Kolny
 */

#include "FakeComm.h"
#include <random>

FakeComm::FakeComm()
: open_var(false),
  rand_generator_thread(nullptr)
{
}

FakeComm::~FakeComm()
{
	close();
}

QWidget* FakeComm::get_configuration_widget()
{
	return nullptr;
}

void FakeComm::generate_data()
{
	typedef std::uniform_int_distribution<int> uni_int_dist;
	std::random_device rd;
	std::default_random_engine e1( rd() );
	uni_int_dist choose_dist( 0, 1 );
	uni_int_dist byte_cnt_dist( 1, 100 );
	uni_int_dist rand_byte( 0, 255 );

	while (open_var)
	{
		if (!choose_dist( e1 ))
			continue;

		int cnt = byte_cnt_dist( e1 );

		vals_mutex.lock();

		rand_vals.clear();

		for (int i = 0; i < cnt; i++)
			rand_vals.push_back( rand_byte( e1 ) );

		vals_mutex.unlock();
	}
}

bool FakeComm::open()
{
	if (open_var)
		return false;

	open_var = true;
	rand_generator_thread = new std::thread(&FakeComm::generate_data, this);

	return true;
}

void FakeComm::close()
{
	if (!open_var)
		return;

	open_var = false;
	rand_generator_thread->join();
	rand_vals.clear();
	delete rand_generator_thread;
	rand_generator_thread = nullptr;
}

void FakeComm::send_char( char c )
{
}

bool FakeComm::data_available( int& how_many )
{
	vals_mutex.lock();
	how_many = rand_vals.size();
	vals_mutex.unlock();

	return !!how_many;
}

int FakeComm::get_char()
{
	vals_mutex.lock();

	if (rand_vals.size())
	{
		int r = rand_vals.back();

		rand_vals.pop_back();
		vals_mutex.unlock();

		return r;
	}

	vals_mutex.unlock();
	return -1;
}

bool FakeComm::is_open()
{
	return open_var;
}
