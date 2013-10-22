/*
 * PIDConfigurator.h
 *
 *  Created on: 19 pa? 2013
 *      Author: Marcin Kolny
 */

#ifndef PIDCONFIGURATOR_H_
#define PIDCONFIGURATOR_H_

#include <map>
#include <QLineEdit>

class PIDConfigurator : QWidget
{
private:
	constexpr static const char* pid_names[] = {"P", "I", "D"};
	std::map<const char*, QLineEdit*> edits;

	QStringList pid_items = {"AcroRoll", "AcroPitch", "AcroYaw", "StableRoll", "StablePitch", "Alt", "Vel"};
public:
	PIDConfigurator(QWidget* parent = 0);
};

#endif /* PIDCONFIGURATOR_H_ */
