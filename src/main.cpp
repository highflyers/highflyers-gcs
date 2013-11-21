#include "gui/MainWindow.h"
#include "core/CoreController.h"
#include <QApplication>

int main( int argc, char** argv )
{
	QApplication a( argc, argv );
	HighFlyers::MainWindow w;
	HighFlyers::CoreController controller( &w );
	w.show();

	return a.exec();
}
