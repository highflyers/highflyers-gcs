#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "core/CoreController.h"
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

namespace HighFlyers
{

class MainWindow : public QMainWindow
{
	Q_OBJECT

	CoreController* controller;

private Q_SLOTS:
	void load_plugin();

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	void set_controller(CoreController* controller);
private:
	Ui::MainWindow *ui;
};

}
#endif // MAINWINDOW_H
