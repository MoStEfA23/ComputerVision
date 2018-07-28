#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPluginLoader>
#include <QPointer>

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

private slots:
    void onPluginActionTriggered(bool);
    void onCorrectPluginUpdateNeeded();

private:
    Ui::MainWindow *ui;

    static const QString PLUGINS_SUBFOLDER;
    static const char* FILE_ONDESK_DYNAMIC_PROPERTY;

    QString mCurrentPluginFile;

    QPointer<QPluginLoader> mCurrentPlugin;
    QPointer<QWidget> mCurrentPluginGui;
};

#endif // MAINWINDOW_H
