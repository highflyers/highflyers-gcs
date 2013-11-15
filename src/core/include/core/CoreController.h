/*
 * CoreController.h
 *
 *  Created on: 15 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef CORECONTROLLER_H_
#define CORECONTROLLER_H_

#include "PluginLoader.h"

namespace HighFlyers
{
class CoreController
{
private:
	PluginLoader_* loader;

public:
	CoreController();
	virtual ~CoreController(){}

	IPluginInterface* load_plugin(const std::string& filename, PluginType type);
};

}
#endif /* CORECONTROLLER_H_ */
