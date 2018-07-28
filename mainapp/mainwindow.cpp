#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSettings>
#include <QDir>
#include <QFileInfoList>
#include <QFileInfo>
#include <QLibrary>
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>

#include "../cvplugininterface/cvplugininterface.h"

const QString MainWindow::PLUGINS_SUBFOLDER= "/cvplugins/";
const char* MainWindow::FILE_ONDESK_DYNAMIC_PROPERTY = "absolute_file_path";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->graphicsView->setScene(&mGraphicsScene);
    mGraphicsScene.addItem(&mOriginalPixmap);
    mGraphicsScene.addItem(&mProcessedPixmap);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    handleConnections();

    loadSettings();//Load settings just after construction
    populatePluginMenu();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *ev)
{
    int result = QMessageBox::warning(this, tr("Exit"), tr("Are you sure?"), QMessageBox::Yes, QMessageBox::No);

    if (result == QMessageBox::Yes)
    {
        saveSettings();
        ev->accept();
    }
    else
    {
        ev->ignore();
    }
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

void MainWindow::handleConnections()
{
    connect(ui->actionOpenImage, SIGNAL(triggered(bool)), this, SLOT(openImage()));
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
        connect(mCurrentPlugin->instance(), SIGNAL(infoMessage(QString)), this, SLOT(onCurrentPluginInfoMessage(QString)));
        connect(mCurrentPlugin->instance(), SIGNAL(errorMessage(QString)), this, SLOT(onCurrentPluginErrorMessage(QString)));
    }

}

void MainWindow::onCorrectPluginUpdateNeeded()
{
    if (!mOriginalMat.empty())
    {
        if (!mCurrentPlugin.isNull())
        {
            CvPluginInterface *currentPluginInterface = dynamic_cast<CvPluginInterface*>(mCurrentPlugin->instance());
            if (currentPluginInterface)
            {
                cv::TickMeter tickMeter;
                tickMeter.start();
                currentPluginInterface->processImage(mOriginalMat, mProcessedMat);
                tickMeter.stop();

                qDebug() << Q_FUNC_INFO << "The copyBoard process tooked" << tickMeter.getTimeSec() << "seconds";
            }
        }
        else
        {
            mProcessedMat = mOriginalMat.clone();
        }
    }

    mOriginalImage = QImage(mOriginalMat.data, mOriginalMat.cols, mOriginalMat.rows, mOriginalMat.step, QImage::Format_RGB888);
    mOriginalPixmap.setPixmap(QPixmap::fromImage(mOriginalImage.rgbSwapped()));

    mProcessedImage = QImage(mProcessedMat.data, mProcessedMat.cols, mProcessedMat.rows, mProcessedMat.step, QImage::Format_RGB888);
    mProcessedPixmap.setPixmap(QPixmap::fromImage(mProcessedImage.rgbSwapped()));
}

void MainWindow::onCurrentPluginErrorMessage(QString)
{

}

void MainWindow::onCurrentPluginInfoMessage(QString)
{

}

void MainWindow::openImage()
{
    QString imgFileName = QFileDialog::getOpenFileName(this, tr("Select an image"), QDir::homePath(),
                                                       tr("Image") + "(*.png *.jpg *.bmp)");

    mOriginalMat = cv::imread(imgFileName.toStdString());
    if (!mOriginalMat.empty())
    {
        onCorrectPluginUpdateNeeded();
    }
    else
    {
        if (imgFileName.trimmed().isEmpty())
        {
            QMessageBox::critical(this,
                                  tr("Error"),
                                  tr("Make sure the selected image exist and is accessible"));
        }
    }
}
