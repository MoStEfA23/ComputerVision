#ifndef TEMPLATEPLUGIN_H
#define TEMPLATEPLUGIN_H

#include "templateplugin_global.h"
#include "cvplugininterface.h"
#include "ui_plugin.h"

namespace Ui {
    class PluginGui;
}

class TEMPLATEPLUGINSHARED_EXPORT TemplatePlugin : public QObject, public CvPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.computervision.cvplugininterface")
    Q_INTERFACES(CvPluginInterface)
public:
    TemplatePlugin();
    ~TemplatePlugin();

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


private:
    Ui::PluginGui *ui;
};

#endif // TEMPLATEPLUGIN_H
