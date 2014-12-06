/*
 *  QColorWidget.h
 *
 *  Created on: 6 gru 2013
 *      Author: Jerzy Spendel
 */
#include "gui.h"
ColorDockWidget::ColorDockWidget(QWidget* parent, Qt::WindowFlags flags): QDockWidget(parent, flags){
    srand(time(NULL)); //gdzie by indziej dac sranda coby nie wykonywało się to wielokrotnie?
    m_color = QColor{rand()%256, rand()%256, rand()%256}; //Jakby byly biale to ledwo daloby sie zobaczyc ktory jest gdzie
}

void ColorDockWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setBrush(m_color);
    painter.drawRect(QRect{0, 0, width(), height()});
}