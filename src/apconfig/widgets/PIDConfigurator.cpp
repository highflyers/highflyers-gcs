/*
 * PIDConfigurator.cpp
 *
 *  Created on: 21 pa? 2013
 *      Author: Marcin Kolny
 */

#include "apconfig/widgets/PIDConfigurator.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <stdexcept>

using namespace HighFlyers;

const char* PIDConfigurator::pid_names[] = {"P", "I", "D"};

PIDConfigurator::PIDConfigurator(QWidget* parent)
: QWidget(parent),
  loc(QLocale::Polish, QLocale::Poland)
{
	QVBoxLayout* main_layout = new QVBoxLayout();

	for (const char* name : pid_names)
	{
		edits[name] = new QLineEdit();
		QHBoxLayout* sub_lay = new QHBoxLayout();
		sub_lay->addWidget(new QLabel(name + QString(":")));
		sub_lay->addWidget(edits[name]);
		main_layout->addItem(sub_lay);
	}

	QComboBox* pid_combo = new QComboBox();

	pid_combo->addItems(pid_items);

	main_layout->addWidget(pid_combo);

	setLayout(main_layout);
}

void PIDConfigurator::set_value(const char* l, double value)
{
	edits[l]->setText(loc.toString(value));
}

double PIDConfigurator::get_value(const char* l)
{
	bool ok;
	double v = loc.toDouble(edits[l]->text(), &ok);

	if (!ok)
		throw std::runtime_error("Cannot get value from PID configurator.");

	return v;
}
