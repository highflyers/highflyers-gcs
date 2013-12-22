/*
 * ICommunicationPlugin.h
 *
 *  Created on: 22 gru 2013
 *      Author: Marcin Kolny
 */

#ifndef ICOMMUNICATIONPLUGIN_H_
#define ICOMMUNICATIONPLUGIN_H_

#include "IPlugin.h"
#include "core/IObservable.h"
#include <QWidget>

namespace HighFlyers {

class CommunicationObserver
{
public:
	virtual ~CommunicationObserver() {}
	virtual void on_receive( IObservable<CommunicationObserver>* sender, std::string data ) = 0;
};

class ICommunicationPlugin : public IPlugin, public IObservable<CommunicationObserver>
{
public:
	virtual QWidget* get_configuration_widget() = 0;
	virtual PluginType get_type_t() { return PluginType::COMMUNICATION; }
	virtual ~ICommunicationPlugin() {}

	virtual bool open() = 0;
	virtual void close() = 0;
	virtual void send_char( char c ) = 0;

	virtual void send_string( std::string data )
	{
		for (auto c : data)
			send_char(c);
	}

	virtual bool data_available( int& how_many ) = 0;
	virtual int get_char() = 0;
	virtual bool is_open() = 0;
};

}


#endif /* ICOMMUNICATIONPLUGIN_H_ */
