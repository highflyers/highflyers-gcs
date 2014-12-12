#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "core/plugin_interfaces/IPlugin.h"
#include <QMainWindow>
#include <QMap>

namespace Ui {
class MainWindow;
}

namespace HighFlyers
{
class CoreController;

class MainWindow : public QMainWindow
{
	Q_OBJECT

private:
	Ui::MainWindow *ui;
	CoreController* controller;
	QMap<QString, QWidget*> plugin_widgets;

	void unload_plugin( QString plugin_name );

private Q_SLOTS:
	void load_plugin();

public:
	explicit MainWindow( QWidget *parent = 0 );
	~MainWindow();

	void plugin_added( IPlugin* plugin );
	void plugin_removed( QString filename );

	void set_controller( CoreController* controller );

	void save_state() const;
	void load_last_state();

	void closeEvent(QCloseEvent*);
};

}
#endif // MAINWINDOW_H
