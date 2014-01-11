/*
 * BaseDataPlayer.cpp
 *
 *  Created on: 11 sty 2014
 *      Author: Marcin Kolny
 */

#include "BaseDataPlayer.h"
#include <iostream>

using namespace HighFlyers;

BaseDataPlayer::BaseDataPlayer()
{}

BaseDataPlayer::~BaseDataPlayer()
{}

void BaseDataPlayer::on_receive( IObservable<CommunicationObserver>* sender, std::string data )
{
	std::cout << "Data received: " << data << std::endl;
}

void BaseDataPlayer::on_state_changed( IObservable<CommunicationObserver>* sender, bool state )
{
	std::cout << "Changed state: " << state << std::endl;
}
