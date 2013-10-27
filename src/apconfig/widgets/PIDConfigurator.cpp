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

	QObject::connect(edits["P"], &QLineEdit::textChanged, this, &PIDConfigurator::p_value_changed);
	QObject::connect(edits["I"], &QLineEdit::textChanged, this, &PIDConfigurator::i_value_changed);
	QObject::connect(edits["D"], &QLineEdit::textChanged, this, &PIDConfigurator::d_value_changed);

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

void PIDConfigurator::p_value_changed(const QString& str)
{
	value_changed("P");
}

void PIDConfigurator::i_value_changed(const QString& str)
{
	value_changed("I");
}

void PIDConfigurator::d_value_changed(const QString& str)
{
	value_changed("D");
}

void PIDConfigurator::value_changed(const char* str)
{
	edits[str]->setStyleSheet("QLineEdit{background: #FFCCCC;}");
}
