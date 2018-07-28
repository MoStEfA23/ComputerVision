#ifndef COPYMAKEBORDERPLUGIN_H
#define COPYMAKEBORDERPLUGIN_H

#include "copymakeborderplugin_global.h"
#include "cvplugininterface.h"
#include "ui_plugin.h"

namespace Ui {
    class PluginGui;
}

class COPYMAKEBORDERPLUGINSHARED_EXPORT CopyMakeBorderPlugin : public QObject, public CvPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.computervision.cvplugininterface")
    Q_INTERFACES(CvPluginInterface)
public:
    CopyMakeBorderPlugin();
    ~CopyMakeBorderPlugin();

    QString title();
    QString version();
    QString description();
    QString help();
    void setupUi(QWidget* parent);
    void processImage(const cv::Mat &inputImage, cv::Mat &outputImage);


signals:
    void updateNeed();
    void errorMessage(QString msg);
    void infoMessage(QString msg);

private slots:
    void onCurrentBorderTypeIndexChanged(int index);

private:
    Ui::PluginGui *ui;
};

#endif // COPYMAKEBORDERPLUGIN_H
