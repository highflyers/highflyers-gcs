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

const char* PIDConfigurator::pid_names[] = {"P", "I", "D"};

PIDConfigurator::PIDConfigurator(QWidget* parent)
: QWidget(parent)
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

	pid_combo->addItem("pid_items");
	main_layout->addWidget(pid_combo);

	setLayout(main_layout);
}
