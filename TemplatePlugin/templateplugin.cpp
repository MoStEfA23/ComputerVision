#include "templateplugin.h"


TemplatePlugin::TemplatePlugin()
{
}

TemplatePlugin::~TemplatePlugin()
{

}

QString TemplatePlugin::title()
{
    return this->metaObject()->className();
}

QString TemplatePlugin::version()
{
    return "1.0.0"; //hard coded version for the moment
}

QString TemplatePlugin::description()
{
    return "TemplatePlugin";
}

QString TemplatePlugin::help()
{
    return "Help";
}

void TemplatePlugin::setupUi(QWidget *parent)
{
    Q_UNUSED(parent)
}

void TemplatePlugin::processImage(const cv::Mat &inputImage, cv::Mat &outputImage)
{
    inputImage.copyTo(outputImage);
}
