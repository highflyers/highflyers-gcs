/*
 * PIDConfigurator.cpp
 *
 *  Created on: 21 pa? 2013
 *      Author: Marcin Kolny
 */

#include "widgets/PIDConfigurator.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

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

	setLayout(main_layout);
}
