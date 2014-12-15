/*
 * DataMaster.h
 *
 *  Created on: Dec 4, 2014
 *      Author: loganek
 */

#ifndef DATAMASTER_H_
#define DATAMASTER_H_

#include "IObservable.h"

#include <string>
#include <map>
#include <memory>

namespace HighFlyers
{

class DataMaster;

class DataMasterObserver
{
public:
	virtual ~DataMasterObserver() {}
	virtual void node_changed( IObservable<DataMasterObserver>*, std::shared_ptr<DataMaster> node ) = 0;
};


class DataMaster : public std::enable_shared_from_this<DataMaster>, public IObservable<DataMasterObserver>
{
public:
	class VarObserver
	{
	public:
		virtual ~VarObserver() {}
		virtual void value_changed( IObservable<VarObserver>*, const std::string& var_name ) = 0;
	};

private:
	class BaseItem : public IObservable<VarObserver>
	{
	public:
		virtual ~BaseItem() {}

		friend class DataMaster;
	};

	template<typename T>
	class Item : public BaseItem
	{
	public:
		T value;
	};

	// listeners; todo

	DataMaster( std::shared_ptr<DataMaster> parent = nullptr ) : parent( parent ){}
	DataMaster( const DataMaster& ) = delete;
	DataMaster& operator=( const DataMaster& ) = delete;

	std::map<std::string, std::shared_ptr<DataMaster>> children;
	std::map<std::string, std::shared_ptr<BaseItem>> vars;

	static std::shared_ptr<DataMaster> root;

	std::shared_ptr<DataMaster> parent;

	template<typename T>
	std::shared_ptr<Item<T>> safe_get_item( const std::string& name );

	std::shared_ptr<DataMaster> get_item_by_path( const std::string& path, bool fail_if_not_exists );

	void notify_change();

public:
	void subscribe_var( VarObserver* observer, const std::string& name );
	void unsubscribe_var( DataMaster::VarObserver* observer, const std::string& name );

	template<typename T>
	void set_value( const std::string& name, const T& value );

	template<typename T>
	T get_value( const std::string& name );

	std::shared_ptr<DataMaster> get_item( const std::string& path );

	std::shared_ptr<DataMaster> register_item( const std::string& path );

	template<typename T>
	void register_var( const std::string& name );

	static std::shared_ptr<DataMaster> get_root();
};

template<typename T>
std::shared_ptr<DataMaster::Item<T>> DataMaster::safe_get_item( const std::string& name )
{
	if (vars.find( name ) == vars.end())
		throw std::runtime_error( "value " + name + " doesn't exist in specific node" );

	auto tmp = std::dynamic_pointer_cast<Item<T>>(vars[name]);

	if (tmp == nullptr)
		throw std::runtime_error( "invalid requested type of value " + name );

	return tmp;
}

template<typename T>
T DataMaster::get_value( const std::string& name )
{
	return safe_get_item<T>( name )->value;
}

template<typename T>
void DataMaster::set_value( const std::string& name, const T& value )
{
	auto item = safe_get_item<T>( name );
	item->value = value;
	item->notify(&VarObserver::value_changed, name);
	notify_change();
}

template<typename T>
void DataMaster::register_var( const std::string& name )
{
	if (vars.find( name ) == vars.end())
	{
		notify_change();
		vars[name] = std::make_shared<Item<T>>();
	}
}

}

#endif /* DATAMASTER_H_ */
