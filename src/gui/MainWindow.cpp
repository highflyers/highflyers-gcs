#include "gui.h"
#include "ui_MainWindow.h"

#include "core/CoreController.h"

#include "core/PluginLoader.h"
#include "core/plugin_interfaces.h"

#include <QHBoxLayout>

using namespace HighFlyers;

MainWindow::MainWindow( QWidget *parent )
: QMainWindow( parent ),
  ui( new Ui::MainWindow ),
  controller( nullptr )
{
	ui->setupUi( this );
	QObject::connect( ui->actionLoad_Plugin, SIGNAL( triggered() ), this, SLOT( load_plugin() ) );
	centralWidget()->setLayout( new QHBoxLayout() );
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::set_controller( CoreController* controller )
{
	this->controller = controller;
}

void MainWindow::load_plugin()
{
	QString plugin_filename = QFileDialog::getOpenFileName( this, "Open File", "", QString( "Dynamic Library Files (*.") +
#ifdef __linux__
			"so"
#elif defined _WIN32
			"dll"
#else
			"" ); static_assert( false, "Unsupported OS." );( QString()
#endif
					+ ")" );

	if (!plugin_filename.isNull())
		controller->load_plugin( plugin_filename.toUtf8().constData() );
}

void MainWindow::unload_plugin( QString plugin_name )
{
	std::string name = plugin_name.toUtf8().constData();
	int pos = name.find( ' ' );
	controller->unload_plugin( name.substr( pos+1 ) );
}

void MainWindow::plugin_added( IPlugin* plugin, const QString& plugin_name )
{
	if (plugin == nullptr)
		return;

	QAction* action = new QAction( "Unload " + plugin_name, nullptr );

	ui->menuPlugins->addAction( action );
	QObject::connect( action, &QAction::triggered, [action, this]{
		unload_plugin( action->text() );
	} );
	QWidget* plugin_widget;

	switch (plugin->get_type_t())
	{
	case PluginType::APCONFIG:
		plugin_widget = static_cast<IApConfigPlugin*>( plugin )->get_widget();
		break;
	case PluginType::VIDEO:
	{
		IVideoSourcePlugin* casted_plugin =
				static_cast<IVideoSourcePlugin*>( plugin );

		plugin_widget = new VideoPlayer();
		VideoPlayer* video_widget= static_cast<VideoPlayer*>( plugin_widget );

		WId xwinid = video_widget->get_canvas_handler();
		connect( video_widget, &VideoPlayer::state_clicked, [casted_plugin]( bool play ){
			if (play)
				casted_plugin->play( false );
			else
				casted_plugin->stop();
		} );
		connect( video_widget, &VideoPlayer::config_window_requested, [casted_plugin]{
			if (casted_plugin->get_config_window() != nullptr)
				casted_plugin->get_config_window()->show();
			else
				QMessageBox::information(0, "No Config", "Configuration is not available for this plugin.");
		} );
		casted_plugin->set_render_window( xwinid );
		break;
	}
	case PluginType::COMMUNICATION:
	{
		QAction* action = new QAction( "Properties " + plugin_name + "...", nullptr );

		ui->menuCommunication->addAction( action );
		QObject::connect( action, &QAction::triggered, [plugin]{
			QWidget* config_widget = static_cast<ICommunicationPlugin*>( plugin )->get_configuration_widget();

			if (config_widget != nullptr)
				config_widget->show();
		});
		break;
	}
	default:
		plugin_widget = nullptr;
		break;
	}

	if (plugin_widget != nullptr)
	{
		// TODO load plugin to a specified place, depends on a plugin's type
		centralWidget()->layout()->addWidget( plugin_widget );
		plugin_widgets[plugin_name] = plugin_widget;
	}
}

void MainWindow::plugin_removed( QString filename )
{
	for (QAction* action : ui->menuPlugins->actions())
	{
		if (action->text().contains( filename, Qt::CaseSensitive ))
		{
			ui->menuPlugins->removeAction( action );
			break;
		}
	}

	for (QAction* action : ui->menuCommunication->actions())
	{
		if (action->text().contains( filename, Qt::CaseSensitive ))
		{
			ui->menuPlugins->removeAction( action );
			break;
		}
	}

	if (plugin_widgets.count( filename ))
		delete plugin_widgets[filename];
}
