#include "gui/MainWindow.h"
#include <QApplication>

int main(int argc, char** argv)
{
	QApplication a(argc, argv);
	HighFlyers::MainWindow w;
	w.set_controller(new HighFlyers::CoreController());
	w.show();

	return a.exec();
}
