/*
 *  QColorWidget.h
 *
 *  Created on: 6 gru 2013
 *      Author: Jerzy Spendel
 */
#ifndef QCOLORWIDGET_H
#define QCOLORWIDGET_H

#include <QWidget>
#include <QPainter>

class QPaintEvent;
namespace HighFlyers
{

    class ColorWidget : public QWidget {
    Q_OBJECT
    private:
        QColor m_color;
    public:
        ColorWidget(QWidget *parent = 0, Qt::WindowFlags flags = 0);

        void paintEvent(QPaintEvent *);
        void print_color() const;
    };
}
#endif