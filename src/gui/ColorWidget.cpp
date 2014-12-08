/*
 *  QColorWidget.h
 *
 *  Created on: 6 gru 2013
 *      Author: Jerzy Spendel
 */
#include "gui.h"
#include <random>
#include <iostream>
using namespace HighFlyers;
ColorWidget::ColorWidget(QWidget* parent, Qt::WindowFlags flags): QWidget(parent, flags)
{
    std::default_random_engine engine(time(NULL));
    auto rand = std::bind(std::uniform_int_distribution<int>(0, 255), engine);
    m_color = QColor{rand(), rand(), rand()};
    print_color();
}

void ColorWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setBrush(m_color);
    painter.drawRect(QRect{0, 0, width(), height()});
}

void ColorWidget::print_color() const
{
    std::cout << "RED: " << m_color.red() << std::endl;
    std::cout << "GREEN: " << m_color.green() << std::endl;
    std::cout << "BLUE:" << m_color.blue() << std::endl;
}