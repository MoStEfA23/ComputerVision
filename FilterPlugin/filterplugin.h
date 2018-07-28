#ifndef FILTERPLUGIN_H
#define FILTERPLUGIN_H

#include "filterplugin_global.h"
#include "cvplugininterface.h"
#include "ui_plugin.h"

namespace Ui {
    class PluginGui;
}

class FILTERPLUGINSHARED_EXPORT FilterPlugin : public QObject, public CvPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.computervision.cvplugininterface")
    Q_INTERFACES(CvPluginInterface)

    enum e_FilterType{
        BILATERAL_FILTER = 0,
        BLUR_FILTER = 1,
        BOX_FILTER = 2,
        GAUSSIAN_FILTER = 3,
        MEDIAN_FILTER = 4,
        FILTER_2D = 5,
        DERIVATIVES_FILTER = 6,
        MORPH_FILTER = 7
    };
public:
    FilterPlugin();
    ~FilterPlugin();

    QString title();
    QString version();
    QString description();
    QString help();
    void setupUi(QWidget* parent);
    void processImage(const cv::Mat &inputImage, cv::Mat &outputImage);

private:
    void handleConnections();

signals:
    void updateNeed();
    void errorMessage(QString msg);
    void infoMessage(QString msg);

private slots:
    void onGaussianKernelSizeChanged(int kernelSize);
    void onMedianApertureValueChanged(int arg);



private:
    Ui::PluginGui *ui;
};

#endif // FILTERPLUGIN_H
