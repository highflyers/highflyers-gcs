#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "core/CoreController.h"

#include "core/PluginLoader.h"
#include "apconfig/ApConfig.h"

#include <QHBoxLayout>

using namespace HighFlyers;

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent),
  ui(new Ui::MainWindow),
  controller(nullptr)
{
	ui->setupUi(this);
	QObject::connect(ui->actionLoad_Plugin, SIGNAL(triggered()), this, SLOT(load_plugin()));
	centralWidget()->setLayout(new QHBoxLayout());
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::set_controller(CoreController* controller)
{
	this->controller = controller;
}

void MainWindow::load_plugin()
{
	QString plugin_filename = QFileDialog::getOpenFileName(this, "Open File", "", QString("Dynamic Library Files (*.") +
#ifdef __linux__
			"so"
#elif defined _WIN32
			"dll"
#else
			""); static_assert(false, "Unsupported OS.");(QString()
#endif
					+ ")");

	controller->load_plugin(plugin_filename.toUtf8().constData());
}

void MainWindow::unload_plugin(QString plugin_name)
{
	std::string name = plugin_name.toUtf8().constData();
	int pos = name.find(' ');
	controller->unload_plugin(name.substr(pos+1));
}

void MainWindow::plugin_added(IPluginInterface* plugin, const QString& plugin_name)
{
	if (plugin == nullptr)
		return;

	QAction* action = new QAction("Unload " + plugin_name, nullptr);

	ui->menuPlugins->addAction(action);
	QObject::connect(action, &QAction::triggered, [action, this]{
		unload_plugin(action->text());
	});
	QWidget* plugin_widget = nullptr;

	switch (plugin->get_type_t())
	{
	case PluginType::APCONFIG:
		plugin_widget = static_cast<ApConfig*>(plugin)->get_widget();
		break;
	}

	if (plugin_widget != nullptr)
	{
		// TODO load plugin to a specified place, depends on a plugin's type
		centralWidget()->layout()->addWidget(plugin_widget);
		plugin_widgets[plugin_name] = plugin_widget;
	}
}

void MainWindow::plugin_removed(QString filename)
{
	QList<QAction*> actions = ui->menuPlugins->actions();

	for (QAction* action : actions)
	{
		if (action->text().contains(filename, Qt::CaseSensitive))
		{
			ui->menuPlugins->removeAction(action);
			break;
		}
	}

	if (plugin_widgets.count(filename))
		delete plugin_widgets[filename];
}
