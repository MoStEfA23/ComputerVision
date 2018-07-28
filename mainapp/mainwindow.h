#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPluginLoader>
#include <QPointer>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>

#include "opencv2/world.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void saveSettings();
    void loadSettings();

    void populatePluginMenu();
    void handleConnections();

private slots:
    void onPluginActionTriggered(bool);
    void onCorrectPluginUpdateNeeded();
    void onCurrentPluginErrorMessage(QString);
    void onCurrentPluginInfoMessage(QString);

    void openImage();

private:
    static const QString PLUGINS_SUBFOLDER;
    static const char* FILE_ONDESK_DYNAMIC_PROPERTY;


    Ui::MainWindow *ui;
    QGraphicsScene mGraphicsScene;

    QString mCurrentPluginFile;

    QPointer<QPluginLoader> mCurrentPlugin;
    QPointer<QWidget> mCurrentPluginGui;

    QGraphicsPixmapItem mOriginalPixmap;
    QGraphicsPixmapItem mProcessedPixmap;

    cv::Mat mOriginalMat;
    cv::Mat mProcessedMat;

    QImage mOriginalImage;
    QImage mProcessedImage;
};

#endif // MAINWINDOW_H
