#ifndef BATTERY_H
#define BATTERY_H

#include <QWidget>
#include <QtGui>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>


class Battery_Control;
class Batterybar;

namespace Ui {
class Battery;
}

class Battery : public QWidget
{
	Q_OBJECT

public:
	explicit Battery(QWidget *parent = 0);
	virtual ~Battery();
	QLabel *max, *min,*actual,*volt;
	Batterybar *status;

public:
	Ui::Battery *ui;

protected:
	void paintEvent(QPaintEvent *event);

};


class Battery_Control
{
private:

	float maxvalue;
	float minvalue;
	float value;
	Battery gui;

public:

	Battery_Control();

	float get_maxvalue();

	void set_maxvalue(float voltage);

	float get_minvalue();

	void set_minvalue(float voltage);

	float get_value();

	void set_value(float voltage);

	int return_procent();

	void update();
};



class Batterybar : public QWidget
{
	Q_OBJECT

	Q_PROPERTY(Qt::Alignment alignment_ READ getalignment WRITE setAlignment)

public:
	Batterybar(QWidget *parent = 0);
	~Batterybar();

	Qt::Alignment alignment_;
	int procent;
	int wid;
	int margin;

	Qt::Alignment getalignment() const;
	void setAlignment(Qt::Alignment alignment);

protected:
	void paintEvent(QPaintEvent *event);
};

#endif // BATTERY_H

