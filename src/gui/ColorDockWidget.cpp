/*
 *  QColorWidget.h
 *
 *  Created on: 6 gru 2013
 *      Author: Jerzy Spendel
 */
#include "gui.h"
#include <random>
ColorDockWidget::ColorDockWidget(QWidget* parent, Qt::WindowFlags flags): QDockWidget(parent, flags){
    std::default_random_engine engine;
    auto rand = std::bind(std::uniform_int_distribution<int>(0, 255), engine);
    m_color = QColor{rand(), rand(), rand()};
}

void ColorDockWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setBrush(m_color);
    painter.drawRect(QRect{0, 0, width(), height()});
}