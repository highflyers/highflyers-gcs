#include "gui/MainWindow.h"
#include "core/CoreController.h"
#include "core/logger/MultiOutPolicy.h"

#include "core/Logger.h"
#include <QApplication>
#include <iostream>


int main( int argc, char** argv )
{
	HighFlyers::MultiOutPolicy p;p.init();
	HighFlyers::Logger<HighFlyers::MultiOutPolicy> logger( HighFlyers::LogLevel::DEBUG, p );
	DEBUG_LOG( "start logger, writing to file: ", p.get_filename() );
	DEBUG_LOG( "running application" );

	QApplication a( argc, argv );
	HighFlyers::MainWindow w;
	HighFlyers::CoreController controller( &w );
	w.show();

	return a.exec();
}
