/*
 * IObservable.cpp
 *
 *  Created on: 15 pa? 2013
 *      Author: Marcin Kolny
 */

#include <gtest/gtest.h>
#include "core/IObservable.h"

using namespace HighFlyers;

class SampleObserver
{
private:
	int last_value;
public:
	void value_added( IObservable<SampleObserver>*, int value )
	{
		last_value = value;
	}

	int get_last_value()
	{
		return last_value;
	}
};

class SampleObservable : public IObservable<SampleObserver>
{
public:
	int append_value(int value)
	{
		notify<int>( &SampleObserver::value_added, value );
		return 0;
	}
};

TEST( IObservableTest, ShouldNotifyObserver )
{
	SampleObservable observable;
	SampleObserver observer;
	observable.register_observer( &observer );

	observable.append_value( 12 );
	ASSERT_EQ( 12, observer.get_last_value() );
}

TEST( IObservableTest, ShouldCorrectlyRegisterAndUnregisterObserver )
{
	SampleObservable observable;
	SampleObserver observer;
	observable.register_observer( &observer );

	observable.append_value( 12 );
	ASSERT_EQ( 12, observer.get_last_value() );

	observable.unregister_observer( &observer );

	observable.append_value( 13 );
	ASSERT_NE( 13, observer.get_last_value() );
}
