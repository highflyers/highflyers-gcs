/*
 * BaseDataPlayer.h
 *
 *  Created on: 11 sty 2014
 *      Author: Marcin Kolny
 */

#ifndef BASEDATAPLAYER_H_
#define BASEDATAPLAYER_H_

#include "core/plugin_interfaces/ICommunicationPlugin.h"

namespace HighFlyers
{
class BaseDataPlayer : public IPlugin, public CommunicationObserver
{
private:
public:
	BaseDataPlayer();
	virtual ~BaseDataPlayer();

	virtual HighFlyers::PluginType get_type_t()
	{
		return HighFlyers::PluginType::UNKNOW;
	}

	virtual PluginSuperPower get_super_power() const
	{ return (PluginSuperPower)(PluginSuperPower::DATA_RECEIVER | PluginSuperPower::DATA_SENDER); }

	void on_receive( IObservable<CommunicationObserver>* sender, std::string data );
	virtual void on_state_changed( IObservable<CommunicationObserver>* sender, bool state );
};
}

#endif /* BASEDATAPLAYER_H_ */
