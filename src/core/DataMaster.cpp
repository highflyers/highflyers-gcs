/*
 * DataMaster.cpp
 *
 *  Created on: Dec 4, 2014
 *      Author: loganek
 */

#include "DataMaster.h"

using namespace HighFlyers;

std::shared_ptr<DataMaster> DataMaster::root = std::shared_ptr<DataMaster>();

std::shared_ptr<DataMaster> DataMaster::get_root()
{
	if (!root)
		root = std::shared_ptr<DataMaster>(new DataMaster());

	return root;
}

std::shared_ptr<DataMaster> DataMaster::get_item_by_path( const std::string& path, bool fail_if_not_exists )
{
	if (path.empty())
		return shared_from_this();

	if (path[0] == '/')
		return get_root()->get_item_by_path( std::string( path, 1 ), fail_if_not_exists );

	std::size_t p = path.find ('/');

	std::string item_name = (p == std::string::npos) ? path : std::string( path, 0, p );

	if (children.find( item_name ) == children.end())
	{
		if (fail_if_not_exists)
			throw std::runtime_error( "path " + path + " doesn't exist" );
		else
			children[item_name] = std::shared_ptr<DataMaster>(new DataMaster());
	}

	if (p == std::string::npos || p == path.length()-1)
		return children[item_name];

	return children[std::string( path, 0, p )]->get_item_by_path( std::string( path, p+1 ), fail_if_not_exists );
}

std::shared_ptr<DataMaster> DataMaster::register_item( const std::string& path )
{
	return get_item_by_path( path, false );
}

std::shared_ptr<DataMaster> DataMaster::get_item( const std::string& path )
{
	return get_item_by_path( path, true );
}

void DataMaster::subscribe_var( const std::string& name )
{
	// todo
}

void DataMaster::unsubscribe_var( const std::string& name )
{
	// todo
}

void DataMaster::subscribe_item()
{
	// todo
}

void DataMaster::unsubscribe_item()
{
	// todo
}
