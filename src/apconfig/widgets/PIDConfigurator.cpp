/*
 * PIDConfigurator.cpp
 *
 *  Created on: 21 pa? 2013
 *      Author: Marcin Kolny
 */

#include "apconfig/widgets/PIDConfigurator.h"

namespace HighFlyers
{
const char* PIDConfigurator::updated_value_style = "QLineEdit{background: #98FB98;}";
const char* PIDConfigurator::edited_value_style = "QLineEdit{background: #FFCCCC;}";

PIDConfigurator::PIDConfigurator(QWidget* parent)
: QWidget(parent),
  transaction(false),
  type(-1),
  loc(QLocale::Polish, QLocale::Poland)
{
	setLayout(new QVBoxLayout());

	for (const char* name : {"P", "I", "D"})
	{
		edits[name[0]] = new QLineEdit();
		edits[name[0]]->setValidator(new QDoubleValidator());
		QHBoxLayout* sub_lay = new QHBoxLayout();
		sub_lay->addWidget(new QLabel(name + QString(":")));
		sub_lay->addWidget(edits[name[0]]);
		static_cast<QHBoxLayout*>(layout())->addLayout(sub_lay);
	}

	pid_combo = new QComboBox();
	pid_combo->addItems(pid_items);
	layout()->addWidget(pid_combo);

	QObject::connect(edits['P'], &QLineEdit::textChanged, this, &PIDConfigurator::p_value_changed);
	QObject::connect(edits['I'], &QLineEdit::textChanged, this, &PIDConfigurator::i_value_changed);
	QObject::connect(edits['D'], &QLineEdit::textChanged, this, &PIDConfigurator::d_value_changed);
	connect(pid_combo , SIGNAL(currentIndexChanged(int)), this, SLOT(update_view(int)));

	update_view(pid_combo->currentIndex());
}

template<>
void PIDConfigurator::set_value(char l, double value)
{
	if (transaction)
		values[l] = {value, true};
}

template<>
void PIDConfigurator::set_value(char l, int value)
{
	if (transaction)
		type = value;
}

template<>
double PIDConfigurator::get_value(char l)
{
	bool ok;
	double v = loc.toDouble(edits[l]->text(), &ok);

	if (!ok)
		throw std::runtime_error("Cannot get value from PID configurator.");

	return v;
}

template<>
int PIDConfigurator::get_value(char l)
{
	return pid_combo->currentIndex();
}

void PIDConfigurator::p_value_changed(const QString& str)
{
	value_changed('P');
}

void PIDConfigurator::i_value_changed(const QString& str)
{
	value_changed('I');
}

void PIDConfigurator::d_value_changed(const QString& str)
{
	value_changed('D');
}

void PIDConfigurator::value_changed(char str)
{
	if (!transaction)
	{
		edits[str]->setStyleSheet(edited_value_style);
		model[pid_combo->currentIndex()][str]= {loc.toDouble(edits[str]->text()), false};
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
	transaction = true;

	for (auto edit : edits)
	{
		edit.second->setText(std::to_string(model[index][edit.first].value).c_str());
		QString style = (model[index][edit.first].updated) ?
				updated_value_style :
				edited_value_style;
		edit.second->setStyleSheet(style);
	}

	transaction = false;
}
}
