/*
 * SerialPortObserver.h
 *
 *  Created on: 28 lis 2013
 *      Author: pawel
 */

#ifndef SERIALPORTOBSERVER_H_
#define SERIALPORTOBSERVER_H_

#include <core/IObservable.h>

namespace HighFlyers
{
class SerialPortObserver
{
public:
	virtual ~SerialPortObserver(){}
	virtual void on_receive( IObservable<SerialPortObserver>* sender, std::string data ) = 0;
};
}



#endif /* SERIALPORTOBSERVER_H_ */
