/*
 * PIDConfigurator.h
 *
 *  Created on: 19 pa? 2013
 *      Author: Marcin Kolny
 */

#ifndef PIDCONFIGURATOR_H_
#define PIDCONFIGURATOR_H_

#include <map>
#include <QtWidgets>
#include <stdexcept>

namespace HighFlyers
{
class PIDConfigurator : public QWidget
{
	Q_OBJECT

private:
	static const char* updated_value_style;
	static const char* edited_value_style;

	struct ModelValue
	{
		double value;
		bool updated;
	};

	bool transaction;
	int type;
	std::map<char, ModelValue> values;

	std::map<char, QLineEdit*> edits;
	std::map<int, std::map<char, ModelValue>> model;
	QLocale loc;
	QStringList pid_items = {"AcroRoll", "AcroPitch", "AcroYaw", "StableRoll", "StablePitch", "Alt", "Vel"};

	QComboBox* pid_combo;

	template<typename T>
	void set_value(char l, T value);
	template<typename T>
	T get_value(char l);

	void p_value_changed(const QString& text);
	void i_value_changed(const QString& text);
	void d_value_changed(const QString& text);
	void value_changed(char str);

public Q_SLOTS:
	void update_view(int index);

public:
	PIDConfigurator(QWidget* parent = 0);
	virtual ~PIDConfigurator(){}

	template<typename T>
	void set(const char* label, const T& value)
	{
		if (strlen(label) < 1)
			throw std::runtime_error("invalid zero-length label");

		if (!strcmp(label, "pid_type"))
			set_value(label[0], value);
		else if (edits.count(std::toupper(label[0])))
			set_value(std::toupper(label[0]), value);
		else
			throw std::runtime_error("unknown label");
	}

	template<typename T>
	T get(const char* label)
	{
		if (strlen(label) < 1)
			throw std::runtime_error("invalid zero-length label");

		if (!strcmp(label, "pid_type"))
			return get_value<T>(0);
		else if (edits.count(std::toupper(label[0])))
			return get_value<T>(std::toupper(label[0]));

		throw std::runtime_error("unknown label");
	}

	void start_transaction();
	void stop_transaction();
};
}
#endif /* PIDCONFIGURATOR_H_ */
