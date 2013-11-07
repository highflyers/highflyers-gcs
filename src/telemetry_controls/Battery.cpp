#include "telemetry_controls/Battery.h"
#include "ui_Battery.h"


#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QtGui>

Battery::Battery(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Battery)
{
	ui->setupUi(this);
	max = new QLabel;
	min = new QLabel;
	actual = new QLabel;
	volt = new QLabel;
	status = new Batterybar;


}


Battery::~Battery()
{
	delete ui;
	delete max;
	delete min;
	delete actual;
	delete volt;
	delete status;
}

void Battery::paintEvent(QPaintEvent *event)
{
	QHBoxLayout *widget = new QHBoxLayout;

	delete this->layout();
	this->setLayout(widget);		// setting layout to widget

	QHBoxLayout *rest = new QHBoxLayout;
	QVBoxLayout *left = new QVBoxLayout;
	QVBoxLayout *right = new QVBoxLayout;

	widget->addLayout(rest);	// adding layout witch holds left and right layouts
	rest->addLayout(left);
	rest->addLayout(right);


	left->addWidget(max);
	left->setAlignment(max,Qt::AlignTop |Qt::AlignCenter);

	left->addWidget(status);	// here is added progress bar
	status->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

	left->setStretch(0,0);		// these instructions set the stretch so progress bar can have right dimensions
	left->setStretch(1,10);

	left->addWidget(min);
	min->setAlignment(Qt::AlignBottom |Qt::AlignCenter);

	volt->setText("Voltage:");
	right->addWidget(volt);
	right->setAlignment(volt,Qt::AlignBottom);

	right->addWidget(actual);
	actual->setAlignment(Qt::AlignTop);

	this->setMinimumHeight(200);
	this->setMinimumWidth(200);
}


Battery_Control::Battery_Control()
{
	set_maxvalue(12.0);
	set_minvalue(6.0);
	set_value(8.0);
	update();
}


float Battery_Control::get_maxvalue()
{
	return maxvalue;
}

void Battery_Control::set_maxvalue(float voltage)
{
	maxvalue = voltage;
}

float Battery_Control::get_minvalue()
{
	return minvalue;
}

void Battery_Control::set_minvalue(float voltage)
{
	minvalue = voltage;
}

float Battery_Control::get_value()
{
	return value;
}

void Battery_Control::set_value(float voltage)
{
	if(voltage > maxvalue)
		value = get_maxvalue();     // maybe send some message about this situation? like "voltage above max"
	else if(voltage < minvalue)
		value = get_minvalue();
	else
	value = voltage;

	update();
	gui.show();
}

int Battery_Control::return_procent()
{
	return ((value - minvalue) / ( maxvalue - minvalue))*100;
}

void Battery_Control::update()
{
	gui.max->setText(QString::number(get_maxvalue()) + " V");
	gui.min->setText(QString::number(get_minvalue()) + " V");
	gui.actual->setText(QString::number(get_value()) + " V");
	gui.status->procent = return_procent();
}



Batterybar::Batterybar(QWidget *parent)
{
	wid = 35;
	margin = 2;
}

Batterybar::~Batterybar()
{
	;	//empty destructor
}


Qt::Alignment Batterybar::getalignment() const
{
	return alignment_;
}

void Batterybar::setAlignment(Qt::Alignment alignment)
{
	alignment_ = alignment;
	update();
}


void Batterybar::paintEvent(QPaintEvent *event)
{
	QPainter mypainter(this);
	mypainter.setRenderHint(QPainter:: Antialiasing);

	int x = 0, y = 0;
		if ( alignment_ & Qt::AlignLeft )
			x = 0;
		else if ( alignment_ & Qt::AlignRight )
			x = width() - wid;
		else if ( alignment_ & Qt::AlignHCenter )
			x = (width() - wid)/2;
		else if ( alignment_ & Qt::AlignJustify )
			x = 0;

		if ( alignment_ & Qt::AlignTop )
			y = 0;
		else if ( alignment_ & Qt::AlignBottom )
			y = height();
		else if ( alignment_ & Qt::AlignVCenter )
			y = (height())/2;

	QBrush background(Qt::gray,Qt::SolidPattern);
	mypainter.setBrush(background);
	mypainter.drawRoundedRect(x,y,wid,height(),10,10);		// background

	if(procent > 25)
	{
		QBrush progresbar(Qt::green,Qt::SolidPattern);
		mypainter.setBrush(progresbar);
	}
	else
	{
		QBrush progresbar(Qt::red,Qt::SolidPattern);
		mypainter.setBrush(progresbar);
	}

	int temp = (height()) * ((float)procent/100);	// calculate how height in px should be progress bar rectangle
	int halfmargin = margin / 2;
	int y1 = height() - temp + halfmargin;		// calculate where it should start drawing on y
	mypainter.drawRoundedRect(x+halfmargin, y1 ,wid-margin, temp-margin,10,10);

	if(procent > 48)
	{
		QPen textpen(Qt::black);
		mypainter.setPen(textpen);
	}
	else
	{
		QPen textpen(Qt::white);
		mypainter.setPen(textpen);
	}
	mypainter.drawText(width()/2 - wid/2 + margin, height()/2 ,30,30,0,QString::number(procent)  + "%");
}


