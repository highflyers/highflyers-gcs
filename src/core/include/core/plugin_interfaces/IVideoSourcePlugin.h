/*
 * IVideoSourcePlugin.h
 *
 *  Created on: 21 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef IVIDEOPLUGIN_H_
#define IVIDEOPLUGIN_H_

#include "IPlugin.h"
#include "core/Image.h"

namespace HighFlyers
{

class IVideoSourcePlugin : public IPlugin
{
public:
	virtual ~IVideoSourcePlugin() {}

	virtual PluginType get_type_t() { return PluginType::VIDEO; }
	virtual void set_filename(const std::string& filename ) = 0;
	virtual void play( bool recording ) = 0;
	virtual void stop() = 0;
	virtual Image* get_image() = 0;
	virtual void set_render_window( unsigned int handler ) = 0;
};

}
#endif /* IVIDEOPLUGIN_H_ */
