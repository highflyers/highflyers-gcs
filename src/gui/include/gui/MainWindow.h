#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "core/plugin_interfaces/IPlugin.h"
#include <QMainWindow>
#include <map>

namespace Ui {
class MainWindow;
}

namespace HighFlyers
{
class CoreController;

class MainWindow : public QMainWindow
{
	Q_OBJECT

	CoreController* controller;

private Q_SLOTS:
	void load_plugin();

public:
	explicit MainWindow( QWidget *parent = 0 );
	~MainWindow();

	void plugin_added( IPlugin* plugin, const QString& plugin_name );
	void plugin_removed( QString filename );

	void set_controller( CoreController* controller );

private:
	Ui::MainWindow *ui;
	std::map<QString, QWidget*> plugin_widgets;

	void unload_plugin( QString plugin_name );
};

}
#endif // MAINWINDOW_H
