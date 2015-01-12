/*
 * IProtocol.h
 *
 *  Created on: Jan 12, 2015
 *      Author: Marcin Kolny
 */

#ifndef IPROTOCOL_H_
#define IPROTOCOL_H_

#include "IPlugin.h"
#include "core/DataMaster.h"

#include <vector>

namespace HighFlyers {

class IProtocol : public IPlugin
{
public:
	virtual PluginType get_type_t() { return PluginType::PROTOCOL; }
	virtual ~IProtocol() {}

	virtual std::vector<unsigned char> serialize_node( const std::shared_ptr<DataMaster>& node ) = 0;
	virtual void append( unsigned char byte ) = 0;
	virtual void append( const std::vector<unsigned char>& bytes )
	{
		for (auto b : bytes)
			append( b );
	}
	virtual std::shared_ptr<DataMaster> get_node( const std::string& path ) = 0;
};

}

#endif /* IPROTOCOL_H_ */
