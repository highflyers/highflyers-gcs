#include "include/battery.h"
#include "ui_battery.h"

Battery::Battery(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Battery)
{
    ui->setupUi(this);
    Battery_Control logic;

/*
    QString style = "QProgressBar {border: 2px solid grey; border-radius: 5px; text-align: center;}";
        style += "QProgressBar::chunk {background-color: #95cd96; width: 10px; margin: 0.5px;}";

        ui->baterry_status->setStyleSheet(style);   // work in progres to change color of the progress bar
*/

    ui->max->setText(QString::number(logic.get_maxvalue()) + " V");
    ui->min->setText(QString::number(logic.get_minvalue()) + " V");
    ui->battery_status->setValue(logic.return_procent());
    ui->actual->setText(QString::number(logic.get_value()) + " V");     // setprecision !
}

Battery::~Battery()
{
    delete ui;
}


Battery_Control::Battery_Control()
{
    // line to add gui function to draw
    set_maxvalue(12.0);
    set_minvalue(6.0);
    set_value(8.11);
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
        value = get_maxvalue();     // maybe send some message about this situation?
    else if(voltage < minvalue)
        value = get_minvalue();
    else
    value = voltage;

    // draw
}

int Battery_Control::return_procent()
{
    return ((value - minvalue) / ( maxvalue - minvalue))*100;
}
