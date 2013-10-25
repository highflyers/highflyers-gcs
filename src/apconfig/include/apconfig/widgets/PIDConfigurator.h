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
#include <QLocale>

namespace HighFlyers
{
class PIDConfigurator : public QWidget
{
private:
	static const char* pid_names[3];
	std::map<const char*, QLineEdit*> edits;
	QLocale loc;
	QStringList pid_items = {"AcroRoll", "AcroPitch", "AcroYaw", "StableRoll", "StablePitch", "Alt", "Vel"};

	void set_value(const char* l, double value);
	double get_value(const char* l);

public:
	PIDConfigurator(QWidget* parent = 0);

	template<typename T>
	void set(const char* label, const T& value)
	{
		if (edits.count(label))
		{
			set_value(label, value);
		}
	}

	template<typename T>
	T get(const char* label)
	{
		if (edits.count(label))
			return get_value(label);

		return T();
	}
};
}
#endif /* PIDCONFIGURATOR_H_ */
