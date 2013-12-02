#include "telemetry_controls/Battery.h"
#include "ui_Battery.h"


#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QtGui>


using namespace HighFlyers;

Battery::Battery( QWidget *parent ) :
	QWidget( parent ),
	ui( new Ui::Battery )
{
	ui->setupUi( this );
	max = new QLabel;
	min = new QLabel;
	actual = new QLabel;
	volt = new QLabel;
	status = new BatteryBar;
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

void Battery::paintEvent( QPaintEvent *event )
{
	QHBoxLayout *widget = new QHBoxLayout;

	delete this->layout();
	this->setLayout( widget );		// setting layout to widget

	QHBoxLayout *rest = new QHBoxLayout;
	QVBoxLayout *left = new QVBoxLayout;
	QVBoxLayout *right = new QVBoxLayout;

	widget->addLayout( rest );	// adding layout witch holds left and right layouts
	rest->addLayout( left );
	rest->addLayout( right );

	left->addWidget( max );
	left->setAlignment( max,Qt::AlignTop | Qt::AlignCenter );

	left->addWidget( status );	// here is added progressbar
	status->setAlignment( Qt::AlignHCenter | Qt::AlignTop );

	left->setStretch( 0,0 );		// these instructions set the stretch so progressbar can have right dimensions
	left->setStretch( 1,10 );

	left->addWidget( min );
	min->setAlignment( Qt::AlignBottom | Qt::AlignCenter );

	volt->setText( "Voltage:" );
	right->addWidget( volt );
	right->setAlignment( volt,Qt::AlignBottom );

	right->addWidget( actual );
	actual->setAlignment( Qt::AlignTop );

	this->setMinimumHeight( 200 );
	this->setMinimumWidth( 200 );
}



BatteryWidget::BatteryWidget( double current, double max, double min ): voltage(current), maxvalue(max), minvalue(min)
{
	update();
}


float BatteryWidget::get_maxvalue()
{
	return maxvalue;
}

void BatteryWidget::set_maxvalue( double maximum )
{
	if(maximum > minvalue)
	{
		maxvalue = maximum;
	}
	else
	{
		maxvalue = get_minvalue();
		set_minvalue(maximum);
	}
}

float BatteryWidget::get_minvalue()
{
	return minvalue;
}

void BatteryWidget::set_minvalue( double miniumum )
{
	if(minvalue < maxvalue)
	{
		minvalue = miniumum;
	}
	else
	{
		minvalue = get_maxvalue();
		set_maxvalue(minimum);
	}

}

float BatteryWidget::get_value()
{
	return voltage;
}

void BatteryWidget::set_voltage( double voltage_ )
{
	if(voltage > maxvalue && maxvalue > 0)
	{
		voltage = get_maxvalue();
	}
	else if(voltage < minvalue && minvalue > 0)
	{
		voltage = get_minvalue();
	}
	else if(voltage > minvalue && minvalue < 0)
	{
		voltage = get_minvalue();
	}
	else if(voltage < maxvalue && maxvalue < 0)
	{
		voltage = get_maxvalue();
	}
	else
	{
		voltage = voltage_;
	}

	update();
	gui.show();
}

int BatteryWidget::return_percent()
{
	if( minvalue != maxvalue && minvalue < maxvalue )
	{
		return ( (voltage - minvalue) / ( maxvalue - minvalue) ) * 100;
	}
	else
	{
		return 0;
	}
}

void BatteryWidget::update()
{
	gui.max->setText( QString::number( get_maxvalue() ) + " V" );
	gui.min->setText( QString::number( get_minvalue() ) + " V" );
	gui.actual->setText( QString::number( get_value() ) + " V" );
	gui.status->percentage = return_percent();
}



BatteryBar::BatteryBar( QWidget *parent )
{
	bar_width = 35;
	margin = 2;
}

BatteryBar::~BatteryBar()
{
	;
}


Qt::Alignment BatteryBar::getalignment() const
{
	return alignment_;
}

void BatteryBar::setAlignment( Qt::Alignment alignment )
{
	alignment_ = alignment;
	update();
}


void BatteryBar::paintEvent( QPaintEvent *event )
{
	QPainter mypainter( this );
	mypainter.setRenderHint( QPainter:: Antialiasing );

	int x = 0, y = 0;
		if ( alignment_ & Qt::AlignLeft )
			x = 0;
		else if ( alignment_ & Qt::AlignRight )
			x = width() - bar_width;
		else if ( alignment_ & Qt::AlignHCenter )
			x = (width() - bar_width)/2;
		else if ( alignment_ & Qt::AlignJustify )
			x = 0;

		if ( alignment_ & Qt::AlignTop )
			y = 0;
		else if ( alignment_ & Qt::AlignBottom )
			y = height();
		else if ( alignment_ & Qt::AlignVCenter )
			y = (height())/2;

	QBrush background( Qt::gray,Qt::SolidPattern );
	mypainter.setBrush( background );
	mypainter.drawRoundedRect( x,y,bar_width,height(),10,10 );		// background

	if( percentage > 25 )
	{
		QBrush progressbar( Qt::green,Qt::SolidPattern );
		mypainter.setBrush( progressbar );
	}
	else
	{
		QBrush progressbar( Qt::red,Qt::SolidPattern );
		mypainter.setBrush( progressbar );
	}

	int temp = (height()) * ((float)percentage / 100);	// calculate how height in px should be progressbar rectangle
	int halfmargin = margin / 2;
	int y1 = height() - temp + halfmargin;		// calculate where it should start drawing on y
	mypainter.drawRoundedRect( x+halfmargin, y1, bar_width - margin, temp - margin, 10, 10 );

	if( percentage > 48 )
	{
		QPen textpen(Qt::black);
		mypainter.setPen(textpen);
	}
	else
	{
		QPen textpen(Qt::white);
		mypainter.setPen(textpen);
	}
	mypainter.drawText( width()/2 - bar_width/2 + margin, height()/2 ,30,30,0,QString::number(percentage)  + "%" );
}


