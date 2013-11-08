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
#include <stdexcept>

using namespace HighFlyers;

const char* PIDConfigurator::pid_names[] = {"P", "I", "D"};
const char* PIDConfigurator::updated_value_style = "QLineEdit{background: #98FB98;}";
const char* PIDConfigurator::edited_value_style = "QLineEdit{background: #FFCCCC;}";

PIDConfigurator::PIDConfigurator(QWidget* parent)
: QWidget(parent),
  set_transaction(false),
  transaction(false),
  type(-1),
  loc(QLocale::Polish, QLocale::Poland)
{
	QVBoxLayout* main_layout = new QVBoxLayout();

	QString regex = QString("(\\d|") + loc.decimalPoint() + ")+";
	QRegExpValidator* number_validator = new QRegExpValidator(QRegExp(regex), NULL);

	for (const char* name : pid_names)
	{
		edits[name] = new QLineEdit();
		edits[name]->setValidator(number_validator);
		QHBoxLayout* sub_lay = new QHBoxLayout();
		sub_lay->addWidget(new QLabel(name + QString(":")));
		sub_lay->addWidget(edits[name]);
		main_layout->addItem(sub_lay);
	}

	pid_combo = new QComboBox();

	pid_combo->addItems(pid_items);

	main_layout->addWidget(pid_combo);

	QObject::connect(edits["P"], &QLineEdit::textChanged, this, &PIDConfigurator::p_value_changed);
	QObject::connect(edits["I"], &QLineEdit::textChanged, this, &PIDConfigurator::i_value_changed);
	QObject::connect(edits["D"], &QLineEdit::textChanged, this, &PIDConfigurator::d_value_changed);
	connect(pid_combo , SIGNAL(currentIndexChanged(int)), this, SLOT(update_view(int)));
	//QObject::connect(pid_combo, &QComboBox::currentIndexChanged, this, &PIDConfigurator::pid_type_changed);

	setLayout(main_layout);
}

void PIDConfigurator::set_value(const char* l, double value)
{
	if (transaction)
	{
		values[l] = {value, true};
	}
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
	if (!transaction)
	{
		edits[str]->setStyleSheet(updated_value_style);
		model[pid_combo->currentIndex()][str] = {edits[str]->text().toDouble(), false};
	}
}

void PIDConfigurator::start_transaction()
{
	transaction = true;
}

void PIDConfigurator::stop_transaction()
{
	model[type] = values;

	if (type == pid_combo->currentIndex())
		update_view(type);

	transaction = false;
}

void PIDConfigurator::update_view(int index)
{
	for (auto edit : edits)
	{
		edit.second->setText(std::to_string(model[index][edit.first].value).c_str());
		QString style = (model[index][edit.first].updated) ?
				updated_value_style :
				edited_value_style;
		edit.second->setStyleSheet(style);
	}

}
