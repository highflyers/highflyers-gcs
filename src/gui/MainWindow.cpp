#include "MainWindow.h"
#include "ui_MainWindow.h"

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
	delete controller;
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

	IPluginInterface* plugin = controller->load_plugin(plugin_filename.toUtf8().constData(), PluginType::UNKNOW);
	QWidget* plugin_widget = nullptr;

	switch (plugin->get_type_t())
	{
	case PluginType::APCONFIG:
		plugin_widget = static_cast<ApConfig*>(plugin)->get_widget();
		break;
	}

	if (plugin_widget != nullptr)
		// TODO load plugin to a specified place, depends on a plugin's type
		centralWidget()->layout()->addWidget(plugin_widget);
}
