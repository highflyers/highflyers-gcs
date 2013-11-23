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

	float maxvalue;
	float minvalue;
	float voltage;
	Battery gui;

public:

	BatteryWidget( float current = 4, float max = 6, float min = 0 );		//if there are all 0 it doesn't make sense so they are some radomly choose values

	float get_maxvalue();

	void set_maxvalue( float maximum );

	float get_minvalue();

	void set_minvalue( float miniumum );

	float get_value();

	void set_voltage( float voltage_ );

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
	int width_;			//added this underscore for purpose look at paintevent
	int margin;

	Qt::Alignment getalignment() const;
	void setAlignment( Qt::Alignment alignment );

protected:
	void paintEvent( QPaintEvent *event );
};

}
#endif // BATTERY_H

