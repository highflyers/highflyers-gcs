#ifndef BATTERY_H
#define BATTERY_H

#include <QWidget>
#include <QtGui>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>


namespace Ui {
class Battery;
}


namespace HighFlyers
{

class BatteryWidget;
class BatteryBar;




class Battery : public QWidget
{
	Q_OBJECT

public:
	explicit Battery( QWidget *parent = 0 );
	virtual ~Battery();
	QLabel *max, *min, *actual, *volt;
	BatteryBar *status;

public:
	Ui::Battery *ui;

protected:
	void paintEvent( QPaintEvent *event );

};


class BatteryWidget
{
private:

	double maxvalue;
	double minvalue;
	double voltage;
	Battery gui;

public:

	BatteryWidget( double current, double max, double min );

	float get_maxvalue();

	void set_maxvalue( double maximum );

	float get_minvalue();

	void set_minvalue( double miniumum );

	float get_value();

	void set_voltage( double voltage_ );

	int return_percent();

	void update();
};



class BatteryBar : public QWidget
{
	Q_OBJECT

	Q_PROPERTY( Qt::Alignment alignment_ READ getalignment WRITE setAlignment )

public:
	BatteryBar( QWidget *parent = 0 );
	~BatteryBar();

	Qt::Alignment alignment_;
	int percentage;
	int bar_width;
	int margin;

	Qt::Alignment getalignment() const;
	void setAlignment( Qt::Alignment alignment );

protected:
	void paintEvent( QPaintEvent *event );
};

}
#endif // BATTERY_H

