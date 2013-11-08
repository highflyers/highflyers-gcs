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
#include <QComboBox>

namespace HighFlyers
{
class PIDConfigurator : public QWidget
{
	Q_OBJECT

private:
	static const char* updated_value_style;
	static const char* edited_value_style;
	static const char* pid_names[3];

	struct cmp_str
	{
		bool operator()(char const *a, char const *b)
		{
			return strcmp(a, b) == 0;
		}
	};

	struct ModelValues
	{
		double value;
		bool updated;
	};

	bool transaction;
	int type;
	std::map<const char*, ModelValues, cmp_str> values;

	std::map<const char*, QLineEdit*> edits;
	std::map<int, std::map<const char*, ModelValues, cmp_str>> model;
	QLocale loc;
	QStringList pid_items = {"AcroRoll", "AcroPitch", "AcroYaw", "StableRoll", "StablePitch", "Alt", "Vel"};
	bool set_transaction;

	QComboBox* pid_combo;

	void set_value(const char* l, double value);
	double get_value(const char* l);

	void p_value_changed(const QString& text);
	void i_value_changed(const QString& text);
	void d_value_changed(const QString& text);
	void value_changed(const char* str);

public Q_SLOTS:
	void update_view(int index);

public:
	PIDConfigurator(QWidget* parent = 0);

	template<typename T>
	void set(const char* label, const T& value)
	{
		if (edits.count(label) || !strcmp(label, "pid_type"))
		{
			set_value(label, value);
		}
	}

	template<typename T>
	T get(const char* label)
	{
		if (edits.count(label) || !strcmp(label, "pid_type"))
			return get_value(label);

		return T();
	}

	void start_transaction();
	void stop_transaction();
};
}
#endif /* PIDCONFIGURATOR_H_ */
