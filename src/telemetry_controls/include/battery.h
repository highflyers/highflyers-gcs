#ifndef BATTERY_H
#define BATTERY_H

#include <QWidget>
#include <QString>

namespace Ui {
class Battery;
}

class Battery : public QWidget
{
    Q_OBJECT

public:
    explicit Battery(QWidget *parent = 0);
    ~Battery();

private:
    Ui::Battery *ui;
};


class Battery_Control
{
private:

    float maxvalue;
    float minvalue;
    float value;

public:

    Battery_Control();

    float get_maxvalue();

    void set_maxvalue(float voltage);

    float get_minvalue();

    void set_minvalue(float voltage);

    float get_value();

    void set_value(float voltage);

    int return_procent();
};

#endif // BATTERY_H
