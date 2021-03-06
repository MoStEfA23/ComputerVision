#include "filterplugin.h"
#include "ui_plugin.h"

#include <QDebug>


FilterPlugin::FilterPlugin()
{

}

FilterPlugin::~FilterPlugin()
{

}

QString FilterPlugin::title()
{
    return this->metaObject()->className();
}

QString FilterPlugin::version()
{
    return "1.0.0"; //hard coded version for the moment
}

QString FilterPlugin::description()
{
    return "FilterPlugin";
}

QString FilterPlugin::help()
{
    return "Help";
}

void FilterPlugin::setupUi(QWidget *parent)
{
    ui = new Ui::PluginGui;
    ui->setupUi(parent);

    ui->mainTabs->setCurrentIndex(0);

    handleConnections();
}

void FilterPlugin::processImage(const cv::Mat &inputImage, cv::Mat &outputImage)
{
    switch (ui->mainTabs->currentIndex())
    {
    case BILATERAL_FILTER:
    {
        cv::bilateralFilter(inputImage, outputImage, ui->mBilateralDiameterSpinBox->value(),
                            ui->mBilateralSigmaColor->value(), ui->mBilateralSigmaSpace->value());
    }
        break;
    case BLUR_FILTER:
    {
        cv::blur(inputImage, outputImage,
                 cv::Size(ui->mBlurKernelSizeSpinBox->value(), ui->mBlurKernelSizeSpinBox->value()),
                 cv::Point(ui->mBlurAnchoXSpin->value(), ui->mBlurAnchoYSpin->value()));
    }
        break;
    case BOX_FILTER:
    {
        cv::boxFilter(inputImage, outputImage, ui->mBoxDepthSpin->value(),
                      cv::Size(ui->mBoxKernelSizeSpinBox->value(), ui->mBoxKernelSizeSpinBox->value()),
                      cv::Point(ui->mBoxAnchoXSpin->value(), ui->mBoxAnchoYSpin->value()),
                      ui->mBoxNormalCheck->isChecked());
    }
        break;
    case GAUSSIAN_FILTER:
    {
        cv::GaussianBlur(inputImage, outputImage,
                         cv::Size(ui->mGaussKernelSizeSpinBox->value(), ui->mGaussKernelSizeSpinBox->value()),
                         ui->mGaussSigmaXSpin->value(), ui->mGaussSigmaYSpin->value());
    }
        break;
    case MEDIAN_FILTER:
    {
        cv::medianBlur(inputImage, outputImage, ui->mMedianApertureSpin->value());
    }
        break;
    }
}

void FilterPlugin::handleConnections()
{
    connect(ui->mainTabs, SIGNAL(currentChanged(int)), this, SIGNAL(updateNeed()));
    connect(ui->mBilateralDiameterSpinBox, SIGNAL(valueChanged(int)), this, SIGNAL(updateNeed()));
    connect(ui->mBilateralSigmaColor, SIGNAL(valueChanged(double)), this, SIGNAL(updateNeed()));
    connect(ui->mBilateralSigmaSpace, SIGNAL(valueChanged(double)), this, SIGNAL(updateNeed()));
    connect(ui->mBlurAnchoXSpin, SIGNAL(valueChanged(int)), this, SIGNAL(updateNeed()));
    connect(ui->mBlurAnchoYSpin, SIGNAL(valueChanged(int)), this, SIGNAL(updateNeed()));
    connect(ui->mBlurKernelSizeSpinBox, SIGNAL(valueChanged(int)), this, SIGNAL(updateNeed()));
    connect(ui->mBoxAnchoXSpin, SIGNAL(valueChanged(int)), this, SIGNAL(updateNeed()));
    connect(ui->mBoxAnchoYSpin, SIGNAL(valueChanged(int)), this, SIGNAL(updateNeed()));
    connect(ui->mBoxDepthSpin, SIGNAL(valueChanged(int)), this, SIGNAL(updateNeed()));
    connect(ui->mBoxKernelSizeSpinBox, SIGNAL(valueChanged(int)), this, SIGNAL(updateNeed()));
    connect(ui->mBoxNormalCheck, SIGNAL(toggled(bool)), this, SIGNAL(updateNeed()));
    connect(ui->mGaussKernelSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onGaussianKernelSizeChanged(int)));
    connect(ui->mGaussSigmaXSpin, SIGNAL(valueChanged(double)), this, SIGNAL(updateNeed()));
    connect(ui->mGaussSigmaYSpin, SIGNAL(valueChanged(double)), this, SIGNAL(updateNeed()));
    connect(ui->mMedianApertureSpin, SIGNAL(valueChanged(int)), this, SLOT(onMedianApertureValueChanged(int)));
    connect(ui->mDerivSobelRadio, SIGNAL(toggled(bool)), this, SIGNAL(updateNeed()));
    connect(ui->mDerivLaplacRadio, SIGNAL(toggled(bool)), this, SIGNAL(updateNeed()));
    connect(ui->mDerivDeltaSpin, SIGNAL(valueChanged(double)), this, SIGNAL(updateNeed()));
    connect(ui->mDerivScaleSpin, SIGNAL(valueChanged(double)), this, SIGNAL(updateNeed()));
    connect(ui->mDerivScharrRadio, SIGNAL(toggled(bool)), this, SIGNAL(updateNeed()));
    connect(ui->mMorphIterSpin, SIGNAL(valueChanged(int)), this, SIGNAL(updateNeed()));
    connect(ui->mMorphMorphRadio, SIGNAL(toggled(bool)), this, SIGNAL(updateNeed()));
    connect(ui->mMorphShapesCombo, SIGNAL(currentIndexChanged(int)), this, SIGNAL(updateNeed()));
}

void FilterPlugin::onGaussianKernelSizeChanged(int kernelSize)
{
    if(kernelSize % 2 == 1)
    {
        emit updateNeed();
    }
}

void FilterPlugin::onMedianApertureValueChanged(int arg)
{
    if (arg % 2 == 1)
    {
        emit updateNeed();
    }
}
