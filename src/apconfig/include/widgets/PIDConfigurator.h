/*
 * PIDConfigurator.h
 *
 *  Created on: 19 pa? 2013
 *      Author: Marcin Kolny
 */

#ifndef PIDCONFIGURATOR_H_
#define PIDCONFIGURATOR_H_

#include <map>
#include <array>
#include <QLineEdit>

class PIDConfigurator : QWidget
{
private:
	static std::array<const char*, 3> pid_names;
	std::map<const char*, QLineEdit*> edits;
public:
	PIDConfigurator(QWidget* parent = 0);
};

#endif /* PIDCONFIGURATOR_H_ */
