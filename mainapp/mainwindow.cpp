#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSettings>
#include <QDir>
#include <QFileInfoList>
#include <QFileInfo>
#include <QLibrary>
#include <QMessageBox>
#include <QDebug>

#include "../cvplugininterface/cvplugininterface.h"

const QString MainWindow::PLUGINS_SUBFOLDER= "/cvplugins/";
const char* MainWindow::FILE_ONDESK_DYNAMIC_PROPERTY = "absolute_file_path";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    loadSettings();//Load settings just after construction

    populatePluginMenu();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::saveSettings()
{
    QSettings settings("Mostefa", "ComputerVision", this);

    settings.setValue("currentPluginFile", mCurrentPluginFile);
}

void MainWindow::loadSettings()
{
    QSettings settings("Mostefa", "ComputerVision", this);

    mCurrentPluginFile = settings.value("currentPluginFile", "").toString();
}

void MainWindow::populatePluginMenu()
{
    QDir pluginsDir = (qApp->applicationDirPath() + PLUGINS_SUBFOLDER);

    QFileInfoList pluginFiles = pluginsDir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files, QDir::Name);

    foreach (QFileInfo pluginFile, pluginFiles)
    {
        if (QLibrary::isLibrary(pluginFile.absoluteFilePath()))
        {
            QPluginLoader pluginLoader(pluginFile.absoluteFilePath(), this);

            CvPluginInterface* plugin = dynamic_cast<CvPluginInterface*>(pluginLoader.instance());
            if (plugin)
            {
                QAction* pluginAction = ui->mMenuPlugins->addAction(plugin->title());
                pluginAction->setProperty(FILE_ONDESK_DYNAMIC_PROPERTY, pluginFile.absoluteFilePath());

                connect(pluginAction, SIGNAL(triggered(bool)), this, SLOT(onPluginActionTriggered(bool)));

                if (mCurrentPluginFile == pluginFile.absoluteFilePath())
                {
                    pluginAction->trigger();
                }
            }
            else
            {
                QMessageBox::warning(this, tr("Warning"),
                                     QString(tr("Make sure %1 is a correct plugin for this application<br>"
                                                "and it's not in use by some other application!")).arg(pluginFile.fileName()));
            }
        }
        else
        {
            QMessageBox::warning(this, tr("Warning"),
                                 QString(tr("Make sure only plugins exist in %1 folder.<br>"
                                            "%2 is not a plugin."))
                                 .arg(PLUGINS_SUBFOLDER)
                                 .arg(pluginFile.fileName()));
        }
    }

    if(ui->mMenuPlugins->actions().count() <= 0)
    {
        QMessageBox::critical(this, tr("No Plugins"), QString(tr("This application cannot work without plugins!"
                                                                 "<br>Make sure that %1 folder exists "
                                                                 "in the same folder as the application<br>and that "
                                                                 "there are some filter plugins inside it")).arg(PLUGINS_SUBFOLDER));
        this->setEnabled(false);
    }
}

/**
 * @brief MainWindow::onPluginActionTriggered
 */
void MainWindow::onPluginActionTriggered(bool)
{
    if (!mCurrentPluginFile.isNull())
    {
        delete mCurrentPlugin;
        delete mCurrentPluginGui;
    }

    mCurrentPluginFile = QObject::sender()->property(FILE_ONDESK_DYNAMIC_PROPERTY).toString();
    mCurrentPlugin = new QPluginLoader(mCurrentPluginFile, this);
    mCurrentPluginGui = new QWidget(this);
    ui->mPluginLayout->addWidget(mCurrentPluginGui);

    CvPluginInterface* currentPluginInstance = dynamic_cast<CvPluginInterface*>(mCurrentPlugin->instance());
    if (currentPluginInstance)
    {
        currentPluginInstance->setupUi(mCurrentPluginGui);

        connect(mCurrentPlugin->instance(), SIGNAL(updateNeed()), this, SLOT(onCorrectPluginUpdateNeeded()));
    }

}

void MainWindow::onCorrectPluginUpdateNeeded()
{
    qDebug() << "update needed";
}
