/*
 *  QColorWidget.h
 *
 *  Created on: 6 gru 2013
 *      Author: Jerzy Spendel
 */
#ifndef QCOLORWIDGET_H
#define QCOLORWIDGET_H

#include <QDockWidget>
#include <QPainter>
class QPaintEvent;
class ColorDockWidget : public QDockWidget{
    Q_OBJECT
private:
    QColor m_color;
public:
    ColorDockWidget(QWidget* parent = 0, Qt::WindowFlags flags = 0);
    void paintEvent(QPaintEvent*);
};

#endif