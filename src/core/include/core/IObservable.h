/*
 * IObservable.h
 *
 *  Created on: 15 pa? 2013
 *      Author: Marcin Kolny
 */

#ifndef IOBSERVABLE_H_
#define IOBSERVABLE_H_

#include <set>
#include <functional>
#include <memory>
#include <type_traits>

namespace HighFlyers
{

template<class OBSERVER>
class IObservable
{
protected:
	std::set<OBSERVER*> observers;

	template<typename ...T>
	void notify( typename std::common_type<std::function<void( OBSERVER*, IObservable<OBSERVER>*, T... value )>>::type fnc, T... values )
	{
		for (auto o : observers)
			fnc( o, this, values... );
	}

public:
	virtual ~IObservable() {}

	void register_observer( OBSERVER* observer )
	{
		observers.insert( observer );
	}
	void unregister_observer( OBSERVER* observer )
	{
		observers.erase( observer );
	}
};
}

#endif /* IOBSERVABLE_H_ */
