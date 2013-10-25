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

namespace HighFlyers
{
class PIDConfigurator : public QWidget
{
private:
	static const char* pid_names[3];
	std::map<const char*, QLineEdit*> edits;

	QStringList pid_items = {"AcroRoll", "AcroPitch", "AcroYaw", "StableRoll", "StablePitch", "Alt", "Vel"};
public:
	PIDConfigurator(QWidget* parent = 0);

	template<typename T>
	void set(const char* label, const T& value)
	{}

	template<typename T>
	T get(const char* label)
	{
		return T();
	}
};
}
#endif /* PIDCONFIGURATOR_H_ */
